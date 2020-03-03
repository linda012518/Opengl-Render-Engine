#include "YoumoMaterial.h"
#include "YoumoShader.h"

namespace lyd
{
	YoumoMaterial::YoumoMaterial()
	{
		_shader = new YoumoShader();
	}

	YoumoMaterial::~YoumoMaterial()
	{
	}
}