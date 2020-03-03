#include "AxisMaterial.h"
#include "AxisShader.h"

namespace lyd
{
	AxisMaterial::AxisMaterial()
	{
		_shader = new AxisShader();
	}

	AxisMaterial::~AxisMaterial()
	{
	}
}