#include "PbrMaterial.h"
#include "PbrShader.h"
#include "TextureMgr.h"

namespace lyd
{
	PbrMaterial::PbrMaterial()
	{
		_shader = new PbrShader();

		if (_shader == nullptr) return;

		_shader->begin();
		_shader->setInt("irradianceMap", 0);
		_shader->setInt("prefilterMap", 1);
		_shader->setInt("brdfLUT", 2);
		_shader->setInt("albedoMap", 3);
		_shader->setInt("normalMap", 4);
		_shader->setInt("metallicMap", 5);
		_shader->setInt("roughnessMap", 6);
		_shader->setInt("aoMap", 7);
	}

	void PbrMaterial::loadTexture(const char* albedo, const char* normal, const char* methallic, const char* roughness, const char* ao)
	{
		_albedoMap = _texMgr->getTexture(albedo);
		_normalMap = _texMgr->getTexture(normal);
		_metallicMap = _texMgr->getTexture(methallic);
		_roughnessMap = _texMgr->getTexture(roughness);
		_aoMap = _texMgr->getTexture(ao);
	}

	void PbrMaterial::setIrrPreBrdfMap(unsigned int irr, unsigned int pre, unsigned int brdf)
	{
		_irradianceMap = irr;
		_prefilterMap = pre;
		_brdfMap = brdf;
	}

	PbrMaterial::~PbrMaterial()
	{
	}

	bool PbrMaterial::begin()
	{
		if (Material::begin())
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, _irradianceMap);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_CUBE_MAP, _prefilterMap);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, _brdfMap);

			// rusted iron
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, _albedoMap);
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, _normalMap);
			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_2D, _metallicMap);
			glActiveTexture(GL_TEXTURE6);
			glBindTexture(GL_TEXTURE_2D, _roughnessMap);
			glActiveTexture(GL_TEXTURE7);
			glBindTexture(GL_TEXTURE_2D, _aoMap);
			return true;
		}
		return false;
	}

	void PbrMaterial::setLights(float3 pos[], float3 color[])
	{
		if (_shader == nullptr) return;

		_shader->setVec3("lightPositions[0]", pos[0]);
		_shader->setVec3("lightColors[0]", color[0]);

		_shader->setVec3("lightPositions[1]", pos[1]);
		_shader->setVec3("lightColors[1]", color[1]);

		_shader->setVec3("lightPositions[2]", pos[2]);
		_shader->setVec3("lightColors[2]", color[2]);

		_shader->setVec3("lightPositions[3]", pos[3]);
		_shader->setVec3("lightColors[3]", color[3]);
	}
}