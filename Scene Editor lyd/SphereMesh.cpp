#include "SphereMesh.h"
#include "glew\glew.h"

namespace lyd
{
	SphereMesh::SphereMesh()
	{
		initVertex();
		setupMesh();
	}

	SphereMesh::~SphereMesh()
	{
	}

	void SphereMesh::render()
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLE_STRIP, _indexSize, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void SphereMesh::initVertex()
	{
		const unsigned int X_SEGMENTS = 32;
		const unsigned int Y_SEGMENTS = 32;

		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
		{
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
			{
				Vertex	vertex;
				float	xSegment			=	(float)x / (float)X_SEGMENTS;
				float	ySegment			=	(float)y / (float)Y_SEGMENTS;
				float	xPos				=	std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float	yPos				=	std::cos(ySegment * PI);
				float	zPos				=	std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

						vertex.Position		=	float3(xPos, yPos, zPos);
						vertex.TexCoords	=	float2(xSegment, ySegment);
						vertex.Normal		=	float3(xPos, yPos, zPos);

				_vertex.push_back(vertex);
			}
		}

		bool oddRow = false;
		for (int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (int x = 0; x <= X_SEGMENTS; ++x)
				{
					_index.push_back(y       * (X_SEGMENTS + 1) + x);
					_index.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					_index.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					_index.push_back(y       * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}

		convertTBN();
	}

	void SphereMesh::convertTBN()
	{
		for (auto itr = _index.begin(); itr != _index.end(); itr += 3)
		{
			auto itr0 = _vertex[*(itr + 0)];
			auto itr1 = _vertex[*(itr + 1)];
			auto itr2 = _vertex[*(itr + 2)];

			//Shortcuts for vertices
			float3  v0 = float3(itr0.Position.x, itr0.Position.y, itr0.Position.z);
			float3  v1 = float3(itr1.Position.x, itr1.Position.y, itr1.Position.z);
			float3  v2 = float3(itr2.Position.x, itr2.Position.y, itr2.Position.z);

			float2  uv0 = float2(itr0.TexCoords.x, itr0.TexCoords.y);
			float2  uv1 = float2(itr1.TexCoords.x, itr1.TexCoords.y);
			float2  uv2 = float2(itr2.TexCoords.x, itr2.TexCoords.y);

			// Edges of the triangle : postion delta
			float3  deltaPos1 = v1 - v0;
			float3  deltaPos2 = v2 - v0;
			// UV delta
			float2	deltaUV1 = uv1 - uv0;
			float2	deltaUV2 = uv2 - uv0;

			float	r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
			float3	tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
			float3	binormal = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

			itr0.Tangent.x = tangent.x;		itr1.Tangent.x = tangent.x;		itr2.Tangent.x = tangent.x;
			itr0.Tangent.y = tangent.y;		itr1.Tangent.y = tangent.y;		itr2.Tangent.y = tangent.y;
			itr0.Tangent.z = tangent.z;		itr1.Tangent.z = tangent.z;		itr2.Tangent.z = tangent.z;

			itr0.Bitangent.x = binormal.x;	itr1.Bitangent.x = binormal.x;	itr2.Bitangent.x = binormal.x;
			itr0.Bitangent.y = binormal.y;	itr1.Bitangent.y = binormal.y;	itr2.Bitangent.y = binormal.y;
			itr0.Bitangent.z = binormal.z;	itr1.Bitangent.z = binormal.z;	itr2.Bitangent.z = binormal.z;

			_vertex[*(itr + 0)]	= itr0;
			_vertex[*(itr + 1)] = itr1;
			_vertex[*(itr + 2)] = itr2;
		}
	}
}