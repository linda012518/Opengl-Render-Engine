#include "MultipleLightMaterial.h"
#include "MultipleLightShader.h"
#include "TextureMgr.h"

namespace lyd
{
	MultipleLightMaterial::MultipleLightMaterial()
	{
		_shader = new MultipleLightShader();

		if (_shader == nullptr) return;

		_diffuseMap = _texMgr->getTexture("map/container2.png");
		_specularMap = _texMgr->getTexture("map/container2_specular.png");

		_shader->begin();

		_shader->setInt("material.diffuse", 0);
		_shader->setInt("material.specular", 1);
		_shader->setFloat("material.shininess", 32.0f);

		_shader->setVec3("dirlight.ambient", 0.4f, 0.4f, 0.4f);
		_shader->setVec3("dirlight.diffuse", 0.8f, 0.8f, 0.8f);
		_shader->setVec3("dirlight.specular", 1.0f, 1.0f, 1.0f);

		_shader->setFloat("pointlight.constant", 1.0f);
		_shader->setFloat("pointlight.linear", 0.09f);
		_shader->setFloat("pointlight.quadratic", 0.032f);

		_shader->setVec3("pointlight.ambient", 0.4f, 0.4f, 0.4f);
		_shader->setVec3("pointlight.diffuse", 0.8f, 0.8f, 0.8f);
		_shader->setVec3("pointlight.specular", 1.0f, 1.0f, 1.0f);
	}

	MultipleLightMaterial::~MultipleLightMaterial()
	{
	}

	bool MultipleLightMaterial::begin()
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

	void MultipleLightMaterial::setLightDir(float3& dir)
	{
		if (_shader == nullptr) return;

		_shader->begin();

		_shader->setVec3("dirlight.direction", dir);
	}

	void MultipleLightMaterial::setLightPos(float3& pos)
	{
		if (_shader == nullptr) return;

		_shader->begin();

		_shader->setVec3("pointlight.position", pos);
	}
}