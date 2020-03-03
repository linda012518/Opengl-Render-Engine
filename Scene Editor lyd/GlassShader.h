#ifndef _Glass_Shader_
#define _Glass_Shader_

#include "Shader.h"

namespace lyd
{
	class GlassShader : public Shader
	{
	public:
		GlassShader();
		virtual ~GlassShader();

	protected:
		virtual bool initialize();
	};
}

#endif // !_Glass_Shader_



