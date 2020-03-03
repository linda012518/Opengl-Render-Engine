#include "AxisMesh.h"
#include "glew\glew.h"

namespace lyd
{
	AxisMesh::AxisMesh()
	{
		initVertex();
		initIndex();
		setupMesh();
	}

	AxisMesh::~AxisMesh() { }

	void AxisMesh::render()
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_LINES, _indexSize, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void AxisMesh::initIndex()
	{
		//01 02 03
		//45 56 67 74 89 910 1011 118 1213 1314 1415 1512
		//14 15 16 17 28 29 210 211 312 312 314 315
		unsigned int indices[] = {
			0,1, 0,2, 0,3,
			4,5, 5,6, 6,7, 7,4, 8,9, 9,10, 10,11, 11,8, 12,13, 13,14, 14,15, 15,12,
			1,4, 1,5, 1,6, 1,7, 2,8, 2,9, 2,10, 2,11, 3,12, 3,12, 3,14, 3,15
		};

		_index.clear();
		for (size_t i = 0; i < 54; i++)
		{
			_index.push_back(indices[i]);
		}
	}

	void AxisMesh::initVertex()
	{
		addVertex(0, 0, 0, 1, 1, 1);
		addVertex(2, 0, 0, 1, 0, 0);
		addVertex(0, 2, 0, 0, 1, 0);
		addVertex(0, 0, 2, 0, 0, 1);

		addVertex(1.4, -0.1, -0.1, 1, 0, 0);//x4
		addVertex(1.4, -0.1,  0.1, 1, 0, 0);//x5
		addVertex(1.4,  0.1,  0.1, 1, 0, 0);//x6
		addVertex(1.4,  0.1, -0.1, 1, 0, 0);//x7

		addVertex(-0.1, 1.4, -0.1, 0, 1, 0);//y8
		addVertex(-0.1, 1.4,  0.1, 0, 1, 0);//y9
		addVertex( 0.1, 1.4,  0.1, 0, 1, 0);//y10
		addVertex( 0.1, 1.4, -0.1, 0, 1, 0);//y11

		addVertex(-0.1, -0.1, 1.4, 0, 0, 1);//z12
		addVertex(-0.1,  0.1, 1.4, 0, 0, 1);//z13
		addVertex( 0.1,  0.1, 1.4, 0, 0, 1);//z14
		addVertex( 0.1, -0.1, 1.4, 0, 0, 1);//z15
	}

	void AxisMesh::addVertex(float x, float y, float z, float r, float g, float b)
	{
		Vertex	vertex;
				vertex.Position = float3(x, y, z);
				vertex.Color = float3(r, g, b);
		_vertex.push_back(vertex);
	}
}