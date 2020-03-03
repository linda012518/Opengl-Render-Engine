#ifndef _Text_Shader_
#define _Text_Shader_

#include "Shader.h"

namespace lyd
{
	class TextShader : public Shader
	{
	public:
		TextShader();
		virtual ~TextShader();

	protected:
		virtual bool initialize();
	};
}

#endif // !_Text_Shader_


