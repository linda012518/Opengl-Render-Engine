#ifndef _Pbr_Shader_
#define _Pbr_Shader_

#include "Shader.h"

namespace lyd
{
	class PbrShader : public Shader
	{
	public:
		PbrShader();
		virtual ~PbrShader();

	protected:
		virtual bool initialize();
	};
}

#endif // !_Pbr_Shader_

