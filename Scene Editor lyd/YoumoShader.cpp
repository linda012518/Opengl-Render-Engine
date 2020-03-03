#include "YoumoShader.h"

namespace lyd
{
	YoumoShader::YoumoShader()
	{
		initialize();
	}

	YoumoShader::~YoumoShader()
	{
	}

	bool YoumoShader::initialize()
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
			in vec2 uv;

			vec2 random2(vec2 st) {
				st = vec2(dot(st, vec2(127.1, 311.7)), dot(st, vec2(269.5, 183.3)));
				return -1.0 + 2.0*fract(sin(st)*43758.5453123);
			}

			// Value Noise by Inigo Quilez - iq/2013
			// https://www.shadertoy.com/view/lsf3WH
			float noise(vec2 st) {
				vec2 i = floor(st);
				vec2 f = fract(st);

				vec2 u = smoothstep(0, 1, f);

				return mix(mix(dot(random2(i + vec2(0.0, 0.0)), f - vec2(0.0, 0.0)),
					dot(random2(i + vec2(1.0, 0.0)), f - vec2(1.0, 0.0)), u.x),
					mix(dot(random2(i + vec2(0.0, 1.0)), f - vec2(0.0, 1.0)),
						dot(random2(i + vec2(1.0, 1.0)), f - vec2(1.0, 1.0)), u.x), u.y);
			}

			void main()
			{
				vec2	st		=	uv;

				vec3	color	=	vec3(0, 0, 0);

				float	t		=	1.0;
						t		=	abs(1.0 - sin(time * 0.1)) * 5;
						st		+=	noise(st * 2) * t; 
						color	=	vec3(0.2, 0.4, 0.1) * smoothstep(.18, .2, noise(st)); 
						color	+=	smoothstep(.15, .2, noise(st*10.)); // 黑色的泼溅点
						color	-=	smoothstep(.35, .4, noise(st*10.)); // 泼溅点上的洞

						color	+=	smoothstep(.25, .6, noise(st*5.)); // 黑色的泼溅点
						color	-=	smoothstep(.55, .8, noise(st*5.)); // 泼溅点上的洞


				gl_FragColor	=	vec4(1 - color, 1);
			}
		);

		return Shader::initialize();
	}

}