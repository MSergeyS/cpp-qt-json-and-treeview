#ifndef JSON2TREE_H
#define JSON2TREE_H

#include <QDialog>

//#include "ParseJson.h"
#include "JsonTreeModel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class JsonToTree;
}
QT_END_NAMESPACE

class JsonToTree : public QDialog
{
    Q_OBJECT

public:
    explicit JsonToTree(QWidget *parent = nullptr);
    ~JsonToTree();

private:
    void initLoadDump();
    void initEdit();
    void updateIndex();
    void expandToKey(QString key);

private slots:
    void keyPressEvent(QKeyEvent *event); // обработка событий нажатия клавиш на клавиатуре

private:
    Ui::JsonToTree *ui;
    //ParseJson parseJson;
    JsonTreeModel *jsonModel;
    void sendMessage();
};
#endif // JSON2TREE_H
