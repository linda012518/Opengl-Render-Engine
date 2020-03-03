#ifndef _Rotate_Pixel_Material_
#define _Rotate_Pixel_Material_

#include "Material.h"

namespace lyd
{
	class RotatePixelMaterial : public Material
	{
	public:
		RotatePixelMaterial();
		virtual ~RotatePixelMaterial();

		virtual bool	begin();

		virtual void	end();

	private:
		unsigned int _diffuseMap = 0;
	};
}

#endif // !_Rotate_Pixel_Material_



