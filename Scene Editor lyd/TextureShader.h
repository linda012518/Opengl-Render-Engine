#ifndef _Texture_Shader_
#define _Texture_Shader_

#include "Shader.h"

namespace lyd
{
	class TextureShader : public Shader
	{
	public:
		TextureShader();
		virtual ~TextureShader();

	protected:
		virtual bool initialize();
	};
}

#endif // !_Texture_Shader_



