#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_sceneeditor.h"

class SceneEditor : public QMainWindow
{
    Q_OBJECT

public:
    SceneEditor(QWidget *parent = Q_NULLPTR);

private:
    Ui::SceneEditorClass ui;
};
