#include "CenterWindow.h"

#include "qevent.h"

#include "SplitWindowLine.h"
#include "OpenglWidget.h"
#include "Scene.h"

CenterWindow::CenterWindow(QWidget* parent)
{
}

CenterWindow::~CenterWindow()
{
}

void CenterWindow::init(QWidget* edit, QWidget* preview, SplitWindowLine* line)
{
	_edit			=	edit;
	_preview		=	preview;
	_line			=	line;

	OpenglWidget* e	=	_edit->findChild<OpenglWidget*>("_edit");
	if (e)
	{
		e->setBorder(_edit, _preview);
	}
	OpenglWidget* p =	_preview->findChild<OpenglWidget*>("_preview");
	if (p)
	{
		p->setBorder(_preview, _edit);
	}

	float height	=	this->height();
	float width		=	this->width();
	float lineWidth =	_line->width();
	float halfWidth =	(width - lineWidth) / 2.0f;

	_edit->setGeometry(		0,						0,	halfWidth,	height);
	_line->setGeometry(		halfWidth,				0,	lineWidth,	height);
	_preview->setGeometry(	halfWidth + lineWidth,	0,	halfWidth,	height);
}

void CenterWindow::resizeEvent(QResizeEvent *event)
{
	float x = _line->pos().x();
	float width = event->size().width();
	float height = event->size().height();
	float lineWidth = _line->width();
	float rate = x / (_edit->width() + _line->width() + _preview->width());

	float editWidth = rate * width;

	_edit->setGeometry(0, 0, editWidth, height);

	_line->setGeometry(editWidth, 0, lineWidth, height);

	_preview->setGeometry(editWidth + lineWidth, 0, width - editWidth - lineWidth, height);
}

void CenterWindow::mouseMoveEvent(QMouseEvent *event)
{
	if (_line->dragLine())
	{
		float width		=	this->width();
		float height	=	this->height();
		float lineWidth =	_line->width();
		float rate		=	event->pos().x() / width;

		float editWidth =	rate * width;

		_edit->setGeometry(0, 0, editWidth, height);

		_line->setGeometry(editWidth, 0, lineWidth, height);

		_preview->setGeometry(editWidth + lineWidth, 0, width - editWidth - lineWidth, height);
	}
}
