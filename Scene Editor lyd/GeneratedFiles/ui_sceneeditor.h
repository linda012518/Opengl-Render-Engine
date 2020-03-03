/********************************************************************************
** Form generated from reading UI file 'sceneeditor.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCENEEDITOR_H
#define UI_SCENEEDITOR_H

#include <PreviewOpenglWidget.h>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "CenterWindow.h"
#include "EditorOpenglWidget.h"
#include "splitwindowline.h"

QT_BEGIN_NAMESPACE

class Ui_SceneEditorClass
{
public:
    CenterWindow *centralWidget;
    SplitWindowLine *_line;
    QWidget *_editLyaout;
    QGridLayout *gridLayout_6;
    EditorOpenglWidget *_edit;
    QGridLayout *gridLayout_4;
    QWidget *_previewLayout;
    QGridLayout *gridLayout_7;
    PreviewOpenglWidget *_preview;
    QGridLayout *gridLayout_5;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidget_2;
    QWidget *_treeWidget;
    QGridLayout *gridLayout_2;
    QDockWidget *dockWidget_4;
    QWidget *_matList;
    QGridLayout *gridLayout_3;
    QDockWidget *dockWidget_5;
    QWidget *dockWidgetContents;
    QDockWidget *dockWidget_6;
    QWidget *dockWidgetContents_2;

    void setupUi(QMainWindow *SceneEditorClass)
    {
        if (SceneEditorClass->objectName().isEmpty())
            SceneEditorClass->setObjectName(QStringLiteral("SceneEditorClass"));
        SceneEditorClass->resize(1117, 825);
        QIcon icon;
        icon.addFile(QStringLiteral(":/SceneEditor/C:/Users/Administrator/Desktop/logo.ico"), QSize(), QIcon::Normal, QIcon::Off);
        SceneEditorClass->setWindowIcon(icon);
        centralWidget = new CenterWindow(SceneEditorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setMinimumSize(QSize(200, 200));
        centralWidget->setStyleSheet(QStringLiteral(""));
        _line = new SplitWindowLine(centralWidget);
        _line->setObjectName(QStringLiteral("_line"));
        _line->setGeometry(QRect(307, 0, 3, 752));
        _line->setStyleSheet(QLatin1String("border:2px solid rgb(138, 138, 138);\n"
"background-color: rgb(102, 139, 109);"));
        _line->setFrameShape(QFrame::StyledPanel);
        _line->setFrameShadow(QFrame::Raised);
        _editLyaout = new QWidget(centralWidget);
        _editLyaout->setObjectName(QStringLiteral("_editLyaout"));
        _editLyaout->setGeometry(QRect(10, 59, 231, 621));
        _editLyaout->setMinimumSize(QSize(100, 100));
        _editLyaout->setStyleSheet(QStringLiteral("background-color: rgb(185, 185, 185);"));
        gridLayout_6 = new QGridLayout(_editLyaout);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        gridLayout_6->setContentsMargins(2, 2, 2, 2);
        _edit = new EditorOpenglWidget(_editLyaout);
        _edit->setObjectName(QStringLiteral("_edit"));
        _edit->setMinimumSize(QSize(100, 100));
        _edit->setStyleSheet(QLatin1String("border:2px solid rgb(181, 181, 181);\n"
"background-color: rgb(134, 117, 148);"));
        gridLayout_4 = new QGridLayout(_edit);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);

        gridLayout_6->addWidget(_edit, 0, 0, 1, 1);

        _previewLayout = new QWidget(centralWidget);
        _previewLayout->setObjectName(QStringLiteral("_previewLayout"));
        _previewLayout->setGeometry(QRect(340, 80, 321, 601));
        _previewLayout->setMinimumSize(QSize(100, 100));
        _previewLayout->setStyleSheet(QStringLiteral("background-color: rgb(185, 185, 185);"));
        gridLayout_7 = new QGridLayout(_previewLayout);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        gridLayout_7->setContentsMargins(2, 2, 2, 2);
        _preview = new PreviewOpenglWidget(_previewLayout);
        _preview->setObjectName(QStringLiteral("_preview"));
        _preview->setMinimumSize(QSize(100, 100));
        _preview->setStyleSheet(QLatin1String("border:2px solid rgb(181, 181, 181);\n"
"background-color: rgb(102, 139, 109);"));
        gridLayout_5 = new QGridLayout(_preview);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setHorizontalSpacing(7);
        gridLayout_5->setContentsMargins(0, 0, 0, 0);

        gridLayout_7->addWidget(_preview, 0, 0, 1, 1);

        SceneEditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SceneEditorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1117, 23));
        SceneEditorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SceneEditorClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        SceneEditorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(SceneEditorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SceneEditorClass->setStatusBar(statusBar);
        dockWidget_2 = new QDockWidget(SceneEditorClass);
        dockWidget_2->setObjectName(QStringLiteral("dockWidget_2"));
        dockWidget_2->setMinimumSize(QSize(200, 41));
        _treeWidget = new QWidget();
        _treeWidget->setObjectName(QStringLiteral("_treeWidget"));
        gridLayout_2 = new QGridLayout(_treeWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        dockWidget_2->setWidget(_treeWidget);
        SceneEditorClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_2);
        dockWidget_4 = new QDockWidget(SceneEditorClass);
        dockWidget_4->setObjectName(QStringLiteral("dockWidget_4"));
        dockWidget_4->setMinimumSize(QSize(200, 47));
        _matList = new QWidget();
        _matList->setObjectName(QStringLiteral("_matList"));
        gridLayout_3 = new QGridLayout(_matList);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        dockWidget_4->setWidget(_matList);
        SceneEditorClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_4);
        dockWidget_5 = new QDockWidget(SceneEditorClass);
        dockWidget_5->setObjectName(QStringLiteral("dockWidget_5"));
        dockWidget_5->setMinimumSize(QSize(200, 38));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        dockWidget_5->setWidget(dockWidgetContents);
        SceneEditorClass->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget_5);
        dockWidget_6 = new QDockWidget(SceneEditorClass);
        dockWidget_6->setObjectName(QStringLiteral("dockWidget_6"));
        dockWidget_6->setMinimumSize(QSize(200, 38));
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QStringLiteral("dockWidgetContents_2"));
        dockWidget_6->setWidget(dockWidgetContents_2);
        SceneEditorClass->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget_6);

        retranslateUi(SceneEditorClass);

        QMetaObject::connectSlotsByName(SceneEditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *SceneEditorClass)
    {
        SceneEditorClass->setWindowTitle(QApplication::translate("SceneEditorClass", "SceneEditor", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SceneEditorClass: public Ui_SceneEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCENEEDITOR_H
