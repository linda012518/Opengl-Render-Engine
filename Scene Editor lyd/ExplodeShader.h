#ifndef _Explode_Shader_
#define _Explode_Shader_

#include "Shader.h"

namespace lyd
{
	class ExplodeShader : public Shader
	{
	public:
		ExplodeShader();
		virtual ~ExplodeShader();

	protected:
		virtual bool initialize();
	};
}

#endif // !_Explode_Shader_

