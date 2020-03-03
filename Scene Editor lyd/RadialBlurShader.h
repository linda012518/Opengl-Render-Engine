#ifndef _Radial_Blur_Shader_
#define _Radial_Blur_Shader_

#include "Shader.h"

namespace lyd
{
	class RadialBlurShader : public Shader
	{
	public:
		RadialBlurShader();
		virtual ~RadialBlurShader();

	protected:
		virtual bool initialize();
	};
}

#endif // !_Radial_Blur_Shader_


