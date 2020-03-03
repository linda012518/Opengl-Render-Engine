#include "GlassShader.h"

namespace lyd
{
	GlassShader::GlassShader()
	{
		initialize();
	}

	GlassShader::~GlassShader()
	{
	}

	bool GlassShader::initialize()
	{
		vs = STR(
			#version 330 core\n

			layout(location = 0) in vec3 inPos;
			layout(location = 1) in vec3 inNormal;
			layout(location = 2) in vec2 inTexCoords;

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;

			out vec3 worldPos;
			out vec3 normal;
			out vec2 uv;

			void main()
			{
				worldPos = vec3(model * vec4(inPos, 1.0));
				//法线矩阵被定义为「模型矩阵左上角的逆矩阵的转置矩阵」
				normal = mat3(transpose(inverse(model))) * inNormal;//不等比缩放防止法线变形
				uv = inTexCoords;
				gl_Position = projection * view * model * vec4(inPos, 1.0);
			}
		);
		fs = STR(
			#version 330 core\n

			struct DirLight {
				vec3 direction;
				vec3 ambient;
				vec3 diffuse;
				vec3 specular;
			};
			struct Material {
				vec3		diffuse;
				vec3		specular;
				float		shininess;
			};

			in vec3 worldPos;
			in vec3 normal;
			in vec2 uv;

			uniform samplerCube skybox;
			uniform vec3		viewPos;
			uniform DirLight	dirlight;
			uniform Material	material;

			vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
			vec3 CalcSpec(DirLight light, vec3 normal, vec3 viewDir);

			void main()
			{
				//观察方向
				vec3 I				=	normalize(worldPos - viewPos);
				//取以法线为轴的反射方向
				vec3 reflectR		=	reflect(I, normalize(normal));
				//通过反射方向采样反射纹理
				vec3 reflectColor	=	texture(skybox, reflectR).rgb;

				//从空气进入玻璃
				float ratio			=	1.00 / 1.52;
				//取以法线为轴的反射方向
				vec3 refractR		=	refract(I, normalize(normal), ratio);
				//通过反射方向采样反射纹理
				vec3 refractColor	=	texture(skybox, refractR).rgb;


				vec3 norm			=	normalize(normal);
				vec3 viewDir		=	normalize(viewPos - worldPos);
				vec3 dirColor		=	CalcDirLight(dirlight, norm, viewDir);
				vec3 specular		=	CalcSpec(dirlight, norm, viewDir);

				gl_FragColor		=	vec4(dirColor*0.2 + refractColor*0.3 + reflectColor*0.8 + specular, specular.r + 0.5);
				//gl_FragColor		=	vec4(dirColor, 1);
			}

			vec3 CalcSpec(DirLight light, vec3 normal, vec3 viewDir)
			{
				//漫反射强度			=	dot(灯光方向, 法线方向)
				vec3	lightDir	=	normalize(-light.direction);

				//高光强度			=	dot(反射向量, 视线方向)
				vec3	reflectDir	=	reflect(-lightDir, normal);
				//取它的32次幂。这个32是高光的反光度(Shininess) 越大光点越聚拢
				float	spec		=	pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

				//高光颜色			=	高光强度 * 高光贴图 * 灯光颜色
				vec3	specular	=	light.specular * spec * material.specular;
				return	specular;
			}

			vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
			{
				//环境光				=	强度（漫反射颜色） * 灯光颜色
				vec3	ambient		=	light.ambient * material.diffuse;

				//漫反射强度			=	dot(灯光方向, 法线方向)
				vec3	lightDir	=	normalize(-light.direction);
				float	diff		=	max(dot(normal, lightDir), 0.0);

				//漫反射颜色			=	漫反射强度 * 漫反射颜色 * 灯光颜色
				vec3	diffuse		=	light.diffuse * diff * material.diffuse;

				//高光强度			=	dot(反射向量, 视线方向)
				vec3	reflectDir	=	reflect(-lightDir, normal);
				//取它的32次幂。这个32是高光的反光度(Shininess) 越大光点越聚拢
				float	spec		=	pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

				//高光颜色			=	高光强度 * 高光贴图 * 灯光颜色
				vec3	specular	=	light.specular * spec * material.specular;

				return (ambient + diffuse);
			}
		);

		return Shader::initialize();
	}

}