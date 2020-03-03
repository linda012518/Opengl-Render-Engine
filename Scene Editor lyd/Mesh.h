#ifndef _Mesh_
#define _Mesh_

#include <vector>

#include "Obj.h"
#include "mathf\CELLMath.hpp"

namespace lyd
{
	struct Vertex {
		float3		Position;	//位置
		float3		Normal;		//法线
		float2		TexCoords;	//uv
		float3		Tangent;	//切线
		float3		Bitangent;	//副切线
		float3		Color;		//颜色
	};

	class Mesh : public Obj
	{
	public:
		std::vector<Vertex>			_vertex;
		std::vector<unsigned int>	_index;

	protected:
		unsigned int VAO, VBO, IBO;
		unsigned int _indexSize		= 0;
		unsigned int _vertexSize	= 0;

	public:
		Mesh();
		Mesh(std::vector<Vertex> vertex, std::vector<unsigned int> index);
		virtual ~Mesh();

		virtual void render();

	protected:
		virtual void initVertex();
		virtual void initIndex();

		virtual void setupMesh();
	};
}

#endif // !_Mesh_


