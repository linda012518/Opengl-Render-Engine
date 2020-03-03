#ifndef _Normal_Map_Material_
#define _Normal_Map_Material_

#include "Material.h"

namespace lyd
{
	class NormalMapMaterial : public Material
	{
	public:
		NormalMapMaterial();
		virtual ~NormalMapMaterial();

		virtual bool	begin();

		void setLightDir(float3& dir);

	private:
		unsigned int _diffuseMap = 0;
		unsigned int _normalMap = 0;
	};
}

#endif // !_Normal_Map_Material_



