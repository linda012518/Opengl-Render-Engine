#include "NormalMapShader.h"


namespace lyd
{
	NormalMapShader::NormalMapShader()
	{
		initialize();
	}

	NormalMapShader::~NormalMapShader()
	{
	}

	bool NormalMapShader::initialize()
	{
		vs = STR(
			#version 330 core\n

			layout(location = 0) in vec3 inPos;
			layout(location = 1) in vec3 inNormal;//����
			layout(location = 2) in vec2 inTexCoords;
			layout(location = 3) in vec3 inTangent;//����
			layout(location = 5) in vec3 inBitangent;//������

			out VS_OUT{
				vec3 worldPos;
				vec2 uv;
				vec3 Tangent_lightDir;
				vec3 Tangent_viewPos;
				vec3 Tangent_worldPos;
			} vs_out;

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;

			uniform vec3 lightDir;
			uniform vec3 viewPos;

			void main()
			{
				vs_out.worldPos		=	vec3(model * vec4(inPos, 1.0));
				vs_out.uv			=	inTexCoords;

				//��������ͼӦ�õ���Щ����ʱ����������ƽ����ͨ���ܻ�ø��ø�ƽ���Ľ����
				//�������и����⣬����TBN�������ܻ᲻�ܻ��ഹֱ������ζ��TBN������������������
				vec3	T	=	normalize(mat3(model) * inTangent);
				vec3	N	=	normalize(mat3(model) * inNormal);
						T	=	normalize(T - dot(T, N) * N);
				vec3	B	=	cross(N, T);

				//���������ת�õ��������		��������ص�����ռ�����ת�䵽�������÷��������Ŀռ�
				mat3	TBN	=	transpose(mat3(T, B, N));
				vs_out.Tangent_lightDir =	TBN * lightDir;
				vs_out.Tangent_viewPos	=	TBN * viewPos;
				vs_out.Tangent_worldPos =	TBN * vs_out.worldPos;

				gl_Position				=	projection * view * model * vec4(inPos, 1.0);
			}
		);
		fs = STR(
			#version 330 core\n

			in VS_OUT{
				vec3 worldPos;
				vec2 uv;
				vec3 Tangent_lightDir;
				vec3 Tangent_viewPos;
				vec3 Tangent_worldPos;
			} fs_in;

			struct Light {
				vec3 ambient;
				vec3 diffuse;
				vec3 specular;
			};
			struct Material {
				sampler2D	diffuse;
				sampler2D	normal;
				vec3		specular;
				float		shininess;
			};

			uniform Light		light;
			uniform Material	material;

			void main()
			{
				// �ӷ�����ͼ���� [0,1]
				vec3	normal		=	texture(material.normal, fs_in.uv).rgb;
						// �����ķ�����0-1��ת���� [-1,1]
						normal		=	normalize(normal * 2.0 - 1.0);  // ���߿ռ��
						//���Ʒ������
						normal.xy	*=	8;
						normal.z	=	sqrt(1 - max(dot(normal.xy, normal.xy), 0.0));

				// ������������ͼ
				vec3	color		=	texture(material.diffuse, fs_in.uv).rgb;

				//������				=	ǿ�ȣ���������ɫ�� * �ƹ���ɫ
				vec3	ambient		=	light.ambient * texture(material.diffuse, fs_in.uv).rgb;

				//������ǿ��			=	dot(�ƹⷽ��, ���߷���)
				//vec3	norm		=	normalize(normal);
				vec3	lightDir	=	normalize(-fs_in.Tangent_lightDir);
				float	diff		=	max(dot(normal, lightDir), 0.0);

				//��������ɫ			=	������ǿ�� * ��������ɫ * �ƹ���ɫ
				vec3	diffuse		=	light.diffuse * diff * texture(material.diffuse, fs_in.uv).rgb;

				//�߹�ǿ��			=	dot(��������, ���߷���)
				vec3	viewDir		=	normalize(fs_in.Tangent_viewPos - fs_in.Tangent_worldPos);
				vec3	reflectDir	=	reflect(-lightDir, normal);
				vec3	halfwayDir	=	normalize(lightDir + viewDir);
				//ȡ����32���ݡ����32�Ǹ߹�ķ����(Shininess) Խ����Խ��£
				float	spec		=	pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

				//�߹���ɫ			=	�߹�ǿ�� * �߹���ͼ * �ƹ���ɫ
				vec3	specular	=	light.specular * spec * material.specular;


				gl_FragColor		=	vec4(ambient + diffuse + specular, 1.0);
			}
		);

		return Shader::initialize();
	}
}