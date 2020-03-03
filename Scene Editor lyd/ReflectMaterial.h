#ifndef _Reflect_Material_
#define _Reflect_Material_

#include "Material.h"

namespace lyd
{
	class ReflectMaterial : public Material
	{
	public:
		ReflectMaterial();
		virtual ~ReflectMaterial();

		virtual bool	begin();

	private:
	public:
		unsigned int _skybox = 0;
	};
}

#endif // !_Reflect_Material_



