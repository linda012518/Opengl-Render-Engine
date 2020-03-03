#ifndef _Normal_Map_Shader_
#define _Normal_Map_Shader_

#include "Shader.h"

namespace lyd
{
	class NormalMapShader : public Shader
	{
	public:
		NormalMapShader();
		virtual ~NormalMapShader();

	protected:
		virtual bool initialize();
	};
}

#endif // !_Normal_Map_Shader_



