#include "PointLightMaterial.h"
#include "PointLightShader.h"
#include "TextureMgr.h"

namespace lyd
{
	PointLightMaterial::PointLightMaterial()
	{
		_shader = new PointLightShader();

		if (_shader == nullptr) return;

		_diffuseMap		=	_texMgr->getTexture("map/container2.png");
		_specularMap	=	_texMgr->getTexture("map/container2_specular.png");

		_shader->begin();

		_shader->setInt("material.diffuse", 0);
		_shader->setInt("material.specular", 1);
		_shader->setFloat("material.shininess", 32.0f);

		_shader->setFloat("light.constant", 1.0f);
		_shader->setFloat("light.linear", 0.09f);
		_shader->setFloat("light.quadratic", 0.032f);

		_shader->setVec3("light.ambient", 0.4f, 0.4f, 0.4f);
		_shader->setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
		_shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	}

	PointLightMaterial::~PointLightMaterial() { }

	bool PointLightMaterial::begin()
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

	void PointLightMaterial::setLightPos(float3& pos)
	{
		if (_shader == nullptr) return;

		_shader->begin();

		_shader->setVec3("light.position", pos);
	}

}