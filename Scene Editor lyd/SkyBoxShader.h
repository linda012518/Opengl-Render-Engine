#ifndef _Sky_Box_Shader_
#define _Sky_Box_Shader_

#include "Shader.h"

namespace lyd
{
	class SkyBoxShader : public Shader
	{
	public:
		SkyBoxShader();
		virtual ~SkyBoxShader();

	protected:
		virtual bool initialize();
	};
}

#endif // !_Sky_Box_Shader_


