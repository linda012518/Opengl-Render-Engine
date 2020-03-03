#ifndef _BRDF_Shader_
#define _BRDF_Shader_

#include "Shader.h"

namespace lyd
{
	class BRDFShader : public Shader
	{
	public:
		BRDFShader();
		virtual ~BRDFShader();

	protected:
		virtual bool initialize();
	};
}

#endif // !_BRDF_Shader_


