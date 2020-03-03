#include "PlaneMesh.h"
#include "glew\glew.h"

namespace lyd
{
	PlaneMesh::PlaneMesh()
	{
		initVertex();
		setupMesh();
	}

	PlaneMesh::~PlaneMesh()
	{
	}

	void PlaneMesh::render()
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, _indexSize, _vertexSize);
		glBindVertexArray(0);
	}

	void PlaneMesh::initVertex()
	{
		addVertex( 1.0f, 0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
		addVertex(-1.0f, 0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		addVertex(-1.0f, 0.5f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
						  		
		addVertex( 1.0f, 0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
		addVertex(-1.0f, 0.5f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		addVertex( 1.0f, 0.5f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	}

	void PlaneMesh::addVertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
	{
		Vertex	vertex;
				vertex.Position =	float3(x, y, z);
				vertex.Normal	=	float3(nx, ny, nz);
				vertex.TexCoords=	float2(u, v);
		_vertex.push_back(vertex);
	}
}