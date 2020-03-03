#ifndef _Center_Window_
#define _Center_Window_

#include "qwidget.h"

class SplitWindowLine;

class CenterWindow : public QWidget
{
	Q_OBJECT

private:
	QWidget*			_edit		=	nullptr;
	QWidget*			_preview	=	nullptr;
	SplitWindowLine*	_line		=	nullptr;

public:
	CenterWindow(QWidget* parent = 0);
	virtual ~CenterWindow();

	void init(QWidget* edit, QWidget* preview, SplitWindowLine* line);

protected:
	virtual void resizeEvent(QResizeEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
};



#endif // !_Center_Window_


