#include "Mesh.h"
#include "glew\glew.h"

namespace lyd
{
	Mesh::Mesh()
	{
		VAO = 0;
		VBO = 0;
		IBO = 0;
	}

	Mesh::Mesh(std::vector<Vertex> vertex, std::vector<unsigned int> index)
	{
		_vertex =	vertex;
		_index	=	index;

		VAO		=	0;
		VBO		=	0;
		IBO		=	0;

		setupMesh();
	}

	Mesh::~Mesh()
	{
		_vertex.clear();
		_index.clear();

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		if (IBO != 0)
		{
			glDeleteBuffers(1, &IBO);
		}
	}

	void Mesh::render()
	{
		glBindVertexArray(VAO);
		if (IBO != 0)
		{
			glDrawElements(GL_TRIANGLES, _indexSize, GL_UNSIGNED_INT, 0);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, _vertexSize);
		}
		glBindVertexArray(0);
	}

	void Mesh::setupMesh()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		if (_index.size() > 3)
		{
			glGenBuffers(1, &IBO);
		}

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, _vertex.size() * sizeof(Vertex), &_vertex[0], GL_STATIC_DRAW);
		_vertexSize = _vertex.size();

		if (_index.size() > 3)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, _index.size() * sizeof(unsigned int), &_index[0], GL_STATIC_DRAW);
			_indexSize = _index.size();
		}

		//offsetof(s, m)，它的第一个参数是一个结构体，第二个参数是这个结构体中变量的名字。这个宏会返回那个变量距结构体头部的字节偏移量(Byte Offset)
		//顶点
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		//法线
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		//uv
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		//切线
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		//颜色
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
		//副切线
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		//glBindVertexArray(0);

		//顺序很重要
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Mesh::initVertex() { }
	void Mesh::initIndex() { }
}