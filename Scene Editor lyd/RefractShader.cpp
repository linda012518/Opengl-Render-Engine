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
				//法线矩阵被定义为「模型矩阵左上角的逆矩阵的转置矩阵」
				normal = mat3(transpose(inverse(model))) * inNormal;//不等比缩放防止法线变形
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
				//材质 		折射率
				//空气 		1.00
				//水 		1.33
				//冰 		1.309
				//玻璃 		1.52
				//钻石 		2.42

				//从空气进入玻璃
				float ratio = 1.00 / 1.52;

				//观察方向
				vec3 I = normalize(worldPos - viewPos);
				//取以法线为轴的反射方向
				vec3 R = refract(I, normalize(normal), ratio);
				//通过反射方向采样反射纹理
				gl_FragColor = vec4(texture(skybox, R).rgb, 1.0);
			}
		);

		return Shader::initialize();
	}

}