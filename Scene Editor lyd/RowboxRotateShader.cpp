#include "RowboxRotateShader.h"

namespace lyd
{
	RowboxRotateShader::RowboxRotateShader()
	{
		initialize();
	}

	RowboxRotateShader::~RowboxRotateShader()
	{
	}

	bool RowboxRotateShader::initialize()
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

			vec2 rotate2D(vec2 _st, float _angle) {
				_st -=	0.5;
				_st =	mat2(cos(_angle), -sin(_angle), sin(_angle), cos(_angle)) * _st;
				_st +=	0.5;
				return	_st;
			}

			vec2 tile(vec2 _st, float _zoom) {
				_st *= _zoom;
				return fract(_st);
			}

			float box(vec2 _st, vec2 _size, float _smoothEdges) {
				_size		=	vec2(0.5, 0.5) - _size * 0.5;
				vec2	aa	=	vec2(_smoothEdges * 0.5, _smoothEdges * 0.5);
				vec2	uv	=	smoothstep(_size, _size + aa, _st);
						uv	*=	smoothstep(_size, _size + aa, vec2(1, 1) - _st);
				return	uv.x * uv.y;
			}

			void main()
			{
				vec3	color	=	vec3(0, 0, 0);
				vec2	st		=	uv;
						st		=	tile(st, 4.);

						st		=	rotate2D(st, 3.1415926*0.25 + sin(time));

				float	val		=	box(st, vec2(0.7, 0.7), 0.01);

						// Draw a square
						color	=	vec3(val, val, val);
						color	*=	vec3(st, 0.0);

						st		=	fract(st);
						st		-=	0.5;

				float	cal		=	clamp((abs(sin(time)) - length(st)), 0., 1.);
				//cal = smoothstep(0., 0.05, cal);
						cal		=	step(0.1, cal);

				float	alpha1	=	step(0.1, cal);
				float	alpha2	=	step(0.1, val);

				gl_FragColor	=	vec4(color*cal, alpha1 * alpha2);
			}
		);

		return Shader::initialize();
	}

}