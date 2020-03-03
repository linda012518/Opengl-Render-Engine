#ifndef _Glass_Material_
#define _Glass_Material_

#include "Material.h"

namespace lyd
{
	class GlassMaterial : public Material
	{
	public:
		GlassMaterial();
		virtual ~GlassMaterial();

		virtual bool	begin();

		virtual void	end();

		void setLightDir(float3& dir);
		void setEyePos(float3& pos);

	private:
	public:
		unsigned int _skybox		= 0;
	};
}

#endif // !_Glass_Material_



