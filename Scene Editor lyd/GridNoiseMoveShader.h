#ifndef _Grid_Noise_Move_Shader_
#define _Grid_Noise_Move_Shader_

#include "Shader.h"

namespace lyd
{
	class GridNoiseMoveShader : public Shader
	{
	public:
		GridNoiseMoveShader();
		virtual ~GridNoiseMoveShader();

	protected:
		virtual bool initialize();
	};
}


#endif // !_Grid_Noise_Move_Shader_

