#ifndef _Rowbox_Rotate_Material_
#define _Rowbox_Rotate_Material_

#include "Material.h"

namespace lyd
{
	class RowboxRotateMaterial : public Material
	{
	public:
		RowboxRotateMaterial();
		virtual ~RowboxRotateMaterial();

		virtual bool	begin();

		virtual void	end();
	};
}


#endif // !_Rowbox_Rotate_Material_

