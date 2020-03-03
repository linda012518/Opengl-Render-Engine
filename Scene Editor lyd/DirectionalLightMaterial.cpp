#include "DirectionalLightMaterial.h"
#include "DirectionalLightShader.h"
#include "TextureMgr.h"

namespace lyd
{
	DirectionalLightMaterial::DirectionalLightMaterial()
	{
		_shader = new DirectionalLightShader();

		if (_shader == nullptr) return;

		_diffuseMap		= _texMgr->getTexture("map/container2.png");
		_specularMap	= _texMgr->getTexture("map/container2_specular.png");

		_shader->begin();

		_shader->setInt("material.diffuse", 0);
		_shader->setInt("material.specular", 1);
		_shader->setFloat("material.shininess", 256.0f);

		_shader->setVec3("light.ambient", 0.4f, 0.4f, 0.4f);
		_shader->setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
		_shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

	}

	DirectionalLightMaterial::~DirectionalLightMaterial() { }

	bool DirectionalLightMaterial::begin()
	{
		if (Material::begin())
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _diffuseMap);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, _specularMap);
			return true;
		}
		return false;
	}

	void DirectionalLightMaterial::setLightDir(float3& dir)
	{
		if (_shader == nullptr) return;

		_shader->begin();

		_shader->setVec3("light.direction", dir);
	}
}