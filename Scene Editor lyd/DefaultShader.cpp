#include "DefaultShader.h"
#include "mathf\CELLMath.hpp"

namespace lyd
{
	DefaultShader::DefaultShader()
	{
		initialize();
	}

	DefaultShader::~DefaultShader()
	{
	}

	bool DefaultShader::initialize()
	{
		vs = STR(
			#version 330 core\n

			layout(location = 0) in vec3 inPos;
			layout(location = 1) in vec3 inNormal;//·¨Ïß
			layout(location = 2) in vec2 inTexCoords;
			layout(location = 3) in vec3 inTangent;//ÇÐÏß

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;

			void main()
			{
				gl_Position = projection * view * model * vec4(inPos, 1.0);
			}
		);
		fs = STR(
			#version 330 core\n

			uniform vec4 inColor;

			void main()
			{
				gl_FragColor = inColor;
			}
		);

		return Shader::initialize();
	}

}