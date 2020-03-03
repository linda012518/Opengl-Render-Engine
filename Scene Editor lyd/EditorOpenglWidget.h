#ifndef _Editor_Opengl_Widget_
#define _Editor_Opengl_Widget_

#include "OpenglWidget.h"
#include "mathf\CELLMath.hpp"

namespace lyd 
{
	class CameraMesh;
}

class PreviewOpenglWidget;

class EditorOpenglWidget : public OpenglWidget
{
protected:
	lyd::CameraMesh*		_mesh		= nullptr;
	PreviewOpenglWidget*	_preview	= nullptr;
	bool					_leftBtnFlag;
	bool					_rightBtnFlag;
	bool					_midBtnFlag;
	bool					_altBtnFlag;
	QPoint					_mousePos;
	lyd::float3             _mouseRot;

public:
	EditorOpenglWidget(QWidget* parent = 0);
	virtual ~EditorOpenglWidget();

	virtual void setBorder(QWidget* border, QWidget* brotherBorder);

protected:
	virtual void resizeEvent(QResizeEvent *event);

	virtual void mousePressEvent(QMouseEvent *event);//����
	virtual void mouseReleaseEvent(QMouseEvent *event);//̧��
	virtual void mouseMoveEvent(QMouseEvent *event);//�ƶ�
	virtual void wheelEvent(QWheelEvent *event);//��

	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);
};

#endif // !_Editor_Opengl_Widget_



