#ifndef _Plane_Mesh_
#define _Plane_Mesh_

#include "Mesh.h"

namespace lyd
{
	class PlaneMesh : public Mesh
	{
	public:
		PlaneMesh();
		virtual ~PlaneMesh();

		virtual void initVertex();

		virtual void render();

	private:
		void addVertex(float x, float y, float z, float nx, float ny, float nz, float u, float v);
	};
}

#endif // !_Plane_Mesh_



