#include "Rectangular2CubemapShader.h"

namespace lyd
{
	Rectangular2CubemapShader::Rectangular2CubemapShader()
	{
		initialize();
	}

	Rectangular2CubemapShader::~Rectangular2CubemapShader() { }

	bool Rectangular2CubemapShader::initialize()
	{
		vs = STR(
			#version 330 core\n

			layout(location = 0) in vec3 inPos;

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;

			out vec3 worldPos;

			void main()
			{
				worldPos	= vec3(model * vec4(inPos, 1.0));
				gl_Position = projection * view * model * vec4(inPos, 1.0);
			}
		);
		fs = STR(
			#version 330 core\n

			uniform sampler2D rectMap;
			in vec3 worldPos;

			const vec2 invAtan = vec2(0.1591, 0.3183);

			void main()
			{
				vec3	v	=	normalize(worldPos);
				//将平铺的图转成立方体贴图
				vec2	uv	=	vec2(atan(v.z, v.x), asin(v.y));
						uv	*=	invAtan;
						uv	+=	0.5;

				gl_FragColor =	vec4(texture(rectMap, uv).rgb, 1.0);
			}
		);

		return Shader::initialize();
	}

}