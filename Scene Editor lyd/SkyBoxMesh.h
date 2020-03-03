#ifndef _Sky_Box_Mesh_
#define _Sky_Box_Mesh_

#include "Mesh.h"

namespace lyd
{
	class SkyBoxMesh : public Mesh
	{
	public:
		SkyBoxMesh();
		virtual ~SkyBoxMesh();

		virtual void initVertex();

	private:
		void addVertex(float x, float y, float z);
	};
}

#endif // !_Sky_Box_Mesh_



