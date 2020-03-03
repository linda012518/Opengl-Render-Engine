#include "PbrPrepare.h"
#include "glew\glew.h"
#include "mathf\CELLMath.hpp"

#include "Rectangular2CubemapShader.h"
#include "ConvolutionIrradianceShader.h"
#include "PrefilterShader.h"
#include "BRDFShader.h"

#include "SkyBoxMesh.h"
#include "QuadMesh.h"

#include "TextureMgr.h"

namespace lyd
{
	PbrPrepare::PbrPrepare(int inwidth, int inheight)
	{
		unsigned int captureFBO;
		unsigned int captureRBO;
		glGenFramebuffers(1, &captureFBO);
		glGenRenderbuffers(1, &captureRBO);

		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

		// 加载hdr的环境贴图
		TextureMgr tmrg;
		unsigned int hdrTexture = tmrg.loadRectHdrMap("map/hdr/newport_loft.hdr");
		//unsigned int hdrTexture = tmrg.loadRectHdrMap("map/hdr/Ridgecrest_Road_Env.hdr");

		//stbi_set_flip_vertically_on_load(true);
		//int width, height, nrComponents;
		//float *data = stbi_loadf("map/hdr/newport_loft.hdr", &width, &height, &nrComponents, 0);
		//unsigned int hdrTexture;
		//if (data)
		//{
		//	glGenTextures(1, &hdrTexture);
		//	glBindTexture(GL_TEXTURE_2D, hdrTexture);
		//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//	stbi_image_free(data);
		//}
		//else
		//{
		//	printf("Failed to load HDR image.");
		//}

		//设置纹理缓冲		准备把加载hdr的环境贴图渲到立方体贴图
		//unsigned int envCubemap;
		glGenTextures(1, &envCubemap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // enable pre-filter mipmap sampling (combatting visible dots artifact)
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//设置投影和观察矩阵		准备把加载hdr的环境贴图渲到立方体贴图
		matrix4 captureProjection = perspective(90.0f, 1.0f, 0.1f, 10.0f);
		matrix4 model(1);
		matrix4 captureViews[] =
		{
			lookAt(float3(0.0f, 0.0f, 0.0f), float3( 1.0f,  0.0f,  0.0f), float3(0.0f, -1.0f,  0.0f)),
			lookAt(float3(0.0f, 0.0f, 0.0f), float3(-1.0f,  0.0f,  0.0f), float3(0.0f, -1.0f,  0.0f)),
			lookAt(float3(0.0f, 0.0f, 0.0f), float3( 0.0f,  1.0f,  0.0f), float3(0.0f,  0.0f,  1.0f)),
			lookAt(float3(0.0f, 0.0f, 0.0f), float3( 0.0f, -1.0f,  0.0f), float3(0.0f,  0.0f, -1.0f)),
			lookAt(float3(0.0f, 0.0f, 0.0f), float3( 0.0f,  0.0f,  1.0f), float3(0.0f, -1.0f,  0.0f)),
			lookAt(float3(0.0f, 0.0f, 0.0f), float3( 0.0f,  0.0f, -1.0f), float3(0.0f, -1.0f,  0.0f))
		};

		SkyBoxMesh boxMesh;
		Rectangular2CubemapShader equirectangularToCubemapShader;
		equirectangularToCubemapShader.begin();
		equirectangularToCubemapShader.setInt("rectMap", 0);
		equirectangularToCubemapShader.setMat4("projection", captureProjection);
		equirectangularToCubemapShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, hdrTexture);

		glViewport(0, 0, 512, 512);
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		for (unsigned int i = 0; i < 6; ++i)
		{
			equirectangularToCubemapShader.setMat4("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			boxMesh.render();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//生成mipmaps，只有一级
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		//创建一个辐照度立方体贴图，然后重新将捕获的FBO缩放为辐照度比例
		//unsigned int irradianceMap;
		glGenTextures(1, &irradianceMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

		//通过卷积求解扩散积分以创建辐照度（立方体）贴图
		ConvolutionIrradianceShader irradianceShader;
		irradianceShader.begin();
		irradianceShader.setInt("environmentMap", 0);
		irradianceShader.setMat4("projection", captureProjection);
		irradianceShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

		glViewport(0, 0, 32, 32);
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		for (unsigned int i = 0; i < 6; ++i)
		{
			irradianceShader.setMat4("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			boxMesh.render();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//创建一个预过滤器立方体贴图，然后重新缩放捕获FBO以进行预过滤器缩放
		//unsigned int prefilterMap;
		glGenTextures(1, &prefilterMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minifcation filter to mip_linear 
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//生成mipmaps，只有一级，方便opengl自动分配内存
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		//在环境照明上运行准蒙特卡罗模拟，以创建预滤镜（立方体）贴图
		PrefilterShader prefilterShader;
		prefilterShader.begin();
		prefilterShader.setInt("environmentMap", 0);
		prefilterShader.setMat4("projection", captureProjection);
		prefilterShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		unsigned int maxMipLevels = 5;
		for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
		{
			// reisze framebuffer according to mip-level size.
			unsigned int mipWidth = 128 * std::pow(0.5, mip);
			unsigned int mipHeight = 128 * std::pow(0.5, mip);
			glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
			glViewport(0, 0, mipWidth, mipHeight);

			float roughness = (float)mip / (float)(maxMipLevels - 1);
			prefilterShader.setFloat("roughness", roughness);
			for (unsigned int i = 0; i < 6; ++i)
			{
				prefilterShader.setMat4("view", captureViews[i]);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				boxMesh.render();
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//根据所使用的BRDF方程生成2D LUT。
		//unsigned int brdfLUTTexture;
		glGenTextures(1, &brdfLUTTexture);

		// pre-allocate enough memory for the LUT texture.
		glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
		// be sure to set wrapping mode to GL_CLAMP_TO_EDGE
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

		glViewport(0, 0, 512, 512);
		BRDFShader brdfShader;
		QuadMesh quadMesh;
		brdfShader.begin();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		quadMesh.render();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, inwidth, inheight);
	}

	unsigned int PbrPrepare::getIrradianceMap()
	{
		return irradianceMap;
	}
	unsigned int PbrPrepare::getPrefilterMap()
	{
		return prefilterMap;
	}
	unsigned int PbrPrepare::getBrdfMap()
	{
		return brdfLUTTexture;
	}

	unsigned int PbrPrepare::getSkybox()
	{
		return envCubemap;
	}

	PbrPrepare::~PbrPrepare()
	{
	}
}