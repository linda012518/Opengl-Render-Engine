#ifndef _Multiple_Light_Shader_
#define _Multiple_Light_Shader_

#include "Shader.h"

namespace lyd
{
	class MultipleLightShader : public Shader
	{
	public:
		MultipleLightShader();
		~MultipleLightShader();

	protected:
		virtual bool initialize();
	};
}

#endif // !_Multiple_Light_Shader_


