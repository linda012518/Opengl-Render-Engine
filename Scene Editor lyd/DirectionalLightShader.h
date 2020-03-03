#ifndef _Directional_Light_Shader_
#define _Directional_Light_Shader_

#include "Shader.h"

namespace lyd
{
	class DirectionalLightShader : public Shader
	{
	public:
		DirectionalLightShader();
		virtual ~DirectionalLightShader();

	protected:
		virtual bool initialize();
	};
}

#endif // !_Directional_Light_Shader_



