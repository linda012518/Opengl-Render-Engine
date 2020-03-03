#ifndef _Youmo_Shader_
#define _Youmo_Shader_

#include "Shader.h"

namespace lyd
{
	class YoumoShader : public Shader
	{
	public:
		YoumoShader();
		virtual ~YoumoShader();

	protected:
		virtual bool initialize();
	};
}

#endif // !_Youmo_Shader_


