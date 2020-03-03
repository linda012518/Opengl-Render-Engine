#include "PbrShader.h"

namespace lyd
{
	PbrShader::PbrShader()
	{
		initialize();
	}

	PbrShader::~PbrShader() { }

	bool PbrShader::initialize()
	{
		vs = STR(
			#version 330 core\n

			layout(location = 0) in vec3 inPos;
			layout(location = 1) in vec3 inNormal;
			layout(location = 2) in vec2 inTexCoords;

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;

			out vec2 uv;
			out vec3 normal;
			out vec3 worldPos;

			void main()
			{
				uv			=	inTexCoords;
				worldPos	=	vec3(model * vec4(inPos, 1.0));
				normal		=	mat3(model) * inNormal;
				gl_Position =	projection * view * vec4(worldPos, 1.0);
			}
		);
		fs = STR(
			#version 330 core\n

			in vec2 uv;
			in vec3 normal;
			in vec3 worldPos;

			const float PI = 3.14159265359;

			uniform sampler2D albedoMap;	//反照率(Albedo)纹理	漫反射纹理中包含一些细小的阴影或者深色的裂纹，反照率纹理中没有，只包含表面的颜色（或者折射吸收系数）
			uniform sampler2D normalMap;
			uniform sampler2D metallicMap;	//金属(Metallic)贴图逐个纹素的指定该纹素是不是金属质地的
			uniform sampler2D roughnessMap;	//粗糙度(Roughness)贴图
			uniform sampler2D aoMap;		//环境光遮蔽(Ambient Occlusion)贴图		指定了一个额外的阴影信息

			// IBL
			uniform samplerCube irradianceMap;
			uniform samplerCube prefilterMap;
			uniform sampler2D	brdfLUT;

			// lights
			uniform vec3 lightPositions[4];
			uniform vec3 lightColors[4];

			uniform vec3 viewPos;

			//获取正切法线到世界空间
			vec3 getNormalFromMap()
			{
				vec3 tangentNormal	=	texture(normalMap, uv).xyz * 2.0 - 1.0;

				vec3 Q1				=	dFdx(worldPos);
				vec3 Q2				=	dFdy(worldPos);
				vec2 st1			=	dFdx(uv);
				vec2 st2			=	dFdy(uv);

				vec3 N				=	normalize(normal);
				vec3 T				=	normalize(Q1*st2.t - Q2*st1.t);
				vec3 B				=	-normalize(cross(N, T));
				mat3 TBN			=	mat3(T, B, N);

				return normalize(TBN * tangentNormal);
			}

			//BRDF的镜面反射部分		正态分布函数(Normal Distribution Function)
			float DistributionGGX(vec3 N, vec3 H, float roughness)
			{
				float	a			=	roughness*roughness;
				float	a2			=	a*a;
				float	NdotH		=	max(dot(N, H), 0.0);
				float	NdotH2		=	NdotH*NdotH;

				float	nom			=	a2;
				float	denom		=	(NdotH2 * (a2 - 1.0) + 1.0);
						denom		=	PI * denom * denom;

				return	nom / denom;
			}

			float GeometrySchlickGGX(float NdotV, float roughness)
			{
				float	r		=	(roughness + 1.0);
				float	k		=	(r*r) / 8.0;

				float	nom		=	NdotV;
				float	denom	=	NdotV * (1.0 - k) + k;

				return	nom / denom;
			}
			//BRDF的镜面反射部分		几何函数(Geometry Function)
			float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
			{
				float NdotV = max(dot(N, V), 0.0);
				float NdotL = max(dot(N, L), 0.0);
				float ggx2	= GeometrySchlickGGX(NdotV, roughness);
				float ggx1	= GeometrySchlickGGX(NdotL, roughness);

				return ggx1 * ggx2;
			}

			//BRDF的镜面反射部分		菲涅尔方程(Fresnel Rquation)
			vec3 fresnelSchlick(float cosTheta, vec3 F0)
			{
				return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
			}
			//BRDF的镜面反射部分		菲涅尔方程(Fresnel Rquation)		加了粗糙度
			vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
			{
				return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
			}

			void main()
			{
				vec3	albedo		=	pow(texture(albedoMap, uv).rgb, vec3(2.2));
				float	metallic	=	texture(metallicMap, uv).r;
				float	roughness	=	texture(roughnessMap, uv).r;
				float	ao			=	texture(aoMap, uv).r;

				vec3	N			=	getNormalFromMap();
				vec3	V			=	normalize(viewPos - worldPos);
				vec3	R			=	reflect(-V, N);

				//计算法向入射时的反射率； 如果电介质（例如塑料）使用的F0为0.04，如果是金属，则使用反照率颜色作为F0（金属工作流程）
				vec3	F0			=	vec3(0.04);
						F0			=	mix(F0, albedo, metallic);

				// 反射率方程
				vec3	Lo			=	vec3(0.0);
				for (int i = 0; i < 4; ++i)
				{
					// 计算每个光辐射
					vec3	L			=	normalize(lightPositions[i] - worldPos);
					vec3	H			=	normalize(V + L);
					float	distance	=	length(lightPositions[i] - worldPos);
					float	attenuation =	1.0 / (distance * distance);
					vec3	radiance	=	lightColors[i] * attenuation;

					// Cook-Torrance BRDF
					float	NDF			=	DistributionGGX(N, H, roughness);
					float	G			=	GeometrySmith(N, V, L, roughness);
					vec3	F			=	fresnelSchlick(max(dot(H, V), 0.0), F0);

					vec3	nominator	=	NDF * G * F;
					float	denominator =	4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // +0.001防止被零除
					vec3	specular	=	nominator / denominator;

					// kS等于菲涅耳
					vec3	kS			=	F;
					// 能量守恒	漫反射和镜面反射相加等于1		kD等于 1.0 - kS
					vec3	kD			=	vec3(1.0) - kS;
					
							//非金属具有漫射照明		如果部分为金属，则为线性混合（纯金属没有漫射光）
							kD			*=	1.0 - metallic;

					//漫反射强度			=	dot(灯光方向, 法线方向)
					float	NdotL		=	max(dot(N, L), 0.0);

							//添加到外向辐射Lo	公式是累加的
							//我们已经将BRDF乘以菲涅耳（kS），因此我们不再再乘以kS
							Lo			+=	(kD * albedo / PI + specular) * radiance * NdotL;
				}

				//环境照明
				vec3	F				=	fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

				vec3	kS				=	F;
				vec3	kD				=	1.0 - kS;
						kD				*=	1.0 - metallic;

				vec3	irradiance		=	texture(irradianceMap, N).rgb;
				vec3	diffuse			=	irradiance * albedo;

				//同时对预过滤器图和BRDF lut采样，并按照Split-Sum近似将它们组合在一起，得到IBL镜面反射部分。
				const float MAX_REFLECTION_LOD = 4.0;
				vec3	prefilteredColor=	textureLod(prefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;
				vec2	brdf			=	texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
				vec3	specular		=	prefilteredColor * (F * brdf.x + brdf.y);

				vec3	ambient			=	(kD * diffuse + specular) * ao;

				vec3	color			=	ambient + Lo;

						//hdr色调映射 
						color			=	color / (color + vec3(1.0));
						//gamma校正
						color			=	pow(color, vec3(1.0 / 2.2));

				gl_FragColor			=	vec4(color, 1.0);
			}
		);

		return Shader::initialize();
	}

}