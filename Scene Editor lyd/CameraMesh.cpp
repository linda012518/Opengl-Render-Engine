#include "CameraMesh.h"
#include "glew\glew.h"

namespace lyd
{
	CameraMesh::CameraMesh()
	{
		cVAO = 0; 
		cVBO = 0; 
		cIBO = 0;

		initVertex();
		initIndex();
		setupMesh();
	}

	CameraMesh::~CameraMesh()
	{
		if (cVAO != 0) glDeleteVertexArrays(1, &cVAO);
		if (cVBO != 0) glDeleteBuffers(1, &cVBO);
		if (cIBO != 0) glDeleteBuffers(1, &cIBO);
		cVAO = 0;
		cVBO = 0;
		cIBO = 0;
	}

	unsigned int cameraIndices[] = {
		0, 1, 1, 2, 2, 3, 3, 0,
		4, 5, 5, 6, 6, 7, 7, 4,
		0, 6, 1, 7, 2, 4, 3, 5,

		8, 9, 9, 10, 11, 12, 12, 13,
		14, 15, 15, 16, 17, 18, 18, 19,
		8, 13, 9, 12, 10, 11,
		14, 19, 15, 18, 16, 17,
		8, 14, 13, 19, 
		7, 10, 6, 11, 5, 17, 4, 16,
		20, 21, 21, 22, 22, 23, 23, 20,
		24, 25, 25, 26, 26, 27, 27, 24,
		20, 27, 21, 26, 22, 25, 23, 24,
		8, 20, 13, 27, 14, 21, 19, 26
	};

	unsigned char frustumIndices[] = {
		0, 1, 1, 2, 2, 3, 3, 0,
		4, 5, 5, 6, 6, 7, 7, 4,
		0, 4, 1, 5, 2, 6, 3, 7
	};

	void CameraMesh::render()
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_LINES, _indexSize, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, &_vertexFrustum[0]);
		glDrawElements(GL_LINES, sizeof(frustumIndices), GL_UNSIGNED_BYTE, &frustumIndices[0]);
		glDisableVertexAttribArray(0);

		//vao不能共享
		//glBindVertexArray(cVAO);
		//glDrawElements(GL_LINES, sizeof(frustumIndices), GL_UNSIGNED_BYTE, &frustumIndices[0]);
		//glBindVertexArray(0);
	}

	void CameraMesh::setupFrustumMesh()
	{
		if (cVAO != 0 && cVAO != 0 && cVAO != 0)
		{
			glBindVertexArray(cVAO);
			glBindBuffer(GL_ARRAY_BUFFER, cVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, _vertexFrustum.size() * sizeof(float), &_vertexFrustum[0]);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			return;
		}

		glGenVertexArrays(1, &cVAO);
		glGenBuffers(1, &cVBO);
		glGenBuffers(1, &cIBO);

		glBindVertexArray(cVAO);

		glBindBuffer(GL_ARRAY_BUFFER, cVBO);
		glBufferData(GL_ARRAY_BUFFER, _vertexFrustum.size() * sizeof(float), &_vertexFrustum[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(frustumIndices), frustumIndices, GL_STATIC_DRAW);

		//顶点
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void CameraMesh::initIndex()
	{
		_index.clear();
		for (size_t i = 0; i < 96; i++)
		{
			_index.push_back(cameraIndices[i]);
		}
	}

	void CameraMesh::initVertex()
	{
		addVertex(-0.8540, -0.7730, -3.3202);//左 下 后
		addVertex( 0.8540, -0.7730, -3.3202);//右 下 后
		addVertex( 0.8540,  0.7730, -3.3202);//右 上 后
		addVertex(-0.8540,  0.7730, -3.3202);//左 上 后

		addVertex( 0.8540,  0.7730, -1.8933);//右 上 前
		addVertex(-0.8540,  0.7730, -1.8933);//左 上 前
		addVertex(-0.8540, -0.7730, -1.8933);//左 下 前
		addVertex( 0.8540, -0.7730, -1.8933);//右 下 前

		addVertex( 0.8540, -0.5508, 2.4017);//1	8
		addVertex( 0.8540, -1.4321, 1.5204);//2	9
		addVertex( 0.8540, -1.4321, -1.3001);//3	10
		addVertex(-0.8540, -1.4321, -1.3001);//3	11
		addVertex(-0.8540, -1.4321, 1.5204);//2	12
		addVertex(-0.8540, -0.5508, 2.4017);//1	13

		addVertex( 0.8540,  0.5508, 2.4017);//1	14
		addVertex( 0.8540,  1.4321, 1.3001);//2	15
		addVertex( 0.8540,  1.4321, -1.3001);//3	16
		addVertex(-0.8540,  1.4321, -1.3001);//3	17
		addVertex(-0.8540,  1.4321, 1.3001);//2	18
		addVertex(-0.8540,  0.5508, 2.4017);//1	19

		addVertex( 1.0740, -0.7711, 4.6486);//1	20
		addVertex( 1.0740,  0.7711, 4.6486);//2	21
		addVertex( 1.7915,  1.4872, 5.6400);//3	22
		addVertex( 1.7915, -1.4872, 5.6400);//4	23

		addVertex(-1.7915, -1.4872, 5.6400);//4	24
		addVertex(-1.7915,  1.4872, 5.6400);//3	25
		addVertex(-1.0740,  0.7711, 4.6486);//2	26
		addVertex(-1.0740, -0.7711, 4.6486);//1	27
	}

	void CameraMesh::addVertex(float x, float y, float z)
	{
		Vertex	vertex;
				vertex.Position = float3(x, y, z);
		_vertex.push_back(vertex);
	}

	void CameraMesh::setFrustumVertex(float* vertex)
	{
		_vertexFrustum.clear();
		for (size_t i = 0; i < 24; ++i)
		{
			_vertexFrustum.push_back(vertex[i]);
		}
		setupFrustumMesh();
	}
}