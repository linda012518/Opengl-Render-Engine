#include "ExplodeShader.h"

namespace lyd
{
	ExplodeShader::ExplodeShader()
	{
		initialize();
	}

	ExplodeShader::~ExplodeShader() { }

	bool ExplodeShader::initialize()
	{
		vs = STR(
			#version 330 core\n

			layout(location = 0) in vec3 inPos;
			layout(location = 1) in vec3 inNormal;
			layout(location = 2) in vec2 inTexCoords;

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;

			out vs_Out {
				vec2 uv;
				vec3 normal;
			} vs_out;

			void main()
			{
				//法线矩阵被定义为「模型矩阵左上角的逆矩阵的转置矩阵」
				mat3 normalMat	=	mat3(transpose(inverse(view * model)));//不等比缩放防止法线变形
				//几何着色器接受的位置向量是剪裁空间坐标
				vs_out.normal	=	vec3(projection * vec4(normalMat * inNormal, 0.0));
				vs_out.uv		=	inTexCoords;
				gl_Position		=	projection * view * model * vec4(inPos, 1.0);
			}
		);
		gs = STR(
			#version 330 core\n

			layout(triangles) in;
			layout(triangle_strip, max_vertices = 3) out;

			in vs_Out{
				vec2 uv;
				vec3 normal;
			} gs_in[];

			out vec2 uv;

			uniform float time;

			vec4 explode(vec4 position, vec3 normal)
			{
				float	magnitude	=	2.0;

				vec3	direction	= normal * ((sin(time) + 1.0) / 2.0 + 0.5 * 10 * pow(sin(time), 2));
				return	position + vec4(direction, 0.0);
			}

			vec3 GetNormal()
			{
				vec3	a = vec3(gl_in[1].gl_Position) - vec3(gl_in[0].gl_Position);
				vec3	b = vec3(gl_in[2].gl_Position) - vec3(gl_in[0].gl_Position);
				return	normalize(cross(a, b));
			}

			void main() {
				vec3	normal		=	GetNormal();

						gl_Position =	explode(gl_in[0].gl_Position, normal);
						uv			=	gs_in[0].uv;
				EmitVertex();
						gl_Position =	explode(gl_in[1].gl_Position, normal);
						uv			=	gs_in[1].uv;
				EmitVertex();
						gl_Position =	explode(gl_in[2].gl_Position, normal);
						uv			=	gs_in[2].uv;
				EmitVertex();

				EndPrimitive();
			}
		);
		fs = STR(
			#version 330 core\n

			uniform sampler2D tex1;

			in vec2 uv;

			void main()
			{
				gl_FragColor = texture(tex1, uv);
			}
		);

		return Shader::initialize();
	}

}