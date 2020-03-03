#include "DirectionalLightShader.h"

namespace lyd
{
	DirectionalLightShader::DirectionalLightShader()
	{
		initialize();
	}

	DirectionalLightShader::~DirectionalLightShader() { }

	bool DirectionalLightShader::initialize()
	{
		vs = STR(
			#version 330 core\n

			layout(location = 0) in vec3 inPos;
			layout(location = 1) in vec3 inNormal;
			layout(location = 2) in vec2 inTexCoords;

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;

			out vec3 worldPos;
			out vec3 normal;
			out vec2 uv;

			void main()
			{
				worldPos	=	vec3(model * vec4(inPos, 1.0));
				//���߾��󱻶���Ϊ��ģ�;������Ͻǵ�������ת�þ���
				normal		=	mat3(transpose(inverse(model))) * inNormal;//���ȱ����ŷ�ֹ���߱���
				uv			=	inTexCoords;
				gl_Position =	projection * view * model * vec4(inPos, 1.0);
			}
		);
		fs = STR(
			#version 330 core\n

			struct Light{
				vec3 direction;
				vec3 ambient;
				vec3 diffuse;
				vec3 specular;
			};
			struct Material{
				sampler2D	diffuse;
				sampler2D	specular;
				float		shininess;
			};

			in vec3 worldPos;
			in vec3 normal;
			in vec2 uv;

			uniform vec3		viewPos;
			uniform Light		light;
			uniform Material	material;

			void main()
			{
				//������				=	ǿ�ȣ���������ɫ�� * �ƹ���ɫ
				vec3	ambient		=	light.ambient * texture(material.diffuse, uv).rgb;

				//������ǿ��			=	dot(�ƹⷽ��, ���߷���)
				vec3	norm		=	normalize(normal);
				vec3	lightDir	=	normalize(-light.direction);
				float	diff		=	max(dot(norm, lightDir), 0.0);

				//��������ɫ			=	������ǿ�� * ��������ɫ * �ƹ���ɫ
				vec3	diffuse		=	light.diffuse * diff * texture(material.diffuse, uv).rgb;

				//�߹�ǿ��			=	dot(��������, ���߷���)
				vec3	viewDir		=	normalize(viewPos - worldPos);
				vec3	reflectDir	=	reflect(-lightDir, norm);
				//ȡ����32���ݡ����32�Ǹ߹�ķ����(Shininess) Խ����Խ��£
				float	spec		=	pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

				//�߹���ɫ			=	�߹�ǿ�� * �߹���ͼ * �ƹ���ɫ
				vec3	specular	=	light.specular * spec * texture(material.specular, uv).rgb;
				
				//���
				vec3	result		=	ambient + diffuse + specular;
				gl_FragColor		=	vec4(result, 1.0);
			}
		);

		return Shader::initialize();
	}
}