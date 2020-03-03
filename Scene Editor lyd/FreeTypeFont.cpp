#include "FreeTypeFont.h"
#include "glew\glew.h"

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#pragma comment(lib, "freetype.lib")

namespace lyd
{
	FreeTypeFont::FreeTypeFont()
	{
		memset(_character, 0, sizeof(_character));

		_fontTexture = 0;
		_library = 0;
		_face = 0;
		_yStart = 0;
		_xStart = 0;
		_fontSize = 0;
		_maxFontPixelX = 0;
		_maxFontPixelY = 0;
		_textureHeight = 512;
		_textureWidth = 512;
		_notFoundIndex = 0;
		_isFirstNotFound = true;

		FT_Init_FreeType(&_library);//初始化freetype

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(FontVertex) * 6 * 100, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	FreeTypeFont::~FreeTypeFont()
	{
		destory();
	}

	void		FreeTypeFont::destory()
	{
		if (_fontTexture != 0)
		{
			glDeleteTextures(1, &_fontTexture);
			_fontTexture = 0;
		}

		if (_face)
		{
			FT_Done_Face(_face);
			_face = 0;
		}

		if (_library)
		{
			FT_Done_FreeType(_library);
			_library = 0;
		}
	}

	unsigned int FreeTypeFont::getTextureHandle()
	{
		return _fontTexture;
	}

	int			FreeTypeFont::getWidth()
	{
		return _textureWidth;
	}
	int			FreeTypeFont::getHeight()
	{
		return _textureHeight;
	}

	void		FreeTypeFont::begin(int width, int height)
	{
		glEnable(GL_CULL_FACE);
		//glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glm::mat4 projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
		_shader.begin();
		_shader.setMat4("projection", projection);
	}
	void		FreeTypeFont::end()
	{
		_shader.end();
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		//glEnable(GL_DEPTH_TEST);
	}

	bool		FreeTypeFont::createFont(const char* facename, int fontsize, int w, int h)
	{
		if (!_library) return false;

		_textureWidth = w;
		_textureHeight = h;

		_fontSize = fontsize;

		if (_face)
		{
			FT_Done_Face(_face);
			_xStart = 0;
			_yStart = 0;
			_maxFontPixelX = 0;
			_maxFontPixelY = 0;
			memset(_character, 0, sizeof(_character));
		}

		FT_Error	error = FT_New_Face(_library, facename, 0, &_face);

		if (error != 0) return false;

		//if (_face->charmaps != 0 && _face->num_charmaps > 0)
		//{
		//	FT_Select_Charmap(_face, _face->charmaps[0]->encoding);
		//}
		//FT_Select_Charmap(_face, FT_ENCODING_UNICODE);//选用unicode字符映射

		//设置字体大小
		//FT_F26Dot6 ftSize = (FT_F26Dot6)(_fontSize * (1 << 6));//freetype字体大小是象素的64倍，所以要乘64
		//FT_Set_Char_Size(_face, ftSize, 0, 72, 72);//设置字符尺寸	字体库的指针	宽	高	分辨率(横向 纵向)
		FT_Set_Pixel_Sizes(_face, 0, _fontSize);

		if (_fontTexture != 0)
		{
			glDeleteTextures(1, &_fontTexture);
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glGenTextures(1, &_fontTexture);
		glBindTexture(GL_TEXTURE_2D, _fontTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _textureWidth, _textureHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
		return true;
	}

	glm::vec2	FreeTypeFont::drawText(int width, int height, int x, int y, int z, wchar_t* text, size_t len, glm::vec3 color, FontVertex** vertexs, size_t*vertexLength)
	{
		begin(width, height);
		_shader.setVec3("textColor", color);

		float		texWidth = float(_textureWidth);//当前纹理宽
		float		texHeight = float(_textureHeight);//当前纹理高
		float		xStart = float(x);//开始的 x y z 
		float		yStart = float(y/* + _fontSize*/);
		float		zStart = float(z);
		unsigned	index = 0;
		unsigned	size = len == 0 ? wcslen(text) : len;
		FontVertex*	vertex = (FontVertex*)_curVertexBuffer;
		glm::vec2	vSize(0, 0);

		if (size == 0)
		{
			return  vSize;
		}
		for (unsigned i = 0; i < size; ++i)
		{
			if (text[i] == 32)
			{
				//空格
				xStart += (_fontSize / 2.0f);
				continue;
			}

			Character   ch = getCharacter(text[i]);

			float       h = float(ch.y1 - ch.y0);
			float       w = float(ch.x1 - ch.x0);
			float       offsetY = (float(h) - float(ch.offsetY));
			/**
			*   第一个点
			*/
			vertex[index + 0].x = xStart;
			vertex[index + 0].y = yStart - offsetY;
			vertex[index + 0].z = zStart;
			vertex[index + 0].u = ch.x0 / texWidth;
			vertex[index + 0].v = ch.y1 / texHeight;
			/**
			*   第二个点
			*/
			vertex[index + 1].x = xStart + w;
			vertex[index + 1].y = yStart - offsetY;
			vertex[index + 1].z = zStart;
			vertex[index + 1].u = ch.x1 / texWidth;
			vertex[index + 1].v = ch.y1 / texHeight;
			/**
			*   第三个点
			*/
			vertex[index + 2].x = xStart + w;
			vertex[index + 2].y = yStart + h - offsetY;
			vertex[index + 2].z = zStart;
			vertex[index + 2].u = ch.x1 / texWidth;
			vertex[index + 2].v = ch.y0 / texHeight;
			/**
			*   第一个点
			*/
			vertex[index + 3].x = xStart;
			vertex[index + 3].y = yStart - offsetY;
			vertex[index + 3].z = zStart;
			vertex[index + 3].u = ch.x0 / texWidth;
			vertex[index + 3].v = ch.y1 / texHeight;
			/**
			*   第三个点
			*/
			vertex[index + 4].x = xStart + w;
			vertex[index + 4].y = yStart + h - offsetY;
			vertex[index + 4].z = zStart;
			vertex[index + 4].u = ch.x1 / texWidth;
			vertex[index + 4].v = ch.y0 / texHeight;
			/**
			*   第四个点
			*/
			vertex[index + 5].x = xStart;
			vertex[index + 5].y = yStart + h - offsetY;
			vertex[index + 5].z = zStart;
			vertex[index + 5].u = ch.x0 / texWidth;
			vertex[index + 5].v = ch.y0 / texHeight;

			index += 6;
			xStart += w + (ch.offsetX + 1);

			vSize.x += w + (ch.offsetX + 1);//计算所有文字占用的大小      就是ui的矩形宽高

			vSize.y = glm::max<float>(h + offsetY, vSize.y);

		}

		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_2D, _fontTexture);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, index * sizeof(FontVertex), vertex);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, index);

		end();
		/**
		*   向外输出顶点数据.
		*/
		if (vertexs)
		{
			*vertexs = vertex;
		}
		if (vertexLength)
		{
			*vertexLength = index;
		}
		return  vSize;

	}

	Character	FreeTypeFont::getCharacter(int ch)
	{
		if (ch > 65535)
		{
			if (!_isFirstNotFound) return _character[_notFoundIndex];
			return _character[65535];
		}

		if (_character[ch].x0 == 0 && _character[ch].y0 == 0 && _character[ch].x1 == 0 && _character[ch].y1 == 0)
		{
			//没加载过这个字符
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);//指定象素对齐 1字节

			if (_xStart + glm::max<int>(_maxFontPixelX, _fontSize) > _textureWidth)
			{
				//写满一行了，下移一行
				_xStart = 0;
				_yStart += glm::max<int>(_maxFontPixelY, _fontSize);
			}

			//查找是否有这个字符
			FT_UInt a = FT_Get_Char_Index((FT_Face)_face, ch);
			if (a == 0)
			{
				if (_isFirstNotFound)
				{
					_isFirstNotFound = false;
					_notFoundIndex = ch;
				}
				else
				{
					return _character[_notFoundIndex];
				}
			}

			//加载字模信息       字		方式
			FT_Error	err = FT_Load_Char(_face, ch, FT_LOAD_RENDER);
			if (err != 0)
			{
				if (!_isFirstNotFound) return _character[_notFoundIndex];
				return _character[65535];
			}

			glBindTexture(GL_TEXTURE_2D, _fontTexture);

			_character[ch].x0 = _xStart;
			_character[ch].y0 = _yStart;
			_character[ch].x1 = _xStart + _face->glyph->bitmap.width;
			_character[ch].y1 = _yStart + _face->glyph->bitmap.rows;

			_character[ch].offsetY = _face->glyph->bitmap_top;
			_character[ch].offsetX = _face->glyph->bitmap_left;
			_character[ch].advance = _face->glyph->advance.x;

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexSubImage2D(
				GL_TEXTURE_2D,
				0,
				_xStart,
				_yStart,
				glm::max<int>(1, _face->glyph->bitmap.width),
				glm::max<int>(1, _face->glyph->bitmap.rows),
				GL_RED,
				GL_UNSIGNED_BYTE,
				_face->glyph->bitmap.buffer
			);
			_xStart += (_face->glyph->bitmap.width + 1);
			_maxFontPixelY = glm::max<int>(_maxFontPixelY, _face->glyph->bitmap.rows);
			_maxFontPixelX = glm::max<int>(_maxFontPixelX, _face->glyph->bitmap.width);
		}
		return  _character[ch];
	}
}