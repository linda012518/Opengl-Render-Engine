#include "NormalMapMaterial.h"
#include "NormalMapShader.h"
#include "TextureMgr.h"

namespace lyd
{
	NormalMapMaterial::NormalMapMaterial()
	{
		_shader = new NormalMapShader();

		if (_shader == nullptr) return;

		_diffuseMap		= _texMgr->getTexture("map/brickwall.jpg");
		_normalMap		= _texMgr->getTexture("map/brickwall_normal.jpg");

		_shader->begin();
		_shader->setInt("material.diffuse", 0);
		_shader->setInt("material.normal", 1);
		_shader->setFloat("material.shininess", 64.0f);
		_shader->setVec3("material.specular", 1.0f, 1.0f, 1.0f);

		_shader->setVec3("light.ambient", 0.4f, 0.4f, 0.4f);
		_shader->setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
		_shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	}

	NormalMapMaterial::~NormalMapMaterial() { }

	bool NormalMapMaterial::begin()
	{
		if (Material::begin())
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _diffuseMap);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, _normalMap);
			return true;
		}
		return false;
	}


	void NormalMapMaterial::setLightDir(float3& dir)
	{
		if (_shader == nullptr) return;

		_shader->begin();

		_shader->setVec3("lightDir", dir);
	}

}