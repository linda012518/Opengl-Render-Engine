#include "NormalMapShader.h"


namespace lyd
{
	NormalMapShader::NormalMapShader()
	{
		initialize();
	}

	NormalMapShader::~NormalMapShader()
	{
	}

	bool NormalMapShader::initialize()
	{
		vs = STR(
			#version 330 core\n

			layout(location = 0) in vec3 inPos;
			layout(location = 1) in vec3 inNormal;//法线
			layout(location = 2) in vec2 inTexCoords;
			layout(location = 3) in vec3 inTangent;//切线
			layout(location = 5) in vec3 inBitangent;//副切线

			out VS_OUT{
				vec3 worldPos;
				vec2 uv;
				vec3 Tangent_lightDir;
				vec3 Tangent_viewPos;
				vec3 Tangent_worldPos;
			} vs_out;

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;

			uniform vec3 lightDir;
			uniform vec3 viewPos;

			void main()
			{
				vs_out.worldPos		=	vec3(model * vec4(inPos, 1.0));
				vs_out.uv			=	inTexCoords;

				//当法向贴图应用到这些表面时将切线向量平均化通常能获得更好更平滑的结果。
				//这样做有个问题，就是TBN向量可能会不能互相垂直，这意味着TBN矩阵不再是正交矩阵了
				vec3	T	=	normalize(mat3(model) * inTangent);
				vec3	N	=	normalize(mat3(model) * inNormal);
						T	=	normalize(T - dot(T, N) * N);
				vec3	B	=	cross(N, T);

				//正交矩阵的转置等于逆矩阵		将所有相关的世界空间向量转变到采样所得法线向量的空间
				mat3	TBN	=	transpose(mat3(T, B, N));
				vs_out.Tangent_lightDir =	TBN * lightDir;
				vs_out.Tangent_viewPos	=	TBN * viewPos;
				vs_out.Tangent_worldPos =	TBN * vs_out.worldPos;

				gl_Position				=	projection * view * model * vec4(inPos, 1.0);
			}
		);
		fs = STR(
			#version 330 core\n

			in VS_OUT{
				vec3 worldPos;
				vec2 uv;
				vec3 Tangent_lightDir;
				vec3 Tangent_viewPos;
				vec3 Tangent_worldPos;
			} fs_in;

			struct Light {
				vec3 ambient;
				vec3 diffuse;
				vec3 specular;
			};
			struct Material {
				sampler2D	diffuse;
				sampler2D	normal;
				vec3		specular;
				float		shininess;
			};

			uniform Light		light;
			uniform Material	material;

			void main()
			{
				// 从法线贴图采样 [0,1]
				vec3	normal		=	texture(material.normal, fs_in.uv).rgb;
						// 采样的法线是0-1，转换到 [-1,1]
						normal		=	normalize(normal * 2.0 - 1.0);  // 切线空间的
						//控制法线深度
						normal.xy	*=	8;
						normal.z	=	sqrt(1 - max(dot(normal.xy, normal.xy), 0.0));

				// 采样漫反射贴图
				vec3	color		=	texture(material.diffuse, fs_in.uv).rgb;

				//环境光				=	强度（漫反射颜色） * 灯光颜色
				vec3	ambient		=	light.ambient * texture(material.diffuse, fs_in.uv).rgb;

				//漫反射强度			=	dot(灯光方向, 法线方向)
				//vec3	norm		=	normalize(normal);
				vec3	lightDir	=	normalize(-fs_in.Tangent_lightDir);
				float	diff		=	max(dot(normal, lightDir), 0.0);

				//漫反射颜色			=	漫反射强度 * 漫反射颜色 * 灯光颜色
				vec3	diffuse		=	light.diffuse * diff * texture(material.diffuse, fs_in.uv).rgb;

				//高光强度			=	dot(反射向量, 视线方向)
				vec3	viewDir		=	normalize(fs_in.Tangent_viewPos - fs_in.Tangent_worldPos);
				vec3	reflectDir	=	reflect(-lightDir, normal);
				vec3	halfwayDir	=	normalize(lightDir + viewDir);
				//取它的32次幂。这个32是高光的反光度(Shininess) 越大光点越聚拢
				float	spec		=	pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

				//高光颜色			=	高光强度 * 高光贴图 * 灯光颜色
				vec3	specular	=	light.specular * spec * material.specular;


				gl_FragColor		=	vec4(ambient + diffuse + specular, 1.0);
			}
		);

		return Shader::initialize();
	}
}