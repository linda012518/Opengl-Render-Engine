#ifndef _Reflect_Shader_
#define _Reflect_Shader_

#include "Shader.h"

namespace lyd
{
	class ReflectShader : public Shader
	{
	public:
		ReflectShader();
		virtual ~ReflectShader();

	protected:
		virtual bool initialize();
	};
}

#endif // !_Reflect_Shader_



