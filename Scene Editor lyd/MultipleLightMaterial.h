#ifndef _Multiple_Light_Material_
#define _Multiple_Light_Material_

#include "Material.h"

namespace lyd
{
	class MultipleLightMaterial : public Material
	{
	public:
		MultipleLightMaterial();
		virtual ~MultipleLightMaterial();

		virtual bool	begin();

		void setLightDir(float3& dir);
		void setLightPos(float3& pos);

	private:
		unsigned int _diffuseMap	= 0;
		unsigned int _specularMap	= 0;
	};
}

#endif // !_Multiple_Light_Material_



