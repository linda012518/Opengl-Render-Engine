#include "QuadMesh.h"
#include "glew\glew.h"

namespace lyd
{
	QuadMesh::QuadMesh()
	{
		initVertex();
		setupMesh();
	}

	QuadMesh::~QuadMesh() { }

	void QuadMesh::render()
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	void QuadMesh::initVertex()
	{

		Vertex	vertex1;
				vertex1.Position	= float3(-1.0f, 1.0f, 0.0f);
				vertex1.TexCoords	= float2(0.0f, 1.0f);
		_vertex.push_back(vertex1);

		Vertex	vertex2;
				vertex2.Position	= float3(-1.0f, -1.0f, 0.0f);
				vertex2.TexCoords	= float2(0.0f, 0.0f);
		_vertex.push_back(vertex2);

		Vertex	vertex3;
				vertex3.Position	= float3(1.0f, 1.0f, 0.0f);
				vertex3.TexCoords	= float2(1.0f, 1.0f);
		_vertex.push_back(vertex3);

		Vertex	vertex4;
				vertex4.Position	= float3(1.0f, -1.0f, 0.0f);
				vertex4.TexCoords	= float2(1.0f, 0.0f);
		_vertex.push_back(vertex4);
	}
}