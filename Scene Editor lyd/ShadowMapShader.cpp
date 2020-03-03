#include "ShadowMapShader.h"

namespace lyd
{
	ShadowMapShader::ShadowMapShader()
	{
		initialize();
	}

	ShadowMapShader::~ShadowMapShader()
	{
	}

	bool ShadowMapShader::initialize()
	{
		vs = STR(
			#version 330 core\n

			layout(location = 0) in vec3 inPos;

			uniform mat4 model;
			uniform mat4 lightSpace_view;
			uniform mat4 lightSpace_projection;

			void main()
			{
				gl_Position = lightSpace_projection * lightSpace_view * model * vec4(inPos, 1.0);
			}
		);
		fs = STR(
			#version 330 core\n

			void main()
			{
				//gl_FragDepth = gl_FragCoord.z;
			}
		);

		return Shader::initialize();
	}

}