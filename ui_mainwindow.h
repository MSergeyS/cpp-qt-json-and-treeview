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
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(630, 467);
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

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 630, 21));
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
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
