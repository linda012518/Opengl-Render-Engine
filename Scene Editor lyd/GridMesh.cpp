#include "GridMesh.h"
#include "glew\glew.h"

namespace lyd
{
	GridMesh::GridMesh()
	{
		initVertex();
		setupMesh();
	}

	GridMesh::~GridMesh()
	{
	}

	void GridMesh::render()
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, _vertexSize);
		glBindVertexArray(0);
	}

	void GridMesh::initVertex()
	{
		int size = 30;
		for (int step = -size; step <= size; step++)
		{
			addVertex(step,  size);
			addVertex(step, -size);
		}
		for (int step = -size; step <= size; step++)
		{
			addVertex(-size, step);
			addVertex( size, step);
		}
	}

	void GridMesh::addVertex(float x, float z)
	{
		Vertex	vertex;
				vertex.Position = float3(x, 0, z);
		_vertex.push_back(vertex);
	}
}