#include "Material.h"
#include "Shader.h"
#include "TextureMgr.h"

#include <Windows.h>

namespace lyd
{
	Material::Material(Shader* shader)
	{
		_texMgr = new TextureMgr();

		_shader = shader;
		setColor(float4(1, 1, 1, 1));

		_currentTime = GetTickCount();
	}

	Material::Material() { _currentTime = GetTickCount();  _texMgr = new TextureMgr(); }

	Material::~Material()
	{
		if (_shader != nullptr)
		{
			delete _shader;
			_shader = nullptr;
		}
		if (_texMgr != nullptr)
		{
			delete _texMgr;
			_texMgr = nullptr;
		}
	}

	Shader* Material::getShader() const
	{
		return _shader;
	}

	void Material::setShader(Shader* shader)
	{
		if (_shader != nullptr)
		{
			delete _shader;
			_shader = nullptr;
		}

		_shader = shader;
		_shader->setName(_name);
		setColor(_color);
	}

	bool Material::begin()
	{
		if (_shader != nullptr)
		{
			_shader->begin();

			float a = GetTickCount() - _currentTime;

			_shader->setFloat("time", a / 1000.0f);
			return true;
		}
		return false;
	}

	void Material::end()
	{
		if (_shader == nullptr) return;

		_shader->end();
	}

	void Material::setMVP(matrix4& m, matrix4& v, matrix4& p)
	{
		if (_shader == nullptr) return;

		_shader->setMVP(m, v, p);
	}

	void Material::setCameraPos(float3& cameraPos)
	{
		_shader->setCameraPos(cameraPos);
	}

	void Material::setColor(float4& color)
	{
		if (_shader == nullptr) return;

		_color = color;
		_shader->begin();
		_shader->setColor(_color);
	}
	void Material::setColor(float r, float g, float b, float a)
	{
		setColor(float4(r, g, b, a));
	}

	void Material::setName(const char* name)
	{
		if (!name) return;

		Obj::setName(name);

		if (_shader == nullptr) return;

		_shader->setName(name);
	}

}