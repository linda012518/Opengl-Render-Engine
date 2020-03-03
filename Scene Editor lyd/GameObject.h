#ifndef _Game_Object_
#define _Game_Object_

#include "Obj.h"
#include "mathf\CELLMath.hpp"

namespace lyd
{
	class Transform;
	class Renderable;
	class Mesh;
}

namespace lyd
{
	class GameObject : public Obj
	{
	protected:
		Transform*		_transform	= nullptr;
		Mesh*			_mesh		= nullptr;
		Renderable*		_renderable = nullptr;
	public:
		GameObject();
		virtual ~GameObject();

		virtual void setName(const char* name);

		virtual void render(matrix4& v, matrix4& p, float3& cameraPos);

		void setMesh(Mesh* mesh);

		Mesh* getMesh() const;

		Renderable* getRenderable() const;
		Transform*  getTransform() const;
	};
}


#endif // !_Game_Object_

