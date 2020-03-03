#include "RenderThread.h"
#include "Scene.h"
#include "GameObject.h"
#include "CameraMesh.h"
#include "Renderable.h"
#include "Material.h"
#include "DefaultMaterial.h"
#include "Transform.h"
//#include "mathf\CELLMath.hpp"
#include "qtimer.h"

RenderThread::RenderThread(HWND hwnd)
{
	_hwnd			=	hwnd;

	_timer			=	new QTimer();
	if (!_timer)
	{
		_timer = nullptr;
		return;
	}
	_timer->moveToThread(this);
	_timer->setInterval(20);
	QObject::connect(this, &RenderThread::started, _timer, static_cast<void (QTimer::*)(void)>(&QTimer::start));
	QObject::connect(_timer, &QTimer::timeout, this, &RenderThread::render, Qt::DirectConnection);
}

RenderThread::~RenderThread()
{
	_isExit = true;
	wait();
	if (_timer != nullptr)
	{
		delete _timer;
		_timer = nullptr;
	}
	if (_scene != nullptr)
	{
		delete _scene;
		_scene = nullptr;
	}
}

lyd::Scene* RenderThread::getScene() const
{
	return _scene;
}

void RenderThread::setScene(lyd::Scene* scene)
{
	_scene = scene;
}

void RenderThread::push(CmdQueue cmd)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_cmdQueue.push_back(cmd);
}

void RenderThread::run()
{
	_context.setup(_hwnd, GetDC(_hwnd));
	_context.makeCurrent();

	_scene->init();

	glEnable(GL_DEPTH_TEST);

	//_scene = new lyd::Scene();
	//if (!_scene)
	//{
	//	_scene = nullptr;
	//	return;
	//}
	//_scene->setViewSize(_width, _height);

	//_timer = new QTimer();
	//if (!_timer)
	//{
	//	_timer = nullptr;
	//	return;
	//}
	//QObject::connect(_timer, &QTimer::timeout, this, &RenderThread::render, Qt::DirectConnection);
	//QObject::connect(this, &RenderThread::deleteLater, _timer, &QTimer::stop, Qt::DirectConnection);
	//_timer->setInterval(20);
	//_timer->start();
	//_timer->moveToThread(this);

	exec();
	return;
}

void RenderThread::render()
{
	if (_isExit)
	{
		_timer->stop();
		exit();
		return;
	}
	if (_cmdQueue.size() > 0)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		CmdQueue cmd = _cmdQueue.front();
		_cmdQueue.pop_front();
		//TODO
		if (cmd == CmdQueue::AddCamera)
		{
			lyd::GameObject* obj = _scene->addGameObject<lyd::CameraMesh, lyd::DefaultMaterial>("Camera");
			obj->getRenderable()->getMaterial()->setColor(0.08f, 0.56f, 0.67f, 1);
			lyd::CameraMesh* mesh = (lyd::CameraMesh*)obj->getMesh();
			obj->getTransform()->setScale(0.5f);
			obj->getTransform()->setPosition(lyd::float3(0, 2, -20));

			mesh->setEye(obj->getTransform()->getPosition());
			mesh->setTarget(obj->getTransform()->getPosition() + lyd::float3(0, 0, 100));
			mesh->setRight(lyd::float3(1, 0, 0));
			mesh->setUp(lyd::float3(0, 1, 0));
			mesh->setViewSize(_previewWidth, _previewHeight);
			mesh->perspective(60, (float)_previewWidth / (float)_previewHeight, 1.0f, 100.0f);
			mesh->setFrustumVertex(mesh->computeFrustumVertices());
			_mesh = mesh;
			if (_callback)
			{
				_callback(_mesh);
			}
		}
	}

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glViewport(0, 0, _width, _height);

	if (_scene != nullptr)
		_scene->render();
	_context.swapBuffer();
}

void RenderThread::setViewSize(int width, int height)
{
	_width	= width;
	_height = height;
}

void RenderThread::setPreviewViewSize(int width, int height)
{
	_previewWidth	= width;
	_previewHeight	= height;

	if (_mesh != nullptr)
	{
		_mesh->setViewSize(_previewWidth, _previewHeight);
		_mesh->perspective(60, (float)_previewWidth / (float)_previewHeight, 1.0f, 100.0f);
		_mesh->setFrustumVertex(_mesh->computeFrustumVertices());
	}
}

void RenderThread::setCallback(callback cb)
{
	_callback = cb;
}