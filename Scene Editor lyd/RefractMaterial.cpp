#include "RefractMaterial.h"
#include "RefractShader.h"
#include "TextureMgr.h"

namespace lyd
{
	RefractMaterial::RefractMaterial()
	{
		_shader = new RefractShader();

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

		if (_shader == nullptr) return;
		_shader->begin();
		_shader->setInt("skybox", 0);
	}

	RefractMaterial::~RefractMaterial()
	{
	}

	bool RefractMaterial::begin()
	{
		if (Material::begin())
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, _skybox);
			return true;
		}
		return false;
	}
}