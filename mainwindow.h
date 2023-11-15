#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>

//#include "ParseJson.h"
#include "JsonTreeModel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initLoadDump();
    void initEdit();
    void updateIndex();
    void expandToKey(QString key);

private slots:
    void keyPressEvent(QKeyEvent *event); // обработка событий нажатия клавиш на клавиатуре

private:
    Ui::MainWindow *ui;
    //ParseJson parseJson;
    JsonTreeModel *jsonModel;
    void sendMessage();
};
#endif // MAINWINDOW_H
