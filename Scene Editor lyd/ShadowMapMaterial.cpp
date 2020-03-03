#include "ShadowMapMaterial.h"
#include "ShadowMapShader.h"

namespace lyd
{
	ShadowMapMaterial::ShadowMapMaterial()
	{
		_shader = new ShadowMapShader();

		if (_shader == nullptr) return;

		_shader->begin();
		matrix4 p = ortho(-100.0f, 100.0f, -100.0f, 100.0f, 1.0f, 500.0f);
		_shader->setMat4("lightSpace_projection", p);

		
		glGenFramebuffers(1, &_depthMapFBO);
		// create depth texture
		
		glGenTextures(1, &_depthMap);
		glBindTexture(GL_TEXTURE_2D, _depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _shadowWidth, _shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//超出深度贴图的坐标的地方也会出现阴影，因为阴影贴图是重复的
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//让所有超出深度贴图的坐标的深度范围是1.0，这样超出的坐标将永远不在阴影之中
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		//储存一个边框颜色
		GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthMap, 0);
		//帧缓冲对象必需要有颜色附件
		//显式告诉OpenGL我们不适用任何颜色数据进行渲染
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	ShadowMapMaterial::~ShadowMapMaterial()
	{
	}

	bool ShadowMapMaterial::begin()
	{
		if (Material::begin())
		{
			glViewport(0, 0, _shadowWidth, _shadowHeight);
			glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			return true;
		}
		return false;
	}

	void ShadowMapMaterial::end()
	{
		Material::end();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void ShadowMapMaterial::setLightV(matrix4& v)
	{
		if (_shader == nullptr) return;

		_shader->setMat4("lightSpace_view", v);
	}
}