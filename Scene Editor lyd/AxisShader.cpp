#include "AxisShader.h"

namespace lyd
{
	AxisShader::AxisShader()
	{
		initialize();
	}

	AxisShader::~AxisShader()
	{
	}

	bool AxisShader::initialize()
	{
		vs = STR(
			#version 330 core\n

			layout(location = 0) in vec3 inPos;
			layout(location = 1) in vec3 inNormal;//法线
			layout(location = 2) in vec2 inTexCoords;
			layout(location = 3) in vec3 inTangent;//切线
			layout(location = 4) in vec3 vColor;//颜色

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;

			out vec3 ourColor;

			void main()
			{
						ourColor	= vColor;
				vec4	clip		= projection * view * model * vec4(inPos, 1.0);
						clip.z		= 0.0;
				gl_Position = clip;
			}
		);
		fs = STR(
			#version 330 core\n

			uniform vec4 inColor;
			in vec3 ourColor;

			void main()
			{
				gl_FragColor = vec4(ourColor, 1.0);
			}
		);

		return Shader::initialize();
	}
}