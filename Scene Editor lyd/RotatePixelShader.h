#ifndef _Rotate_Pixel_Shader_
#define _Rotate_Pixel_Shader_

#include "Shader.h"

namespace lyd
{
	class RotatePixelShader : public Shader
	{
	public:
		RotatePixelShader();
		virtual ~RotatePixelShader();

	protected:
		virtual bool initialize();
	};
}

#endif // !_Rotate_Pixel_Shader_



