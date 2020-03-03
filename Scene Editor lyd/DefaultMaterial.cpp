#include "DefaultMaterial.h"
#include "DefaultShader.h"

namespace lyd
{
	DefaultMaterial::DefaultMaterial()
	{
		_shader = new DefaultShader();
		setColor(float4(1, 1, 1, 1));
	}

	DefaultMaterial::~DefaultMaterial()
	{
	}
}