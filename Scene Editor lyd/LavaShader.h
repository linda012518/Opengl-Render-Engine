#ifndef _Lava_Shader_
#define _Lava_Shader_

#include "Shader.h"

namespace lyd
{
	class LavaShader : public Shader
	{
	public:
		LavaShader();
		virtual ~LavaShader();

	protected:
		virtual bool initialize();
	};
}

#endif // !_Lava_Shader_


