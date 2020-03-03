#include "RowboxRotateMaterial.h"
#include "RowboxRotateShader.h"

namespace lyd
{
	RowboxRotateMaterial::RowboxRotateMaterial()
	{
		_shader = new RowboxRotateShader();
	}

	RowboxRotateMaterial::~RowboxRotateMaterial()
	{
	}

	bool RowboxRotateMaterial::begin()
	{
		if (Material::begin())
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			return true;
		}
		return false;
	}

	void RowboxRotateMaterial::end()
	{
		Material::end();
		glDisable(GL_BLEND);
	}
}