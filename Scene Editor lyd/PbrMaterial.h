#ifndef _Pbr_Material_
#define _Pbr_Material_

#include "Material.h"

namespace lyd
{
	class PbrMaterial : public Material
	{
	public:
		PbrMaterial();
		virtual ~PbrMaterial();

		void loadTexture(const char* albedo, const char* normal, const char* methallic, const char* roughness, const char* ao);

		void setIrrPreBrdfMap(unsigned int irr, unsigned int pre, unsigned int brdf);

		void setLights(float3 pos[], float3 color[]);

		virtual bool	begin();

	private:
		unsigned int _albedoMap = 0;
		unsigned int _normalMap = 0;
		unsigned int _metallicMap = 0;
		unsigned int _roughnessMap = 0;
		unsigned int _aoMap = 0;

		unsigned int _irradianceMap = 0;
		unsigned int _prefilterMap = 0;
		unsigned int _brdfMap = 0;

	};
}

#endif // !_Pbr_Material_



