#include "TextureShader.h"

namespace lyd
{
	TextureShader::TextureShader()
	{
		initialize();
	}

	TextureShader::~TextureShader()
	{
	}

	bool TextureShader::initialize()
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

			out vec2 TexCoord;

			void main()
			{
				TexCoord = inTexCoords;
				gl_Position = projection * view * model * vec4(inPos, 1.0);
			}
		);
		fs = STR(
			#version 330 core\n

			uniform vec4 inColor;
			uniform sampler2D texture1;
			uniform sampler2D texture2;
			uniform float mixValue;

			in vec2 TexCoord;

			void main()
			{
				gl_FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue) * inColor;
			}
		);

		return Shader::initialize();
	}

}