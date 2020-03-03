#ifndef _Sphere_Mesh_
#define _Sphere_Mesh_

#include "Mesh.h"

namespace lyd
{
	class SphereMesh : public Mesh
	{
	public:
		SphereMesh();
		virtual ~SphereMesh();

		virtual void initVertex();

		virtual void render();

	private:
		void convertTBN();
	};

}

#endif // !_Sphere_Mesh_


