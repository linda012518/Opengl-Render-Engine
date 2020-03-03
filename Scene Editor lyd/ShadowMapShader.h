#ifndef _Shadow_Map_Shader_
#define _Shadow_Map_Shader_

#include "Shader.h"

namespace lyd
{
	class ShadowMapShader : public Shader
	{
	public:
		ShadowMapShader();
		virtual ~ShadowMapShader();

	protected:
		virtual bool initialize();
	};
}

#endif // !_Shadow_Map_Shader_



