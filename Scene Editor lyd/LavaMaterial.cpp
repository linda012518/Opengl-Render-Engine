#include "LavaMaterial.h"
#include "LavaShader.h"

namespace lyd
{
	LavaMaterial::LavaMaterial()
	{
		_shader = new LavaShader();
	}

	LavaMaterial::~LavaMaterial()
	{
	}
}