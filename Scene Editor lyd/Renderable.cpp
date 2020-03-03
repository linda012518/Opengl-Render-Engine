#include "Renderable.h"
#include "Shader.h"
#include "DefaultShader.h"
#include "Mesh.h"
#include "Material.h"

namespace lyd
{
	Renderable::Renderable()
	{
		//_material = new Material*[];
	}

	Renderable::~Renderable()
	{
		deleteMaterial(0);
		deleteMaterial(1);
	}

	void Renderable::render(matrix4& m, matrix4& v, matrix4& p, float3& cameraPos)
	{
		if (_material == nullptr) return;

		if (_material->begin())
		{
			_material->setMVP(m, v, p);
			_material->setCameraPos(cameraPos);

			if (_mesh != nullptr)
			{
				_mesh->render();
			}

			_material->end();
		}

		if (_material2 == nullptr) return;

		if (_material2->begin())
		{
			_material2->setMVP(m, v, p);
			_material2->setCameraPos(cameraPos);

			if (_mesh != nullptr)
			{
				_mesh->render();
			}

			_material2->end();
		}
	}

	void Renderable::setMesh(Mesh* mesh)
	{
		_mesh = mesh;
	}

	void Renderable::setMaterial(Material* material, short index)
	{
		deleteMaterial(index);
		if (index == 0)
		{
			_material = material;

			if (!_material) return;
			_material->setName(_name);
		}
		else if (index == 1)
		{
			_material2 = material;

			if (!_material2) return;
			_material2->setName(_name);
		}

	}

	void Renderable::deleteMaterial(short index)
	{
		if (index == 0)
		{
			if (_material)
			{
				delete _material;
				_material = nullptr;
			}
		}
		else if (index == 1)
		{
			if (_material2)
			{
				delete _material2;
				_material2 = nullptr;
			}
		}

	}

	void Renderable::setName(const char* name)
	{
		if (!name) return;

		Obj::setName(name);

		if (!_material) return;

		_material->setName(name);

		if (!_material2) return;

		_material2->setName(name);
	}

	Material* Renderable::getMaterial(short index) const
	{
		if (index == 0)
		{
			return _material;
		}
		else if (index == 1)
		{
			return _material2;
		}
		return nullptr;
	}

}