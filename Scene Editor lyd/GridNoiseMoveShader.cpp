#include "GridNoiseMoveShader.h"

namespace lyd
{
	GridNoiseMoveShader::GridNoiseMoveShader()
	{
		initialize();
	}

	GridNoiseMoveShader::~GridNoiseMoveShader()
	{
	}

	bool GridNoiseMoveShader::initialize()
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

			vec2 random2(vec2 p) {
				return fract(sin(vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3))))*43758.5453);
			}

			void main()
			{
				vec2 st = uv;

				vec3 color = vec3(0);

				st *= 3;

				vec2 i_st = floor(st);
				vec2 f_st = fract(st);

				float m_dist = 1.;

				for (int y = -1; y <= 1; y++) {
					for (int x = -1; x <= 1; x++) {
						vec2 neighbor = vec2(float(x), float(y));

						vec2 pos = random2(i_st + neighbor);//计算每一个格里随机点的位置

						pos = 0.5 + 0.5 * sin(time + 6.2831 * pos);

						vec2 diff = neighbor + pos - f_st;

						float dist = length(diff);

						m_dist = min(m_dist, dist);
					}
				}

				color += m_dist;

				color += 1. - step(.02, m_dist);

				//color.r += step(.98, f_st.x) + step(.98, f_st.y);

				//color -= step(.7,abs(sin(27.0*m_dist)))*.5;
				gl_FragColor = vec4(color, 1);

			}
		);

		return Shader::initialize();
	}

}