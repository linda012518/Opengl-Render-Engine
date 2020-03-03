#include "ConvolutionIrradianceShader.h"

namespace lyd
{
	ConvolutionIrradianceShader::ConvolutionIrradianceShader()
	{
		initialize();
	}

	ConvolutionIrradianceShader::~ConvolutionIrradianceShader() { }

	bool ConvolutionIrradianceShader::initialize()
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

			uniform samplerCube environmentMap;
			in vec3 worldPos;

			const float PI = 3.14159265359;

			void main()
			{
				//�������м���������Ļ���
				//������֣�������Ȼ���ܽ���һ������Ի�����ͼ��������Ҫ�Ӱ��� �� �����п��ܵķ�����в���
				//����ĳ�����������ǲ�׽���նȵ�λ�ã�����Ԥ�ȼ���ÿ�����ܵİ�����ķ��ն�
				//��ģ���㷨���ƣ�ȡ��Χһ������������ƽ��ֵ
				vec3	N			=	normalize(worldPos);

				vec3	irradiance	=	vec3(0.0);

				// tangent space calculation from origin point
				vec3	up			=	vec3(0.0, 1.0, 0.0);
				vec3	right		=	cross(up, N);
						up			=	cross(N, right);

				float	sampleDelta =	0.025;
				float	nrSamples	=	0.0f;
				for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
				{
					for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
					{
						// spherical to cartesian (in tangent space)
						vec3	tangentSample	=	vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
						// tangent space to world
						vec3	sampleVec		=	tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;

								irradiance		+=	texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta);
								nrSamples++;
					}
				}
				irradiance		=	PI * irradiance * (1.0 / float(nrSamples));

				gl_FragColor	=	vec4(irradiance, 1.0);
			}
		);

		return Shader::initialize();
	}

}