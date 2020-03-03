#include "GlassMaterial.h"
#include "GlassShader.h"
#include "TextureMgr.h"

namespace lyd
{
	GlassMaterial::GlassMaterial()
	{
		_shader = new GlassShader();

		if (_shader == nullptr) return;

		std::vector<std::string> faces
		{
			"map/skybox/china/right.tga",//右
			"map/skybox/china/left.tga",//左
			"map/skybox/china/top.tga",//上
			"map/skybox/china/bottom.tga",//下
			"map/skybox/china/front.tga",//前
			"map/skybox/china/back.tga"//后
		};

		_skybox = _texMgr->loadCubemap(faces);

		_shader->begin();
		_shader->setInt("skybox", 0);

		_shader->setVec3("material.diffuse", 1.0f, 1.0f, 1.0f);
		_shader->setVec3("material.specular", 1.0f, 1.0f, 1.0f);
		_shader->setFloat("material.shininess", 64);

		_shader->setVec3("dirlight.ambient", 0.4f, 0.4f, 0.4f);
		_shader->setVec3("dirlight.diffuse", 0.8f, 0.8f, 0.8f);
		_shader->setVec3("dirlight.specular", 1.0f, 1.0f, 1.0f);
	}

	GlassMaterial::~GlassMaterial()
	{
	}

	bool GlassMaterial::begin()
	{
		if (Material::begin())
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, _skybox);
			return true;
		}
		return false;
	}

	void GlassMaterial::end()
	{
		Material::end();
		glDisable(GL_BLEND);
	}

	void GlassMaterial::setLightDir(float3& dir)
	{
		if (_shader == nullptr) return;

		_shader->begin();

		_shader->setVec3("dirlight.direction", dir);
	}
	void GlassMaterial::setEyePos(float3& pos)
	{
		if (_shader == nullptr) return;

		_shader->begin();

		_shader->setVec3("viewPos", pos);
	}
}