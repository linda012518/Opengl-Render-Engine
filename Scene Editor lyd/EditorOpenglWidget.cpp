#include "EditorOpenglWidget.h"
#include "Scene.h"
#include "Camera.h"
#include "CameraMesh.h"
#include "RenderThread.h"
#include "PreviewOpenglWidget.h"
#include "mathf\CELLMath.hpp"

#include "qevent.h"

EditorOpenglWidget::EditorOpenglWidget(QWidget* parent) : OpenglWidget(parent)
{
	_leftBtnFlag	= false;
	_rightBtnFlag	= false;
	_midBtnFlag		= false;
	_altBtnFlag		= false;

	setEye(lyd::float3(100, 100, 100));
	setTarget(lyd::float3(0, 0, 0));
	setRight(lyd::float3(1, 0, 0));
	setUp(lyd::float3(0, 1, 0));

	setViewSize(width(), height());
	perspective(60, width() / height(), 1.0f, 500.0f);

	updateCamera();
	setSceneVP();

	_thread->setCallback([this](void* mesh) 
		{ 
			_mesh = (lyd::CameraMesh*)mesh; 

			if (_preview && _mesh)
			{
				_preview->setEye(_mesh->getEye());
				_preview->setTarget(_mesh->getTarget());
				_preview->setRight(_mesh->getRight());
				_preview->setUp(_mesh->getUp());
				_preview->perspective(60, (float)_preview->width() / (float)_preview->height(), 1.0f, 500.0f);
				_preview->updateCamera();
				_preview->setSceneVP();
			}
		}
	);
	_thread->push(CmdQueue::AddCamera);

}

EditorOpenglWidget::~EditorOpenglWidget()
{
}

void EditorOpenglWidget::setBorder(QWidget* border, QWidget* brotherBorder)
{
	OpenglWidget::setBorder(border, brotherBorder);
	_preview = brotherBorder->findChild<PreviewOpenglWidget*>("_preview");
}

void EditorOpenglWidget::resizeEvent(QResizeEvent *event)
{
	OpenglWidget::resizeEvent(event);
	if (_preview != nullptr)
	{
		_thread->setPreviewViewSize(_preview->width(), _preview->height());
	}
	
}

void EditorOpenglWidget::mousePressEvent(QMouseEvent *event)
{
	OpenglWidget::mousePressEvent(event);

	switch (event->button())
	{
	case Qt::LeftButton:
	{
		_leftBtnFlag = true;
		_mousePos = QPoint(event->x(), event->y());
	}
	break;
	case Qt::RightButton:
	{
		_rightBtnFlag = true;
		_mousePos = QPoint(event->x(), event->y());

		_mouseRot = _camera->getMousePos(lyd::int2(event->x(), event->y()));
	}
	break;
	case Qt::MiddleButton:
	{
		_midBtnFlag = true;
		_mousePos = QPoint(event->x(), event->y());
	}
	break;
	}
}

void EditorOpenglWidget::mouseReleaseEvent(QMouseEvent *event)
{
	switch (event->button())
	{
	case Qt::LeftButton:
	{
		_leftBtnFlag = false;
	}
	break;
	case Qt::RightButton:
	{
		_rightBtnFlag = false;
	}
	break;
	case Qt::MiddleButton:
	{
		_midBtnFlag = false;
	}
	break;
	}
}

void EditorOpenglWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (_rightBtnFlag)
	{
		if (_camera != nullptr)
		{
			_camera->cameraRotateXYByPos(lyd::int2(event->x(), event->y()) - lyd::int2(_mousePos.x(), _mousePos.y()), _mouseRot);
			setSceneVP();
			_mousePos = QPoint(event->x(), event->y());
		}
	}
	else if (_midBtnFlag)
	{
		if (_camera != nullptr)
		{
			_camera->cameraMove(lyd::int2(_mousePos.x(), _mousePos.y()), lyd::int2(event->x(), event->y()));
			setSceneVP();
			_mousePos = QPoint(event->x(), event->y());
		}
	}
}

void EditorOpenglWidget::wheelEvent(QWheelEvent *event)
{
	if (_camera != nullptr)
	{
		_camera->updateCameraByPos(lyd::int2(event->x(), event->y()), event->delta());
		setSceneVP();
	}
}

void EditorOpenglWidget::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_Alt:
	{
		_altBtnFlag = true;
	}
	break;
	}
}

void EditorOpenglWidget::keyReleaseEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_Alt:
	{
		_altBtnFlag = false;
	}
	break;
	}
}