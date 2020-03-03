#include "GameObject.h"
#include "Transform.h"
#include "Renderable.h"
#include "Mesh.h"

namespace lyd
{
	GameObject::GameObject()
	{
		_transform = new Transform();
		_renderable = new Renderable();
	}

	GameObject::~GameObject()
	{
		if (_renderable)
		{
			delete _renderable;
			_renderable = nullptr;
		}
		if (_mesh)
		{
			delete _mesh;
			_mesh = nullptr;
		}
		if (_transform)
		{
			delete _transform;
			_transform = nullptr;
		}
	}

	void GameObject::setName(const char* name)
	{
		if (!name) return;

		Obj::setName(name);

		if (!_transform) return;
		if (!_renderable) return;
		if (!_mesh) return;

		_transform->setName(name);
		_renderable->setName(name);
		_mesh->setName(name);
	}

	void GameObject::render(matrix4& v, matrix4& p, float3& cameraPos)
	{
		if (!_transform) return;
		_transform->update();
		if (!_renderable) return;

		_renderable->render(_transform->local(), v, p, cameraPos);
	}

	void GameObject::setMesh(Mesh* mesh)
	{
		_mesh = mesh;

		if (!_renderable) return;

		_renderable->setMesh(mesh);
	}

	Mesh* GameObject::getMesh() const
	{
		return _mesh;
	}

	Renderable* GameObject::getRenderable() const
	{
		return _renderable;
	}

	Transform* GameObject::getTransform() const
	{
		return _transform;
	}
}