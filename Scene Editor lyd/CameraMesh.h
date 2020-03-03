#ifndef _Camera_Mesh_
#define _Camera_Mesh_

#include "Mesh.h"
#include "Camera.h"

namespace lyd
{
	class CameraMesh : public Mesh , public Camera
	{
	public:
		CameraMesh();
		virtual ~CameraMesh();

		virtual void initVertex();

		virtual void initIndex();

		virtual void render();

		void setFrustumVertex(float* vertex);

	private:
		void setupFrustumMesh();

		void addVertex(float x, float y, float z);

	private:
		std::vector<float> _vertexFrustum;
		unsigned int cVAO, cVBO, cIBO;

	};
}

#endif // !_Camera_Mesh_


