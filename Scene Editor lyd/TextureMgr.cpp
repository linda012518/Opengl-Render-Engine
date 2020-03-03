#include "TextureMgr.h"
#include "FileSystem.h"
#include "freeimage\FreeImage.h"
#include "glew\glew.h"
#include "stb_image.h"

#pragma comment(lib, "FreeImage.lib")

namespace lyd
{
	TextureMgr::TextureMgr()
	{
		//stbi_set_flip_vertically_on_load(true);
	}

	TextureMgr::~TextureMgr()
	{
		for (auto itr = _texMap.begin(); itr != _texMap.end(); ++itr)
		{
			glDeleteTextures(1, &itr->second);
		}
	}

	unsigned int TextureMgr::loadCubemap(std::vector<std::string> faces)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int width, height, nrChannels;
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			char filePath[1024];
			FileSystem::getResourcePath(faces[i].c_str(), filePath);

			unsigned char *data = stbi_load(filePath, &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				printf("Cubemap texture failed to load at path: %s", faces[i].c_str());
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//纹理的第三个维度（和位置的z一样）
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		return textureID;
		//它们背后的int值是线性递增的
		//GL_TEXTURE_CUBE_MAP_POSITIVE_X 	右
		//GL_TEXTURE_CUBE_MAP_NEGATIVE_X 	左
		//GL_TEXTURE_CUBE_MAP_POSITIVE_Y 	上
		//GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 	下
		//GL_TEXTURE_CUBE_MAP_POSITIVE_Z 	后
		//GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 	前
	}

	unsigned int TextureMgr::getTexture(char const * path, bool gammaCorrection)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		auto itr = _texMap.find(path);
		if (itr != _texMap.end())
		{
			return itr->second;
		}

		unsigned int textureID;
		glGenTextures(1, &textureID);

		char filePath[1024];
		FileSystem::getResourcePath(path, filePath);

		int width, height, nrComponents;
		unsigned char *data = stbi_load(filePath, &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum internalFormat;
			GLenum dataFormat;
			if (nrComponents == 1)
			{
				internalFormat = dataFormat = GL_RED;
			}
			else if (nrComponents == 3)
			{
				internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
				dataFormat = GL_RGB;
			}
			else if (nrComponents == 4)
			{
				internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
				dataFormat = GL_RGBA;
			}


			glBindTexture(GL_TEXTURE_2D, textureID);

			//specular贴图和法线贴图几乎都在线性空间中
			//diffuse纹理，这种为物体上色的纹理几乎都是在sRGB空间中的
			//GL_SRGB和GL_SRGB_ALPHA
			//OpenGL将自动把颜色校正到线性空间中  没有校验过的贴图
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			printf("Texture failed to load at path: %s", path);
			stbi_image_free(data);
		}

		_texMap[path] = textureID;
		return _texMap[path];
	}

	unsigned int TextureMgr::loadRectHdrMap(const char* filename)
	{
		stbi_set_flip_vertically_on_load(true);

		char filePath[1024];
		FileSystem::getResourcePath(filename, filePath);

		int width, height, nrComponents;
		float *data = stbi_loadf(filePath, &width, &height, &nrComponents, 0);
		unsigned int hdrTexture = 0;
		if (data)
		{
			glGenTextures(1, &hdrTexture);
			glBindTexture(GL_TEXTURE_2D, hdrTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			printf("Failed to load HDR image.");
		}
		return hdrTexture;
	}

	//unsigned int TextureMgr::getTexture(const char* filename, bool gammaCorrection)
	//{
	//	auto itr = _texMap.find(filename);
	//	if (itr != _texMap.end())
	//	{
	//		return itr->second;
	//	}
	//	char filePath[1024];
	//	FileSystem::getResourcePath(filename, filePath);
	//	unsigned int tex = createTextureFromImage(filePath, gammaCorrection);
	//	_texMap[filename] = tex;
	//	return _texMap[filename];
	//}

	unsigned int TextureMgr::createTextureFromImage(const char* fileName, bool gammaCorrection)
	{
		FIBITMAP*               dib = nullptr;
		try
		{
			//1 获取图片格式
			FREE_IMAGE_FORMAT       fifmt = FreeImage_GetFileType(fileName, 0);
			if (fifmt == FIF_UNKNOWN)
			{
				return  0;
			}
			//2 加载图片
			dib = FreeImage_Load(fifmt, fileName, 0);
		}
		catch (const std::exception&)
		{
			printf("TextureMgr::createTextureFromImage error!!!");
		}

		FREE_IMAGE_COLOR_TYPE   type = FreeImage_GetColorType(dib);
		//if (type == FIC_RGB)
		//{
		//	_internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
		//	_dataFormat		= GL_RGB;
		//}
		//else if (type == FIC_RGBALPHA)
		{
			_internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
			_dataFormat		= GL_RGBA;
		}
		//FREE_IMAGE_TYPE			tpye = FreeImage_GetImageType(dib);

		//! 获取数据指针
		FIBITMAP*   temp = dib;
		dib = FreeImage_ConvertTo32Bits(dib);
		FreeImage_Unload(temp);

		BYTE*   pixels = (BYTE*)FreeImage_GetBits(dib);
		int     width = FreeImage_GetWidth(dib);
		int     height = FreeImage_GetHeight(dib);
		//bgr转rgb
		for (int i = 0; i < width * height * 4; i += 4)
		{
			BYTE temp = pixels[i];
			pixels[i] = pixels[i + 2];
			pixels[i + 2] = temp;
		}

		unsigned    res = createTexture(width, height, pixels, gammaCorrection);
		FreeImage_Unload(dib);
		return      res;
	}

	unsigned int TextureMgr::createTexture(int w, int h, const void* data, bool gammaCorrection)
	{
		unsigned    texId;
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, w, h, 0, _dataFormat, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		return  texId;
	}
}