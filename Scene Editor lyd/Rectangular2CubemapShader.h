#ifndef _Rectangular_2_Cubemap_
#define _Rectangular_2_Cubemap_

#include "Shader.h"

namespace lyd
{
	class Rectangular2CubemapShader : public Shader
	{
	public:
		Rectangular2CubemapShader();
		virtual ~Rectangular2CubemapShader();

	protected:
		virtual bool initialize();
	};
}

#endif // !_Rectangular_2_Cubemap_



