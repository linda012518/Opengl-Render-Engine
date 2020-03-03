#include "TextureMaterial.h"
#include "TextureShader.h"
#include "TextureMgr.h"

namespace lyd
{
	TextureMaterial::TextureMaterial()
	{
		_shader = new TextureShader();
		setColor(float4(1, 1, 1, 1));

		_tex1 = _texMgr->getTexture("map/wall.jpg");
		_tex2 = _texMgr->getTexture("map/toy_box_diffuse.png");

		setTexture();
	}

	TextureMaterial::~TextureMaterial()
	{
	}

	bool TextureMaterial::begin()
	{
		if (Material::begin())
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _tex1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, _tex2);
			return true;
		}
		return false;
	}

	void TextureMaterial::setTexture()
	{
		if (_shader == nullptr) return;

		_shader->begin();

		_shader->setInt("texture1", 0);

		_shader->setInt("texture2", 1);

		_shader->setFloat("mixValue", 0.3f);
	}
}