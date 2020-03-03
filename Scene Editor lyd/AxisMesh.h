#ifndef _Axis_Mesh_
#define _Axis_Mesh_

#include "Mesh.h"

namespace lyd
{
	class AxisMesh : public Mesh
	{
	public:
		AxisMesh();
		virtual ~AxisMesh();

		virtual void initVertex();

		virtual void initIndex();

		virtual void render();

	private:
		void addVertex(float x, float y, float z, float r, float g, float b);
	};
}

#endif // !_Axis_Mesh_



