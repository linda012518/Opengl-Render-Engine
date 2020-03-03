#ifndef _Rowbox_Rotate_Shader_
#define _Rowbox_Rotate_Shader_

#include "Shader.h"

namespace lyd
{
	class RowboxRotateShader : public Shader
	{
	public:
		RowboxRotateShader();
		virtual ~RowboxRotateShader();

	protected:
		virtual bool initialize();
	};
}

#endif // !_Rowbox_Rotate_Shader_



