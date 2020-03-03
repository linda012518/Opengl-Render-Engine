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
			offsetX = 0;//ƫ��  ���������׼�ߵĲ���
			offsetY = 0;//ƫ��  ���������׼�ߵĲ���
		}

		unsigned __int64 x0 : 10;//�ַ�ͼ��λ��x������
		unsigned __int64 y0 : 10;//�ַ�ͼ��λ��y������
		unsigned __int64 x1 : 10;//�ַ�ͼ��λ��x + width		face->glyph->bitmap.width
		unsigned __int64 y1 : 10;//�ַ�ͼ��λ��y + height		face->glyph->bitmap.rows

		unsigned __int64 offsetX : 8;//�ַ������ԭ���ˮƽλ�ã�����	bearingX	face->glyph->bitmap_left
		unsigned __int64 offsetY : 8;//�ַ������ԭ��Ĵ�ֱλ�ã�����	bearingY 	face->glyph->bitmap_top

		unsigned __int64 advance : 8;//ԭ�㵽��һ������ԭ���ˮƽ����	face->glyph->advance.x
	};

	class FreeTypeFont
	{
	public:
		FT_Library		_library;			//freetype����
		FT_Face			_face;				//�����ļ�
		unsigned int	_fontTexture;		//�洢���ɵ�����
		Character		_character[1 << 16];//һ����65535���֣���ÿ���ֶ�Ӧ��intֵ������

		TextShader		_shader;

		int				_textureWidth;		//�����
		int				_textureHeight;		//�����

		int				_xStart;			//��ǰ����д�������ϵ�Xλ������
		int				_yStart;			//��ǰ����д�������ϵ�Yλ������

		int				_fontSize;			//�����С

		int				_maxFontPixelX;		//�Ѽ����ַ����X
		int				_maxFontPixelY;		//�Ѽ����ַ����Y
		char			_curVertexBuffer[1024 * 512];//��ǰ�����ַ��Ķ��㻺��

		int				_notFoundIndex;		//�Ҳ������ַ�����ͬһ������
		bool			_isFirstNotFound;	//�Ƿ��һ�η����Ҳ����ַ�

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

		//�����ָ�����ⲿ�ͷ�
		glm::vec2 drawText(int width, int height, int x, int y, int z, wchar_t* text, size_t len, glm::vec3 color, FontVertex** vertexs, size_t*vertexLength = 0);

	private:
		Character getCharacter(int ch);
	};
}

#endif // !_Free_Type_Font_