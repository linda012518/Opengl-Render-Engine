#include "DirectionalLightMesh.h"
#include "glew\glew.h"

namespace lyd
{
	DirectionalLightMesh::DirectionalLightMesh()
	{
		initVertex();
		initIndex();
		setupMesh();
	}

	DirectionalLightMesh::~DirectionalLightMesh()
	{
	}

	void DirectionalLightMesh::render()
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_LINES, _indexSize, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void DirectionalLightMesh::initIndex()
	{
		//01 23 45 67 89 1011 1213 1415 1617 1819 2021 2223 2425 2627 2829 3031
		//02 24 46 68 810 1012 1214 1416 1618 1820 2022 2224 2426 2628 2830
		//13 35 57 79 911 1113 1315 1517 1719 1921 2123 2325 2527 2729 2931
		//3233 3334 3435 3532 3236 3336 3436 3536

		unsigned int indices[] = { 
			0,1, 2,3, 4,5, 6,7, 8,9, 10,11, 12,13, 14,15, 16,17, 18,19, 20,21, 22,23, 24,25, 26,27, 28,29, 30,31,
			0,2, 2,4, 4,6, 6,8, 8,10, 10,12, 12,14, 14,16, 16,18, 18,20, 20,22, 22,24, 24,26, 26,28, 28,30, 30, 0,
			1,3, 3,5, 5,7, 7,9, 9,11, 11,13, 13,15, 15,17, 17,19, 19,21, 21,23, 23,25, 25,27, 27,29, 29,31, 31, 1,
			32,33, 33,34, 34,35, 35,32, 32,36, 33,36, 34,36, 35,36
		};

		_index.clear();
		for (size_t i = 0; i < 112; i++)
		{
			_index.push_back(indices[i]);
		}
	}

	void DirectionalLightMesh::initVertex()
	{
		for (int i = 0; i < 16; ++i)
		{
			Vertex	vertex1;
					vertex1.Position.x = (float)cos((double)(i)*(360.0f / 16.0f)*PI / 180) * 0.5f;
					vertex1.Position.y = 0;
					vertex1.Position.z = (float)sin((double)(i)*(360.0f / 16.0f)*PI / 180) * 0.5f;
			_vertex.push_back(vertex1);

			Vertex	vertex2;
					vertex2.Position.x = (float)cos((double)(i)*(360.0f / 16.0f)*PI / 180) * 0.5f;
					vertex2.Position.y = -1.5f;
					vertex2.Position.z = (float)sin((double)(i)*(360.0f / 16.0f)*PI / 180) * 0.5f;
			_vertex.push_back(vertex2);
		}

		addVertex(-0.1f, 0, -0.1f);
		addVertex(-0.1f, 0,  0.1f);
		addVertex( 0.1f, 0,  0.1f);
		addVertex( 0.1f, 0, -0.1f);
		addVertex( 0, -0.2, 0);
	}

	void DirectionalLightMesh::addVertex(float x, float y, float z)
	{
		Vertex	vertex;
				vertex.Position = float3(x, y, z);
		_vertex.push_back(vertex);
	}

}