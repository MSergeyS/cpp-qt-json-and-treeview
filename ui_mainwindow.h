/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *loadLayout_txt;
    QPushButton *btnLoadTxtPath;
    QLineEdit *editLoadTxtPath;
    QPushButton *btnLoadTxt;
    QHBoxLayout *loadLayout;
    QPushButton *btnLoadPath;
    QLineEdit *editLoadPath;
    QPushButton *btnLoadJson;
    QHBoxLayout *dumpLayout;
    QPushButton *btnDumpPath;
    QLineEdit *editDumpPath;
    QPushButton *btnDumpJson;
    QHBoxLayout *actionLayout;
    QPushButton *btnInsert;
    QPushButton *btnInsertChild;
    QPushButton *btnRemove;
    QPushButton *insertColumnAction;
    QPushButton *removeColumnAction;
    QSpacerItem *horizontalSpacer;
    QTreeView *treeView;
    QTextEdit *textEdit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(560, 605);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        loadLayout_txt = new QHBoxLayout();
        loadLayout_txt->setObjectName("loadLayout_txt");
        btnLoadTxtPath = new QPushButton(centralwidget);
        btnLoadTxtPath->setObjectName("btnLoadTxtPath");

        loadLayout_txt->addWidget(btnLoadTxtPath);

        editLoadTxtPath = new QLineEdit(centralwidget);
        editLoadTxtPath->setObjectName("editLoadTxtPath");

        loadLayout_txt->addWidget(editLoadTxtPath);

        btnLoadTxt = new QPushButton(centralwidget);
        btnLoadTxt->setObjectName("btnLoadTxt");

        loadLayout_txt->addWidget(btnLoadTxt);


        verticalLayout->addLayout(loadLayout_txt);

        loadLayout = new QHBoxLayout();
        loadLayout->setObjectName("loadLayout");
        btnLoadPath = new QPushButton(centralwidget);
        btnLoadPath->setObjectName("btnLoadPath");

        loadLayout->addWidget(btnLoadPath);

        editLoadPath = new QLineEdit(centralwidget);
        editLoadPath->setObjectName("editLoadPath");

        loadLayout->addWidget(editLoadPath);

        btnLoadJson = new QPushButton(centralwidget);
        btnLoadJson->setObjectName("btnLoadJson");

        loadLayout->addWidget(btnLoadJson);


        verticalLayout->addLayout(loadLayout);

        dumpLayout = new QHBoxLayout();
        dumpLayout->setObjectName("dumpLayout");
        btnDumpPath = new QPushButton(centralwidget);
        btnDumpPath->setObjectName("btnDumpPath");

        dumpLayout->addWidget(btnDumpPath);

        editDumpPath = new QLineEdit(centralwidget);
        editDumpPath->setObjectName("editDumpPath");

        dumpLayout->addWidget(editDumpPath);

        btnDumpJson = new QPushButton(centralwidget);
        btnDumpJson->setObjectName("btnDumpJson");

        dumpLayout->addWidget(btnDumpJson);


        verticalLayout->addLayout(dumpLayout);

        actionLayout = new QHBoxLayout();
        actionLayout->setObjectName("actionLayout");
        btnInsert = new QPushButton(centralwidget);
        btnInsert->setObjectName("btnInsert");

        actionLayout->addWidget(btnInsert);

        btnInsertChild = new QPushButton(centralwidget);
        btnInsertChild->setObjectName("btnInsertChild");

        actionLayout->addWidget(btnInsertChild);

        btnRemove = new QPushButton(centralwidget);
        btnRemove->setObjectName("btnRemove");

        actionLayout->addWidget(btnRemove);

        insertColumnAction = new QPushButton(centralwidget);
        insertColumnAction->setObjectName("insertColumnAction");

        actionLayout->addWidget(insertColumnAction);

        removeColumnAction = new QPushButton(centralwidget);
        removeColumnAction->setObjectName("removeColumnAction");

        actionLayout->addWidget(removeColumnAction);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        actionLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(actionLayout);

        treeView = new QTreeView(centralwidget);
        treeView->setObjectName("treeView");
        treeView->setAnimated(true);
        treeView->header()->setVisible(false);

        verticalLayout->addWidget(treeView);

        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName("textEdit");
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(0, 0, 0, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette.setBrush(QPalette::Active, QPalette::Light, brush1);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush1);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush1);
        QBrush brush2(QColor(0, 255, 0, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush1);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush1);
        QBrush brush3(QColor(255, 255, 220, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush3);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush1);
        QBrush brush4(QColor(255, 255, 255, 128));
        brush4.setStyle(Qt::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Active, QPalette::PlaceholderText, brush4);
#endif
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush1);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush4);
#endif
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush1);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush4);
#endif
        textEdit->setPalette(palette);
        textEdit->setAutoFillBackground(true);
        textEdit->setReadOnly(true);

        verticalLayout->addWidget(textEdit);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 560, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "json2tree", nullptr));
        btnLoadTxtPath->setText(QCoreApplication::translate("MainWindow", "loadpath", nullptr));
        editLoadTxtPath->setText(QCoreApplication::translate("MainWindow", "://dvl_ep.txt", nullptr));
        btnLoadTxt->setText(QCoreApplication::translate("MainWindow", "load", nullptr));
        btnLoadPath->setText(QCoreApplication::translate("MainWindow", "loadpath", nullptr));
        editLoadPath->setText(QCoreApplication::translate("MainWindow", "://dvl_params_list.txt", nullptr));
        btnLoadJson->setText(QCoreApplication::translate("MainWindow", "load", nullptr));
        btnDumpPath->setText(QCoreApplication::translate("MainWindow", "dumppath", nullptr));
        editDumpPath->setText(QCoreApplication::translate("MainWindow", "://dump_data.json", nullptr));
        btnDumpJson->setText(QCoreApplication::translate("MainWindow", "dump", nullptr));
        btnInsert->setText(QCoreApplication::translate("MainWindow", "Insert", nullptr));
        btnInsertChild->setText(QCoreApplication::translate("MainWindow", "InsertChild", nullptr));
        btnRemove->setText(QCoreApplication::translate("MainWindow", "Remove", nullptr));
        insertColumnAction->setText(QCoreApplication::translate("MainWindow", "+ \321\201\321\202\320\276\320\261\320\265\321\206", nullptr));
        removeColumnAction->setText(QCoreApplication::translate("MainWindow", "- \321\201\321\202\320\276\320\261\320\265\321\206", nullptr));
        textEdit->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'MS Shell Dlg 2'; font-size:8.25pt;\"><br /></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
