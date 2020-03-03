#include "RefractShader.h"

namespace lyd
{
	RefractShader::RefractShader()
	{
		initialize();
	}

	RefractShader::~RefractShader()
	{
	}

	bool RefractShader::initialize()
	{
		vs = STR(
			#version 330 core\n

			layout(location = 0) in vec3 inPos;
			layout(location = 1) in vec3 inNormal;

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;

			out vec3 worldPos;
			out vec3 normal;

			void main()
			{
				//���߾��󱻶���Ϊ��ģ�;������Ͻǵ�������ת�þ���
				normal = mat3(transpose(inverse(model))) * inNormal;//���ȱ����ŷ�ֹ���߱���
				worldPos = vec3(model * vec4(inPos, 1.0));
				gl_Position = projection * view * model * vec4(inPos, 1.0);
			}
		);
		fs = STR(
			#version 330 core\n

			uniform samplerCube skybox;
			uniform vec3 viewPos;

			in vec3 worldPos;
			in vec3 normal;

			void main()
			{
				//���� 		������
				//���� 		1.00
				//ˮ 		1.33
				//�� 		1.309
				//���� 		1.52
				//��ʯ 		2.42

				//�ӿ������벣��
				float ratio = 1.00 / 1.52;

				//�۲췽��
				vec3 I = normalize(worldPos - viewPos);
				//ȡ�Է���Ϊ��ķ��䷽��
				vec3 R = refract(I, normalize(normal), ratio);
				//ͨ�����䷽�������������
				gl_FragColor = vec4(texture(skybox, R).rgb, 1.0);
			}
		);

		return Shader::initialize();
	}

}