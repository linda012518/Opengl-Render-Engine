#ifndef _Texture_Material_
#define _Texture_Material_

#include "Material.h"

namespace lyd
{
	class TextureMaterial : public Material
	{
	public:
		TextureMaterial();
		virtual ~TextureMaterial();

		virtual bool	begin();

		void setTexture();

	private:
		unsigned int _tex1, _tex2;
	};
}

#endif // !_Texture_Material_


