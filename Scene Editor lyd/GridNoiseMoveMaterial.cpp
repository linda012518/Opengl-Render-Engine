#include "GridNoiseMoveMaterial.h"
#include "GridNoiseMoveShader.h"

namespace lyd
{
	GridNoiseMoveMaterial::GridNoiseMoveMaterial()
	{
		_shader = new GridNoiseMoveShader();
	}

	GridNoiseMoveMaterial::~GridNoiseMoveMaterial()
	{
	}
}