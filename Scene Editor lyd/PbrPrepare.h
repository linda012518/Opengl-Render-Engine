#ifndef _Pbr_Prepare_
#define _Pbr_Prepare_

namespace lyd
{
	class PbrPrepare
	{
	public:
		PbrPrepare(int inwidth, int inheight);
		virtual ~PbrPrepare();

		unsigned int getIrradianceMap();
		unsigned int getPrefilterMap();
		unsigned int getBrdfMap();

		unsigned int getSkybox();

	private:
		unsigned int irradianceMap, prefilterMap, brdfLUTTexture;
		unsigned int envCubemap;
	};
}

#endif // !_Pbr_Prepare_



