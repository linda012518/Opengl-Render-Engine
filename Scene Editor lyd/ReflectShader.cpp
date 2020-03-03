#include "ReflectShader.h"

namespace lyd
{
	ReflectShader::ReflectShader()
	{
		initialize();
	}

	ReflectShader::~ReflectShader()
	{
	}

	bool ReflectShader::initialize()
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
				//法线矩阵被定义为「模型矩阵左上角的逆矩阵的转置矩阵」
				normal		=	mat3(transpose(inverse(model))) * inNormal;//不等比缩放防止法线变形
				worldPos	=	vec3(model * vec4(inPos, 1.0));
				gl_Position =	projection * view * model * vec4(inPos, 1.0);
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
				//观察方向
				vec3 I = normalize(worldPos - viewPos);
				//取以法线为轴的反射方向
				vec3 R = reflect(I, normalize(normal));
				//通过反射方向采样反射纹理
				gl_FragColor = vec4(texture(skybox, R).rgb, 1.0);
			}
		);

		return Shader::initialize();
	}
}