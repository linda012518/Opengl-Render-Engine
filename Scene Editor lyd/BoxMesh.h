#ifndef _Box_Mesh_
#define _Box_Mesh_

#include "Mesh.h"

namespace lyd
{
	class BoxMesh : public Mesh
	{
	public:
		BoxMesh();
		virtual ~BoxMesh();

		virtual void initVertex();

	private:
		void addVertex(float x, float y, float z, float nx, float ny, float nz, float u, float v);
	};
}

#endif // !_Box_Mesh_



