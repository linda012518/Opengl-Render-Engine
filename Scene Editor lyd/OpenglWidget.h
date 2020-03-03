#ifndef _Opengl_Widget_
#define _Opengl_Widget_

#include "qwidget.h"
#include "mathf\CELLMath.hpp"

namespace lyd 
{
	class Scene;
	class Camera;
}

class RenderThread;

class OpenglWidget : public QWidget
{
	Q_OBJECT
protected:
	QWidget*		_border			=	nullptr;
	QWidget*		_brotherBorder	=	nullptr;
	RenderThread*	_thread			=	nullptr;
	lyd::Scene*		_scene			=	nullptr;
	lyd::Camera*	_camera			=	nullptr;
public:
	OpenglWidget(QWidget* parent = 0);
	~OpenglWidget();

	virtual void setBorder(QWidget* border, QWidget* brotherBorder);

protected:
	virtual void resizeEvent(QResizeEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);//按下

public:
	virtual void setEye(lyd::float3 pos);
	virtual void setTarget(lyd::float3 pos);
	virtual void setRight(lyd::float3 dir = lyd::float3(1, 0, 0));
	virtual void setUp(lyd::float3 dir = lyd::float3(0, 1, 0));
	virtual void updateCamera();
	virtual void setViewSize(float width, float height);
	virtual void perspective(float fovy, float aspect, float zNear, float zFar);
	virtual void ortho(float left, float right, float bottom, float top, float zNear, float zFar);

	virtual void setSceneVP();

public:
	QPaintEngine *paintEngine() const Q_DECL_OVERRIDE;//屏蔽qt自带的绘制即可

};

#endif // !_Opengl_Widget_



