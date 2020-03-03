#ifndef _Shadow_Map_Material_
#define _Shadow_Map_Material_

#include "Material.h"

namespace lyd
{
	class ShadowMapMaterial : public Material
	{
	public:
		ShadowMapMaterial();
		virtual ~ShadowMapMaterial();

		virtual bool	begin();

		virtual void	end();

		void setLightV(matrix4& v);

	public:
		unsigned int _depthMap;
	private:
		unsigned int _depthMapFBO;
		unsigned int _shadowWidth = 1024, _shadowHeight = 1024;

	};
}

#endif // !_Shadow_Map_Material_



