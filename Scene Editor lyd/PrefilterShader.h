#ifndef _Prefilter_Shader_
#define _Prefilter_Shader_

#include "Shader.h"

namespace lyd
{
	class PrefilterShader : public Shader
	{
	public:
		PrefilterShader();
		virtual ~PrefilterShader();

	protected:
		virtual bool initialize();
	};
}

#endif // !_Prefilter_Shader_


