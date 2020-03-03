#ifndef _Default_Shader_
#define _Default_Shader_

#include "Shader.h"

namespace lyd
{
	class DefaultShader : public Shader
	{
	public:
		DefaultShader();
		virtual ~DefaultShader();

	protected:
		virtual bool initialize();

	};
}

#endif // !_Default_Shader_



