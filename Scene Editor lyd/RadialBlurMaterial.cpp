#include "RadialBlurMaterial.h"
#include "RadialBlurShader.h"
#include "TextureMgr.h"

namespace lyd
{
	RadialBlurMaterial::RadialBlurMaterial()
	{
		_shader = new RadialBlurShader();

		if (_shader == nullptr) return;

		_diffuseMap = _texMgr->getTexture("map/sakura0.jpg");

		_shader->begin();
		_shader->setInt("_MainTex", 0);

		_shader->setFloat("_CenterX", 0.7);
		_shader->setFloat("_CenterY", 0.2);
		_shader->setFloat("_Samples", 18);
		_shader->setFloat("_BlurStart", 0.6f);
		_shader->setFloat("_BlurWidth", 1);
	}


	RadialBlurMaterial::~RadialBlurMaterial()
	{
	}

	bool RadialBlurMaterial::begin()
	{
		if (Material::begin())
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _diffuseMap);
			return true;
		}
		return false;
	}
}