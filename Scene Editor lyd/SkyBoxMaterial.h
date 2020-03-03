#ifndef _Sky_Box_Material_
#define _Sky_Box_Material_

#include "Material.h"

namespace lyd
{
	class SkyBoxMaterial : public Material
	{
	public:
		SkyBoxMaterial();
		virtual ~SkyBoxMaterial();

		virtual bool	begin();

		virtual void	end();

	private:
	public:
		unsigned int _skybox;
	};
}

#endif // !_Sky_Box_Material_


