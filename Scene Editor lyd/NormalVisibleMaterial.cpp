#include "NormalVisibleMaterial.h"
#include "NormalVisibleShader.h"

namespace lyd
{
	NormalVisibleMaterial::NormalVisibleMaterial()
	{
		_shader = new NormalVisibleShader();
	}

	NormalVisibleMaterial::~NormalVisibleMaterial()
	{
	}
}