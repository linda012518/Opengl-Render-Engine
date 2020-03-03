#include "ExplodeMaterial.h"
#include "ExplodeShader.h"
#include "TextureMgr.h"

#include <Windows.h>

namespace lyd
{
	ExplodeMaterial::ExplodeMaterial()
	{
		_currentTime = GetTickCount();

		_shader = new ExplodeShader();

		if (_shader == nullptr) return;

		_tex1 = _texMgr->getTexture("map/wall.jpg");

	}

	ExplodeMaterial::~ExplodeMaterial()
	{
	}

	bool ExplodeMaterial::begin()
	{
		if (Material::begin())
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _tex1);

			float a =  GetTickCount() - _currentTime;

			_shader->setFloat("time", a / 1000.0f);
			return true;
		}
		return false;
	}
}