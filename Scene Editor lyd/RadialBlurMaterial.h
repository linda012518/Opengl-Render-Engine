#ifndef _Radial_Blur_Material_
#define _Radial_Blur_Material_

#include "Material.h"

namespace lyd
{
	class RadialBlurMaterial : public Material
	{
	public:
		RadialBlurMaterial();
		virtual ~RadialBlurMaterial();

		virtual bool	begin();

	private:
		unsigned int _diffuseMap = 0;
	};
}

#endif // !_Radial_Blur_Material_


