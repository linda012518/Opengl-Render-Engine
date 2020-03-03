#include "NormalVisibleShader.h"

namespace lyd
{
	NormalVisibleShader::NormalVisibleShader()
	{
		initialize();
	}

	NormalVisibleShader::~NormalVisibleShader()
	{
	}

	bool NormalVisibleShader::initialize()
	{
		vs = STR(
			#version 330 core\n

			layout(location = 0) in vec3 inPos;
			layout(location = 1) in vec3 inNormal;
			layout(location = 2) in vec2 inTexCoords;

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;

			out vs_Out{
				vec3 normal;
			} vs_out;

			void main()
			{
				//几何着色器接受的位置向量是剪裁空间坐标
				mat3 normalMat	=	mat3(transpose(inverse(view * model)));
				vs_out.normal	=	vec3(projection * vec4(normalMat * inNormal, 0.0));
				gl_Position		=	projection * view * model * vec4(inPos, 1.0);
			}
		);
		gs = STR(
			#version 330 core\n

			layout(triangles) in;
			layout(line_strip, max_vertices = 6) out;

			in vs_Out{
				vec3 normal;
			} gs_in[];

			const float MAGNITUDE = 0.1;

			void GenerateLine(int index)
			{
				gl_Position = gl_in[index].gl_Position;
				EmitVertex();
				gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE;
				EmitVertex();
				EndPrimitive();
			}

			void main() {
				GenerateLine(0);
				GenerateLine(1);
				GenerateLine(2);
			}
		);
		fs = STR(
			#version 330 core\n

			void main()
			{
				gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
			}
		);

		return Shader::initialize();
	}

}