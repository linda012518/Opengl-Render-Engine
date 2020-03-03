#ifndef _Normal_Visible_Shader_
#define _Normal_Visible_Shader_

#include "Shader.h"

namespace lyd
{
	class NormalVisibleShader : public Shader
	{
	public:
		NormalVisibleShader();
		virtual ~NormalVisibleShader();

	protected:
		virtual bool initialize();
	};
}

#endif // !_Normal_Visible_Shader_



