#ifndef _Free_Type_Font_
#define _Free_Type_Font_

#include <glm/glm.hpp>

#include "ft2build.h"
#include FT_FREETYPE_H

#include "TextShader.h"

namespace lyd
{
	struct FontVertex
	{
		float		x, y, z;
		float		u, v;
	};

	class Character
	{
	public:
		Character()
		{
			x0 = 0;
			y0 = 0;
			x1 = 0;
			y1 = 0;
			offsetX = 0;//偏移  超出横向标准线的部分
			offsetY = 0;//偏移  超出纵向标准线的部分
		}

		unsigned __int64 x0 : 10;//字符图的位置x：像素
		unsigned __int64 y0 : 10;//字符图的位置y：像素
		unsigned __int64 x1 : 10;//字符图的位置x + width		face->glyph->bitmap.width
		unsigned __int64 y1 : 10;//字符图的位置y + height		face->glyph->bitmap.rows

		unsigned __int64 offsetX : 8;//字符相对于原点的水平位置：像素	bearingX	face->glyph->bitmap_left
		unsigned __int64 offsetY : 8;//字符相对于原点的垂直位置：像素	bearingY 	face->glyph->bitmap_top

		unsigned __int64 advance : 8;//原点到下一个字形原点的水平距离	face->glyph->advance.x
	};

	class FreeTypeFont
	{
	public:
		FT_Library		_library;			//freetype对象
		FT_Face			_face;				//字体文件
		unsigned int	_fontTexture;		//存储生成的纹理
		Character		_character[1 << 16];//一共存65535个字，用每个字对应的int值当索引

		TextShader		_shader;

		int				_textureWidth;		//纹理宽
		int				_textureHeight;		//纹理高

		int				_xStart;			//当前字体写到文理上的X位置坐标
		int				_yStart;			//当前字体写到文理上的Y位置坐标

		int				_fontSize;			//字体大小

		int				_maxFontPixelX;		//已加载字符最大X
		int				_maxFontPixelY;		//已加载字符最大Y
		char			_curVertexBuffer[1024 * 512];//当前绘制字符的顶点缓冲

		int				_notFoundIndex;		//找不到的字符都用同一个索引
		bool			_isFirstNotFound;	//是否第一次发现找不到字符

		unsigned int VAO, VBO;

	public:
		FreeTypeFont();
		virtual ~FreeTypeFont();

		void destory();

		unsigned int getTextureHandle();

		int getWidth();
		int getHeight();

		void begin(int width, int height);
		void end();

		bool createFont(const char* facename, int fontsize, int w, int h);

		//输出的指针由外部释放
		glm::vec2 drawText(int width, int height, int x, int y, int z, wchar_t* text, size_t len, glm::vec3 color, FontVertex** vertexs, size_t*vertexLength = 0);

	private:
		Character getCharacter(int ch);
	};
}

#endif // !_Free_Type_Font_