#include "LavaShader.h"

namespace lyd
{
	LavaShader::LavaShader()
	{
		initialize();
	}

	LavaShader::~LavaShader()
	{
	}

	bool LavaShader::initialize()
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

			vec3 mod289(vec3 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
			vec2 mod289(vec2 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
			vec3 permute(vec3 x) { return mod289(((x*34.0) + 1.0)*x); }

			float snoise(vec2 v) {
				const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
					0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
					-0.577350269189626,  // -1.0 + 2.0 * C.x
					0.024390243902439); // 1.0 / 41.0
				vec2 i = floor(v + dot(v, C.yy));
				vec2 x0 = v - i + dot(i, C.xx);
				vec2 i1;
				i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
				vec4 x12 = x0.xyxy + C.xxzz;
				x12.xy -= i1;
				i = mod289(i); // Avoid truncation effects in permutation
				vec3 p = permute(permute(i.y + vec3(0.0, i1.y, 1.0))
					+ i.x + vec3(0.0, i1.x, 1.0));

				vec3 m = max(0.5 - vec3(dot(x0, x0), dot(x12.xy, x12.xy), dot(x12.zw, x12.zw)), 0.0);
				m = m*m;
				m = m*m;
				vec3 x = 2.0 * fract(p * C.www) - 1.0;
				vec3 h = abs(x) - vec3(0.5);
				vec3 ox = floor(x + 0.5);
				vec3 a0 = x - ox;
				m *= 1.79284291400159 - 0.85373472095314 * (a0*a0 + h*h);
				vec3 g;
				g.x = a0.x  * x0.x + h.x  * x0.y;
				g.yz = a0.yz * x12.xz + h.yz * x12.yw;
				return 130.0 * dot(m, g);
			}

			void main()
			{
				vec2 st = uv;

				vec3 color = vec3(0);
				vec2 pos = vec2(st*3.0);

				float DF = 0.0;

				float a = 0.0;
				vec2 vel = vec2(time*0.1, time*0.1);
				DF += snoise(pos + vel)*.25 + .25;

				a = snoise(pos*vec2(cos(time*0.15), sin(time*0.1))*0.1)*3.1415;
				vel = vec2(cos(a), sin(a));
				DF += snoise(pos + vel)*0.25 + 0.25;

				color = vec3(smoothstep(0.7, 0.75, fract(DF)));

				gl_FragColor = vec4(1 - color, 1);
			}
		);

		return Shader::initialize();
	}

}