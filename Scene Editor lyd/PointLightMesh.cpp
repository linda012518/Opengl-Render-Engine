#include "PointLightMesh.h"
#include "glew\glew.h"

namespace lyd
{
	PointLightMesh::PointLightMesh()
	{
		initVertex();
		initIndex();
		setupMesh();
	}

	PointLightMesh::~PointLightMesh()
	{
	}

	void PointLightMesh::render()
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_LINES, _indexSize, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void PointLightMesh::initIndex()
	{
		unsigned int indices[] = { 0,1, 1,2, 2,3, 3,0,		0,4, 1,4, 2,4, 3,4,		0,5, 1,5, 2,5, 3,5 };

		_index.clear();
		for (size_t i = 0; i < 24; i++)
		{
			_index.push_back(indices[i]);
		}
	}

	void PointLightMesh::initVertex()
	{
		addVertex(-0.5f, 0,  0);
		addVertex(    0, 0,  0.5f);
		addVertex( 0.5f, 0,  0);
		addVertex(    0, 0, -0.5f);

		addVertex(0,  0.5f, 0);
		addVertex(0, -0.5f, 0);
	}

	void PointLightMesh::addVertex(float x, float y, float z)
	{
		Vertex	vertex;
				vertex.Position = float3(x, y, z);
		_vertex.push_back(vertex);
	}
}