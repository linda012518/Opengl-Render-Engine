#include "OpenglWidget.h"
#include "RenderThread.h"
#include "Scene.h"
#include "Camera.h"
#include "mathf\CELLMath.hpp"

#include "qevent.h"

OpenglWidget::OpenglWidget(QWidget* parent) : QWidget(parent)
{
	setAttribute(Qt::WA_PaintOnScreen, true);//激活qt引擎的函数

	_camera = new lyd::Camera();
	if (!_camera)
	{
		_camera = nullptr;
		return;
	}

	_scene = new lyd::Scene();
	if (!_scene)
	{
		_scene = nullptr;
		return;
	}
	_scene->setCamera(_camera);
	//_scene->setViewSize(width(), height());

	_thread = new RenderThread((HWND)winId());
	_thread->setScene(_scene);
	_thread->start();

}

OpenglWidget::~OpenglWidget()
{
	if (_thread != nullptr)
	{
		delete _thread;
		_thread = nullptr;
	}
	if (_camera != nullptr)
	{
		delete _camera;
		_camera = nullptr;
	}
}

void OpenglWidget::mousePressEvent(QMouseEvent *event)
{
	this->setFocusPolicy(Qt::ClickFocus);

	_border->setStyleSheet(QLatin1String("background-color: rgb(255, 192, 32);"));
	_brotherBorder->setStyleSheet(QLatin1String("background-color: rgb(181, 181, 181);"));
}

void OpenglWidget::resizeEvent(QResizeEvent *event)
{
	float w = (float)width();
	float h = (float)height();

	if (_thread != nullptr)
		_thread->setViewSize(width(), height());

	if (_camera != nullptr)
	{
		_camera->setViewSize(w, h);
		_camera->perspective(60, w / h, 1.0f, 500.0f);
		//_camera->ortho(-width, width, -height, height, 1.0f, 500.0f);
		_camera->update();
		setSceneVP();
	}
}

QPaintEngine* OpenglWidget::paintEngine() const 
{
	return 0;//屏蔽qt自带的绘制即可
}

void OpenglWidget::setBorder(QWidget* border, QWidget* brotherBorder)
{
	_border = border;
	_brotherBorder = brotherBorder;
}

void OpenglWidget::setEye(lyd::float3 pos)
{
	if( _camera != nullptr)
		_camera->setEye(pos);
}
void OpenglWidget::setTarget(lyd::float3 pos)
{
	if (_camera != nullptr)
		_camera->setTarget(pos);
}
void OpenglWidget::setRight(lyd::float3 dir)
{
	if (_camera != nullptr)
		_camera->setRight(dir);
}
void OpenglWidget::setUp(lyd::float3 dir)
{
	if (_camera != nullptr)
		_camera->setUp(dir);
}
void OpenglWidget::updateCamera()
{
	if (_camera != nullptr)
		_camera->update();
}
void OpenglWidget::setViewSize(float width, float height)
{
	if (_camera != nullptr)
		_camera->setViewSize(width, height);
}
void OpenglWidget::perspective(float fovy, float aspect, float zNear, float zFar)
{
	if (_camera != nullptr)
		_camera->perspective(fovy, aspect, zNear, zFar);
}
void OpenglWidget::ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	if (_camera != nullptr)
		_camera->ortho(left, right, bottom, top, zNear, zFar);
}

void OpenglWidget::setSceneVP()
{
	if (!_scene) return;
	if (!_camera) return;
	_scene->setVP(_camera->getView(), _camera->getProject());
}