#ifndef _Axis_Shader_
#define _Axis_Shader_

#include "Shader.h"

namespace lyd
{
	class AxisShader : public Shader
	{
	public:
		AxisShader();
		virtual ~AxisShader();

	protected:
		virtual bool initialize();
	};
}

#endif // !_Axis_Shader_



