#ifndef _Explode_Material_
#define _Explode_Material_

#include "Material.h"

namespace lyd
{
	class ExplodeMaterial : public Material
	{
	public:
		ExplodeMaterial();
		~ExplodeMaterial();

		virtual bool	begin();

	private:
		unsigned int	_tex1;
		unsigned long	_currentTime;
	};
}

#endif // !_Explode_Material_



