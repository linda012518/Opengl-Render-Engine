#ifndef _Directional_Light_Mesh_
#define _Directional_Light_Mesh_

#include "Mesh.h"

namespace lyd
{
	class DirectionalLightMesh : public Mesh
	{
	public:
		DirectionalLightMesh();
		virtual ~DirectionalLightMesh();

		virtual void initVertex();

		virtual void initIndex();

		virtual void render();

	private:
		void addVertex(float x, float y, float z);
	};
}

#endif // !_Directional_Light_Mesh_


