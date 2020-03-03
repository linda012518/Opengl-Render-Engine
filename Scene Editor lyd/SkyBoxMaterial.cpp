#include "SkyBoxMaterial.h"
#include "SkyBoxShader.h"
#include "TextureMgr.h"

namespace lyd
{
	SkyBoxMaterial::SkyBoxMaterial()
	{
		_shader = new SkyBoxShader();

		std::vector<std::string> faces
		{
			"map/skybox/china/right.tga",//��
			"map/skybox/china/left.tga",//��
			"map/skybox/china/top.tga",//��
			"map/skybox/china/bottom.tga",//��
			"map/skybox/china/front.tga",//ǰ
			"map/skybox/china/back.tga"//��
		};

		_skybox = _texMgr->loadCubemap(faces);

		if (_shader == nullptr) return;
		_shader->begin();
		_shader->setInt("skybox", 0);
	}

	SkyBoxMaterial::~SkyBoxMaterial()
	{
	}

	bool SkyBoxMaterial::begin()
	{
		glDepthFunc(GL_LEQUAL);
		if (Material::begin())
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, _skybox);
			return true;
		}
		return false;
	}

	void SkyBoxMaterial::end()
	{
		Material::end();
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glDepthFunc(GL_LESS);
	}
}