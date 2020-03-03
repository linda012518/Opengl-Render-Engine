#include "SkyBoxMesh.h"

namespace lyd
{
	SkyBoxMesh::SkyBoxMesh()
	{
		initVertex();
		setupMesh();
	}

	SkyBoxMesh::~SkyBoxMesh()
	{
	}


	void SkyBoxMesh::addVertex(float x, float y, float z)
	{
		Vertex	vertex;
				vertex.Position = float3(x, y, z);
		_vertex.push_back(vertex);
	}

	void SkyBoxMesh::initVertex()
	{
		addVertex(-1.0f, 1.0f, -1.0f);
		addVertex(-1.0f, -1.0f, -1.0f);
		addVertex(1.0f, -1.0f, -1.0f);
		addVertex(1.0f, -1.0f, -1.0f);
		addVertex(1.0f, 1.0f, -1.0f);
		addVertex(-1.0f, 1.0f, -1.0f);

		addVertex(-1.0f, -1.0f, 1.0f);
		addVertex(-1.0f, -1.0f, -1.0f);
		addVertex(-1.0f, 1.0f, -1.0f);
		addVertex(-1.0f, 1.0f, -1.0f);
		addVertex(-1.0f, 1.0f, 1.0f);
		addVertex(-1.0f, -1.0f, 1.0f);

		addVertex(1.0f, -1.0f, -1.0f);
		addVertex(1.0f, -1.0f, 1.0f);
		addVertex(1.0f, 1.0f, 1.0f);
		addVertex(1.0f, 1.0f, 1.0f);
		addVertex(1.0f, 1.0f, -1.0f);
		addVertex(1.0f, -1.0f, -1.0f);

		addVertex(-1.0f, -1.0f, 1.0f);
		addVertex(-1.0f, 1.0f, 1.0f);
		addVertex(1.0f, 1.0f, 1.0f);
		addVertex(1.0f, 1.0f, 1.0f);
		addVertex(1.0f, -1.0f, 1.0f);
		addVertex(-1.0f, -1.0f, 1.0f);

		addVertex(-1.0f, 1.0f, -1.0f);
		addVertex(1.0f, 1.0f, -1.0f);
		addVertex(1.0f, 1.0f, 1.0f);
		addVertex(1.0f, 1.0f, 1.0f);
		addVertex(-1.0f, 1.0f, 1.0f);
		addVertex(-1.0f, 1.0f, -1.0f);

		addVertex(-1.0f, -1.0f, -1.0f);
		addVertex(-1.0f, -1.0f, 1.0f);
		addVertex(1.0f, -1.0f, -1.0f);
		addVertex(1.0f, -1.0f, -1.0f);
		addVertex(-1.0f, -1.0f, 1.0f);
		addVertex(1.0f, -1.0f, 1.0f);
	}
}