#include "sceneeditor.h"

SceneEditor::SceneEditor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.centralWidget->init(ui._editLyaout, ui._previewLayout, ui._line);
}
