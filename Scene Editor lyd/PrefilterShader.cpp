#include "PrefilterShader.h"


namespace lyd
{
	PrefilterShader::PrefilterShader()
	{
		initialize();
	}

	PrefilterShader::~PrefilterShader() { }

	bool PrefilterShader::initialize()
	{
		vs = STR(
			#version 330 core\n

			layout(location = 0) in vec3 inPos;

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;

			out vec3 worldPos;

			void main()
			{
				worldPos	= vec3(model * vec4(inPos, 1.0));
				gl_Position = projection * view * model * vec4(inPos, 1.0);
			}
			);
		fs = STR(
			#version 330 core\n

			uniform samplerCube	environmentMap;
			uniform float		roughness;
			in vec3 worldPos;

			const float PI = 3.14159265359;

			//BRDF的镜面反射部分		正态分布函数(Normal Distribution Function)
			float DistributionGGX(vec3 N, vec3 H, float roughness)
			{
				float	a		=	roughness*roughness;
				float	a2		=	a*a;
				float	NdotH	=	max(dot(N, H), 0.0);
				float	NdotH2	=	NdotH*NdotH;

				float	nom		=	a2;
				float	denom	=	(NdotH2 * (a2 - 1.0) + 1.0);
						denom	=	PI * denom * denom;

				return	nom / denom;
			}

			// http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
			//Van Der Corput 序列
			float RadicalInverse_VdC(uint bits)
			{
				bits = (bits << 16u) | (bits >> 16u);
				bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
				bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
				bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
				bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
				return float(bits) * 2.3283064365386963e-10; // / 0x100000000
			}
			//并非所有 OpenGL 相关驱动程序都支持位运算符（例如WebGL和OpenGL ES 2.0）
			//替代 Van Der Corput 序列
			float VanDerCorpus(uint n, uint base)
			{
				float invBase	=	1.0 / float(base);
				float denom		=	1.0;
				float result	=	0.0;

				for (uint i = 0u; i < 32u; ++i)
				{
					if (n > 0u)
					{
						denom	=	mod(float(n), 2.0);
						result	+=	denom * invBase;
						invBase =	invBase / 2.0;
						n		=	uint(float(n) / 2.0);
					}
				}
				return result;
			}
			//低差异序列生成蒙特卡洛样本向量，该过程称为拟蒙特卡洛积分		拟蒙特卡洛方法具有更快的收敛速度
			vec2 Hammersley(uint i, uint N)
			{
				return vec2(float(i) / float(N), RadicalInverse_VdC(i));
				//return vec2(float(i) / float(N), VanDerCorpus(i, 2u));
			}
			//GGX 重要性采样
			vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
			{
				float	a			=	roughness*roughness;

				float	phi			=	2.0 * PI * Xi.x;
				float	cosTheta	=	sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
				float	sinTheta	=	sqrt(1.0 - cosTheta*cosTheta);

				// from spherical coordinates to cartesian coordinates - halfway vector
				vec3	H;
						H.x			=	cos(phi) * sinTheta;
						H.y			=	sin(phi) * sinTheta;
						H.z			=	cosTheta;

				// from tangent-space H vector to world-space sample vector
				vec3	up			=	abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
				vec3	tangent		=	normalize(cross(up, N));
				vec3	bitangent	=	cross(N, tangent);

				vec3	sampleVec	=	tangent * H.x + bitangent * H.y + N * H.z;
				return	normalize(sampleVec);
			}

			void main()
			{
				//预滤波环境贴图			预先计算的环境卷积贴图，但这次考虑了粗糙度
				//随着粗糙度的增加		参与环境贴图卷积的采样向量会更分散，导致反射更模糊
				//卷积的每个粗糙度级别		按顺序把模糊后的结果存储在预滤波贴图的 mipmap 中
				vec3 N = normalize(worldPos);

				// make the simplyfying assumption that V equals R equals the normal 
				vec3 R = N;
				vec3 V = R;

				const uint SAMPLE_COUNT = 1024u;
				vec3 prefilteredColor = vec3(0.0);
				float totalWeight = 0.0;

				for (uint i = 0u; i < SAMPLE_COUNT; ++i)
				{
					// generates a sample vector that's biased towards the preferred alignment direction (importance sampling).
					vec2	Xi		=	Hammersley(i, SAMPLE_COUNT);
					vec3	H		=	ImportanceSampleGGX(Xi, N, roughness);
					vec3	L		=	normalize(2.0 * dot(V, H) * H - V);

					float	NdotL	=	max(dot(N, L), 0.0);
					if (NdotL > 0.0)
					{
						// sample from the environment's mip level based on roughness/pdf
						float	D					=	DistributionGGX(N, H, roughness);
						float	NdotH				=	max(dot(N, H), 0.0);
						float	HdotV				=	max(dot(H, V), 0.0);
						float	pdf					=	D * NdotH / (4.0 * HdotV) + 0.0001;

						float	resolution			=	512.0; // 立方体贴图的分辨率
						float	saTexel				=	4.0 * PI / (6.0 * resolution * resolution);
						float	saSample			=	1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);

						float	mipLevel			=	roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);

								prefilteredColor	+=	textureLod(environmentMap, L, mipLevel).rgb * NdotL;
								totalWeight			+=	NdotL;
					}
				}

				prefilteredColor	=	prefilteredColor / totalWeight;
				gl_FragColor		=	vec4(prefilteredColor, 1.0);
			}
		);

		return Shader::initialize();
	}

}