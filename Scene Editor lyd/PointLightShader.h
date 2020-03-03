#ifndef _Point_Light_Shader_
#define _Point_Light_Shader_

#include "Shader.h"

namespace lyd
{
	class PointLightShader : public Shader
	{
	public:
		PointLightShader();
		virtual ~PointLightShader();

	protected:
		virtual bool initialize();
	};
}


#endif // !_Point_Light_Shader_

