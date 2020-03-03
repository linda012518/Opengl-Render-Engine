#ifndef _Quad_Mesh_
#define _Quad_Mesh_

#include "Mesh.h"

namespace lyd
{
	class QuadMesh : public Mesh
	{
	public:
		QuadMesh();
		virtual ~QuadMesh();

		virtual void render();

	protected:
		virtual void initVertex();
	};
}

#endif // !_Quad_Mesh_



