#ifndef _Point_Light_Material_
#define _Point_Light_Material_

#include "Material.h"

namespace lyd
{
	class PointLightMaterial : public Material
	{
	public:
		PointLightMaterial();
		virtual ~PointLightMaterial();

		virtual bool	begin();

		void setLightPos(float3& pos);

	private:
		unsigned int _diffuseMap	= 0;
		unsigned int _specularMap	= 0;
	};
}

#endif // !_Point_Light_Material_


