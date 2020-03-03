#ifndef _Refract_Material_
#define _Refract_Material_

#include "Material.h"

namespace lyd
{
	class RefractMaterial : public Material
	{
	public:
		RefractMaterial();
		virtual ~RefractMaterial();

		virtual bool	begin();

	private:
	public:
		unsigned int _skybox = 0;
	};
}

#endif // !_Refract_Material_



