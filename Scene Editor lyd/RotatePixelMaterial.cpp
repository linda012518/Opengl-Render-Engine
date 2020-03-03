#include "RotatePixelMaterial.h"
#include "RotatePixelShader.h"
#include "TextureMgr.h"

namespace lyd
{
	RotatePixelMaterial::RotatePixelMaterial()
	{
		_shader = new RotatePixelShader();

		if (_shader == nullptr) return;

		_diffuseMap = _texMgr->getTexture("map/wall.jpg");

		_shader->begin();
		_shader->setInt("tex", 0);
		_shader->setFloat("amount", 26);
	}

	RotatePixelMaterial::~RotatePixelMaterial()
	{
	}

	bool RotatePixelMaterial::begin()
	{
		if (Material::begin())
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _diffuseMap);
			return true;
		}
		return false;
	}

	void RotatePixelMaterial::end()
	{
		Material::end();
		glDisable(GL_BLEND);
	}

}