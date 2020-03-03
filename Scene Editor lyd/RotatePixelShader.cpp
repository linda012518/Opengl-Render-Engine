#include "RotatePixelShader.h"

namespace lyd
{
	RotatePixelShader::RotatePixelShader()
	{
		initialize();
	}

	RotatePixelShader::~RotatePixelShader()
	{
	}

	bool RotatePixelShader::initialize()
	{
		vs = STR(
			#version 330 core\n

			layout(location = 0) in vec3 inPos;
			layout(location = 2) in vec2 inTexCoords;

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;

			out vec2 uv;

			void main()
			{
				uv = inTexCoords;
				gl_Position = projection * view * model * vec4(inPos, 1.0);
			}
		);
		fs = STR(
			#version 330 core\n

			uniform sampler2D tex;
			uniform float amount;
			uniform float time;
			in vec2 uv;

			void main()
			{
				vec2	center	=	vec2(0.5, 0.5);
				vec2	dt		=	uv - center;
				float	len		=	length(dt);
				float	theta	=	len * amount;

				mat2	rot		=	mat2( cos(theta), -sin(theta), sin(theta), cos(theta) );
						dt		=	rot * dt + center;

				float	val		=	clamp((3.0*abs(sin(time / 4)) - length(dt)), 0., 1.);

				vec4	col		=	texture(tex, dt);

				gl_FragColor	=	vec4(col.rgb, val);
			}
		);

		return Shader::initialize();
	}

}