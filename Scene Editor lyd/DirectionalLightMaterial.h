#ifndef _Directional_Light_Material_
#define _Directional_Light_Material_

#include "Material.h"

namespace lyd
{
	class DirectionalLightMaterial : public Material
	{
	public:
		DirectionalLightMaterial();
		virtual ~DirectionalLightMaterial();

		virtual bool	begin();

		void setLightDir(float3& dir);

	private:
		unsigned int _diffuseMap	= 0;
		unsigned int _specularMap	= 0;
	};
}

#endif // !_Directional_Light_Material_

