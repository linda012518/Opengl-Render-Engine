#include "TextShader.h"

namespace lyd
{
	TextShader::TextShader()
	{
		initialize();
	}

	TextShader::~TextShader() { }

	bool TextShader::initialize()
	{
		vs = STR(
			#version 330 core\n

			layout(location = 0) in vec3 inPos;
			layout(location = 1) in vec2 inUV;
			out vec2 TexCoords;

			uniform mat4 projection;

			void main()
			{
				gl_Position = projection * vec4(inPos, 1.0);
				TexCoords = inUV;
			}
		);
		fs = STR(
			#version 330 core\n

			in vec2 TexCoords;
			out vec4 color;

			uniform sampler2D text;
			uniform vec3 textColor;

			void main()
			{
				vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
				color = sampled * vec4(textColor, 1.0);
			}
		);

		return Shader::initialize();
	}

}