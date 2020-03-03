#ifndef _Texture_Mgr_
#define _Texture_Mgr_

#include "Singleton.h"
#include <map>
#include <string>
#include <vector>
#include <mutex>

namespace lyd
{
	struct Texture {
		std::string		type;		//类型 漫反射/镜面反射/法线
		std::string		path;		//路径
		unsigned int	id;			//id
	};

#pragma region 暂时去掉单例，后期优化
	//class TextureMgr : public Singleton<TextureMgr>
	//{
	//	//友元访问私有构造
	//	friend class Singleton<TextureMgr>;
	//public:
	//	virtual ~TextureMgr();

	//	TextureMgr(const TextureMgr&) = delete;
	//	TextureMgr& operator =(const TextureMgr&) = delete;

	//	unsigned int getTexture(const char* filename, bool gammaCorrection = false);

	//	unsigned int loadCubemap(std::vector<std::string> faces);

	//private:
	//	TextureMgr() = default;

	//	unsigned int createTextureFromImage(const char* fileName, bool gammaCorrection = false);

	//	unsigned int createTexture(int w, int h, const void* data, bool gammaCorrection = false);

	//private:
	//	std::map<std::string, unsigned int> _texMap;
	//	std::mutex						_mutex;
	//	unsigned int _dataFormat = 0;
	//	unsigned int _internalFormat = 0;
	//};
#pragma endregion

	class TextureMgr
	{
	public:
		TextureMgr();
		virtual ~TextureMgr();

		unsigned int getTexture(const char* filename, bool gammaCorrection = false);

		unsigned int loadCubemap(std::vector<std::string> faces);

		unsigned int loadRectHdrMap(const char* filename);

	private:

		unsigned int createTextureFromImage(const char* fileName, bool gammaCorrection = false);

		unsigned int createTexture(int w, int h, const void* data, bool gammaCorrection = false);

	private:
		std::map<std::string, unsigned int> _texMap;
		std::mutex						_mutex;
		unsigned int _dataFormat = 0;
		unsigned int _internalFormat = 0;
	};



}

#endif // !_Texture_Mgr_



