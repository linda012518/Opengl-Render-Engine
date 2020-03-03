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

			uniform sampler2D albedoMap;	//������(Albedo)����	�����������а���һЩϸС����Ӱ������ɫ�����ƣ�������������û�У�ֻ�����������ɫ��������������ϵ����
			uniform sampler2D normalMap;
			uniform sampler2D metallicMap;	//����(Metallic)��ͼ������ص�ָ���������ǲ��ǽ����ʵص�
			uniform sampler2D roughnessMap;	//�ֲڶ�(Roughness)��ͼ
			uniform sampler2D aoMap;		//�������ڱ�(Ambient Occlusion)��ͼ		ָ����һ���������Ӱ��Ϣ

			// IBL
			uniform samplerCube irradianceMap;
			uniform samplerCube prefilterMap;
			uniform sampler2D	brdfLUT;

			// lights
			uniform vec3 lightPositions[4];
			uniform vec3 lightColors[4];

			uniform vec3 viewPos;

			//��ȡ���з��ߵ�����ռ�
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

			//BRDF�ľ��淴�䲿��		��̬�ֲ�����(Normal Distribution Function)
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
			//BRDF�ľ��淴�䲿��		���κ���(Geometry Function)
			float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
			{
				float NdotV = max(dot(N, V), 0.0);
				float NdotL = max(dot(N, L), 0.0);
				float ggx2	= GeometrySchlickGGX(NdotV, roughness);
				float ggx1	= GeometrySchlickGGX(NdotL, roughness);

				return ggx1 * ggx2;
			}

			//BRDF�ľ��淴�䲿��		����������(Fresnel Rquation)
			vec3 fresnelSchlick(float cosTheta, vec3 F0)
			{
				return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
			}
			//BRDF�ľ��淴�䲿��		����������(Fresnel Rquation)		���˴ֲڶ�
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

				//���㷨������ʱ�ķ����ʣ� �������ʣ��������ϣ�ʹ�õ�F0Ϊ0.04������ǽ�������ʹ�÷�������ɫ��ΪF0�������������̣�
				vec3	F0			=	vec3(0.04);
						F0			=	mix(F0, albedo, metallic);

				// �����ʷ���
				vec3	Lo			=	vec3(0.0);
				for (int i = 0; i < 4; ++i)
				{
					// ����ÿ�������
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
					float	denominator =	4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // +0.001��ֹ�����
					vec3	specular	=	nominator / denominator;

					// kS���ڷ�����
					vec3	kS			=	F;
					// �����غ�	������;��淴����ӵ���1		kD���� 1.0 - kS
					vec3	kD			=	vec3(1.0) - kS;
					
							//�ǽ���������������		�������Ϊ��������Ϊ���Ի�ϣ�������û������⣩
							kD			*=	1.0 - metallic;

					//������ǿ��			=	dot(�ƹⷽ��, ���߷���)
					float	NdotL		=	max(dot(N, L), 0.0);

							//��ӵ��������Lo	��ʽ���ۼӵ�
							//�����Ѿ���BRDF���Է�������kS����������ǲ����ٳ���kS
							Lo			+=	(kD * albedo / PI + specular) * radiance * NdotL;
				}

				//��������
				vec3	F				=	fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

				vec3	kS				=	F;
				vec3	kD				=	1.0 - kS;
						kD				*=	1.0 - metallic;

				vec3	irradiance		=	texture(irradianceMap, N).rgb;
				vec3	diffuse			=	irradiance * albedo;

				//ͬʱ��Ԥ������ͼ��BRDF lut������������Split-Sum���ƽ����������һ�𣬵õ�IBL���淴�䲿�֡�
				const float MAX_REFLECTION_LOD = 4.0;
				vec3	prefilteredColor=	textureLod(prefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;
				vec2	brdf			=	texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
				vec3	specular		=	prefilteredColor * (F * brdf.x + brdf.y);

				vec3	ambient			=	(kD * diffuse + specular) * ao;

				vec3	color			=	ambient + Lo;

						//hdrɫ��ӳ�� 
						color			=	color / (color + vec3(1.0));
						//gammaУ��
						color			=	pow(color, vec3(1.0 / 2.2));

				gl_FragColor			=	vec4(color, 1.0);
			}
		);

		return Shader::initialize();
	}

}