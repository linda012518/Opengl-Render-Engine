#ifndef _Renderable_
#define _Renderable_

#include "Obj.h"
#include "mathf\CELLMath.hpp"

namespace lyd
{
	class Shader;
	class Mesh;
	class Material;
}

namespace lyd
{
	class Renderable : public Obj
	{
	public:
		Renderable();
		virtual ~Renderable();

		virtual void render(matrix4& m, matrix4& v, matrix4& p, float3& cameraPos);

		virtual void setMesh(Mesh* mesh);

		virtual void setMaterial(Material* material, short index = 0);

		virtual void setName(const char* name);

		Material* getMaterial(short index = 0) const;

	protected:
		virtual void deleteMaterial(short index);

	protected:
		Material*	_material	= nullptr;
		Material*	_material2	= nullptr;
		Mesh*		_mesh		= nullptr;
	};
}

#endif // !_Renderable_


