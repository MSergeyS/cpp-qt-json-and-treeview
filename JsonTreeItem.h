#ifndef JSONTREEITEM_H
#define JSONTREEITEM_H

#include <QList>
#include <QVariant>

// Для нескольких структур вы можете написать абстрактный родительский класс элемента
// Мой элемент относительно прост и в основном устанавливается извне
class JsonTreeItem
{
public:
    //Тип элемента: None — недопустимый тип
    enum JsonItemType{
        None,
        Object,
        Array,
        Value
    };
public:
    explicit JsonTreeItem(JsonTreeItem *parent=nullptr);
    explicit JsonTreeItem(const QHash<int,QVariant> &datas,JsonTreeItem::JsonItemType type,JsonTreeItem *parent=nullptr);
    ~JsonTreeItem();

    // операции добавления и удаления
    bool insertChild(int row,JsonTreeItem *child);
    bool removeChild(int row);
    bool insertChildren(int row,int count);
    bool removeChildren(int row,int count);
    void appendChild(JsonTreeItem *child);
    void deleteAllChild();

    // Используется для структуры модели
    JsonTreeItem *childItem(int row);
    JsonTreeItem *parentItem();
    void setParentItem(JsonTreeItem *parent);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    void setData(int column,const QVariant &val);
    int row() const;
    bool editable(int column) const;
    //QHash<int,QByteArray> roleNames() const;

    // Удобно для работы с Json
    QString key() const;
    void setKey(const QString &key);
    QVariant value() const;
    void setValue(const QVariant &value);
    JsonTreeItem::JsonItemType type() const;
    void setType(JsonTreeItem::JsonItemType type);

private:
    JsonTreeItem *theParentItem;         // Конструктор для инициализации родителя
    QList<JsonTreeItem*> theChildItems;  // чтобы добавить дочерний элемент
    JsonItemType theItemType;
    //хешируем содержимое
    //1: ключ
    //2: значение
    //Цель использования хеша — облегчить расширение, например добавление проверочных столбцов и т. д.
    //Если есть только две простые строки данных ключ+значение, просто используйте QString+QVariant.
    QHash<int, QVariant> theItemDatas;    // Сохраните содержимое каждого столбца (дерево qml использует роли)
};

#endif // JSONTREEITEM_H
