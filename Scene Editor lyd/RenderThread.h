#ifndef _Render_Thread_
#define _Render_Thread_

#include "qthread.h"
#include "GLContext.h"

#include <list>
#include <mutex>
#include <functional>

enum CmdQueue 
{
	AddObj,
	AddCamera,
	DeleteObj,
	MoveObj,
	ScaleObj,
};

namespace lyd
{
	class Scene;
	class GLContext;
	class CameraMesh;
}

class QTimer;

typedef std::function<void(void*)> callback;

class RenderThread : public QThread
{
public:
	RenderThread(HWND hwnd);
	virtual ~RenderThread();

	virtual void run();

	void render();

	void setScene(lyd::Scene* scene);

	lyd::Scene* getScene() const;

	void setViewSize(int width, int height);
	void setPreviewViewSize(int width, int height);

	void push(CmdQueue cmd);

	void setCallback(callback cb);

private:
	std::list<CmdQueue>		_cmdQueue;
	std::mutex				_mutex;
	lyd::GLContext			_context;
	callback				_callback;
	lyd::Scene*				_scene			=	nullptr;
	lyd::CameraMesh*		_mesh			=	nullptr;
	QTimer*					_timer			=	nullptr;
	HWND					_hwnd;
	int						_width			=	0;
	int						_height			=	0;
	int						_previewWidth	=	0;
	int						_previewHeight	=	0;
	bool					_isExit			=	false;
};

#endif // !_Render_Thread_


