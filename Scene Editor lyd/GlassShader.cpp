#include "GlassShader.h"

namespace lyd
{
	GlassShader::GlassShader()
	{
		initialize();
	}

	GlassShader::~GlassShader()
	{
	}

	bool GlassShader::initialize()
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
				worldPos = vec3(model * vec4(inPos, 1.0));
				//���߾��󱻶���Ϊ��ģ�;������Ͻǵ�������ת�þ���
				normal = mat3(transpose(inverse(model))) * inNormal;//���ȱ����ŷ�ֹ���߱���
				uv = inTexCoords;
				gl_Position = projection * view * model * vec4(inPos, 1.0);
			}
		);
		fs = STR(
			#version 330 core\n

			struct DirLight {
				vec3 direction;
				vec3 ambient;
				vec3 diffuse;
				vec3 specular;
			};
			struct Material {
				vec3		diffuse;
				vec3		specular;
				float		shininess;
			};

			in vec3 worldPos;
			in vec3 normal;
			in vec2 uv;

			uniform samplerCube skybox;
			uniform vec3		viewPos;
			uniform DirLight	dirlight;
			uniform Material	material;

			vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
			vec3 CalcSpec(DirLight light, vec3 normal, vec3 viewDir);

			void main()
			{
				//�۲췽��
				vec3 I				=	normalize(worldPos - viewPos);
				//ȡ�Է���Ϊ��ķ��䷽��
				vec3 reflectR		=	reflect(I, normalize(normal));
				//ͨ�����䷽�������������
				vec3 reflectColor	=	texture(skybox, reflectR).rgb;

				//�ӿ������벣��
				float ratio			=	1.00 / 1.52;
				//ȡ�Է���Ϊ��ķ��䷽��
				vec3 refractR		=	refract(I, normalize(normal), ratio);
				//ͨ�����䷽�������������
				vec3 refractColor	=	texture(skybox, refractR).rgb;


				vec3 norm			=	normalize(normal);
				vec3 viewDir		=	normalize(viewPos - worldPos);
				vec3 dirColor		=	CalcDirLight(dirlight, norm, viewDir);
				vec3 specular		=	CalcSpec(dirlight, norm, viewDir);

				gl_FragColor		=	vec4(dirColor*0.2 + refractColor*0.3 + reflectColor*0.8 + specular, specular.r + 0.5);
				//gl_FragColor		=	vec4(dirColor, 1);
			}

			vec3 CalcSpec(DirLight light, vec3 normal, vec3 viewDir)
			{
				//������ǿ��			=	dot(�ƹⷽ��, ���߷���)
				vec3	lightDir	=	normalize(-light.direction);

				//�߹�ǿ��			=	dot(��������, ���߷���)
				vec3	reflectDir	=	reflect(-lightDir, normal);
				//ȡ����32���ݡ����32�Ǹ߹�ķ����(Shininess) Խ����Խ��£
				float	spec		=	pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

				//�߹���ɫ			=	�߹�ǿ�� * �߹���ͼ * �ƹ���ɫ
				vec3	specular	=	light.specular * spec * material.specular;
				return	specular;
			}

			vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
			{
				//������				=	ǿ�ȣ���������ɫ�� * �ƹ���ɫ
				vec3	ambient		=	light.ambient * material.diffuse;

				//������ǿ��			=	dot(�ƹⷽ��, ���߷���)
				vec3	lightDir	=	normalize(-light.direction);
				float	diff		=	max(dot(normal, lightDir), 0.0);

				//��������ɫ			=	������ǿ�� * ��������ɫ * �ƹ���ɫ
				vec3	diffuse		=	light.diffuse * diff * material.diffuse;

				//�߹�ǿ��			=	dot(��������, ���߷���)
				vec3	reflectDir	=	reflect(-lightDir, normal);
				//ȡ����32���ݡ����32�Ǹ߹�ķ����(Shininess) Խ����Խ��£
				float	spec		=	pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

				//�߹���ɫ			=	�߹�ǿ�� * �߹���ͼ * �ƹ���ɫ
				vec3	specular	=	light.specular * spec * material.specular;

				return (ambient + diffuse);
			}
		);

		return Shader::initialize();
	}

}