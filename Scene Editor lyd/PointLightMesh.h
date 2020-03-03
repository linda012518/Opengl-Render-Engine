#ifndef _Point_Light_Mesh_
#define _Point_Light_Mesh_

#include "Mesh.h"

namespace lyd
{
	class PointLightMesh : public Mesh
	{
	public:
		PointLightMesh();
		virtual ~PointLightMesh();

		virtual void initVertex();

		virtual void initIndex();

		virtual void render();

	private:
		void addVertex(float x, float y, float z);
	};
}

#endif // !_Point_Light_Mesh_



