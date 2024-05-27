#include <QApplication>
#include "json2tree.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    JsonToTree w;
    w.show();
    return a.exec();
}
