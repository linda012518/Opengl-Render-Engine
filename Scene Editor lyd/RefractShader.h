#ifndef _Refract_Shader_
#define _Refract_Shader_

#include "Shader.h"

namespace lyd
{
	class RefractShader : public Shader
	{
	public:
		RefractShader();
		virtual ~RefractShader();

	protected:
		virtual bool initialize();
	};
}

#endif // !_Refract_Shader_



