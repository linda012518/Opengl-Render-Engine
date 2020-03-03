#ifndef _Convolution_Irradiance_Shader_
#define _Convolution_Irradiance_Shader_

#include "Shader.h"

namespace lyd
{
	class ConvolutionIrradianceShader : public Shader
	{
	public:
		ConvolutionIrradianceShader();
		virtual ~ConvolutionIrradianceShader();

	protected:
		virtual bool initialize();
	};
}

#endif // !_Convolution_Irradiance_Shader_

