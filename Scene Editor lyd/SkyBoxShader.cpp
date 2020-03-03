#include "SkyBoxShader.h"

namespace lyd
{
	SkyBoxShader::SkyBoxShader()
	{
		initialize();
	}

	SkyBoxShader::~SkyBoxShader() { }

	bool SkyBoxShader::initialize()
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
						worldPos	=	inPos;
				mat4	removeTrans	=	mat4(mat3(view));
				vec4	clipPos		=	projection * removeTrans * vec4(worldPos, 1.0);
				gl_Position = clipPos.xyww;
			}
		);
		fs = STR(
			#version 330 core\n

			uniform samplerCube skybox;

			in vec3 worldPos;

			void main()
			{
				vec3 envColor = texture(skybox, worldPos).rgb;

				//hdr色调映射 
				envColor = envColor / (envColor + vec3(1.0));
				//gamma校正
				envColor = pow(envColor, vec3(1.0 / 2.2));

				gl_FragColor = texture(skybox, worldPos);
				gl_FragColor = vec4(envColor, 1.0);
			}
		);

		return Shader::initialize();
	}
}