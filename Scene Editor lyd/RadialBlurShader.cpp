#include "RadialBlurShader.h"

namespace lyd
{
	RadialBlurShader::RadialBlurShader()
	{
		initialize();
	}

	RadialBlurShader::~RadialBlurShader()
	{
	}

	bool RadialBlurShader::initialize()
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

			uniform float time;
			uniform sampler2D	_MainTex;
			uniform float		_CenterX;
			uniform float		_CenterY;
			uniform float		_Samples;
			uniform float		_BlurStart;
			uniform float		_BlurWidth;
			in vec2 uv;

			void main()
			{
				vec2	st			=	uv;

				vec2	center		=	vec2(_CenterX, _CenterY);
						center.x	=	clamp(sin(time / 4.0), 0.0, 1.0);
						center.y	=	clamp(cos(time / 4.0), 0.0, 1.0);
						st			-=	center;
				float	precompute	=	_BlurWidth * (1.0 / float(_Samples - 1));

				vec4	color		=	vec4(0, 0, 0, 0);
				for (int i = 0; i < _Samples; i++)
				{
					float	scale	=	_BlurStart + (float(i)* precompute);
							color	+=	texture(_MainTex, st * scale + center);
				}

				color /= float(_Samples);

				gl_FragColor = vec4(color.rgb, 1);
			}
		);

		return Shader::initialize();
	}

}