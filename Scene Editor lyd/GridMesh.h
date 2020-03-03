#ifndef _Grid_Mesh_
#define _Grid_Mesh_

#include "Mesh.h"

namespace lyd
{
	class GridMesh : public Mesh
	{
	public:
		GridMesh();
		virtual ~GridMesh();

		virtual void initVertex();

		virtual void render();

	private:
		void addVertex(float x, float z);
	};
}

#endif // !_Grid_Mesh_



