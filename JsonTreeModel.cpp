#include "JsonTreeModel.h"
#include "TxtTreeModel.h"

#include <QFile>
#include <QDebug>

JsonTreeModel::JsonTreeModel(QObject *parent)
    : QAbstractItemModel(parent),theRootItem(new JsonTreeItem(nullptr))
{

}

JsonTreeModel::~JsonTreeModel()
{
    delete theRootItem;
}

bool JsonTreeModel::loadTxt(const QString &filepath)
{
    // Определить путь и нормально ли он открывается
    if(filepath.isEmpty())
        return false;
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        return false;

    // Закрыть файл после чтения данных
    const QByteArray raw_data=file.readAll();
    file.close();

    beginResetModel();
    theRootItem->deleteAllChild(); // Очистить предыдущую модель

    // Создаем заголовки столбцов:
    QStringList headers;
    headers << tr("Заголовок") << tr("Описание");
    // Загружаем данные в модель:
    TreeModel *model = new TreeModel(headers, raw_data);

    endResetModel();

    qDebug()<<"load txt file";
    return true;
}

bool JsonTreeModel::loadJson(const QString &filepath)
{
    // Определить путь и нормально ли он открывается
    if(filepath.isEmpty())
        return false;
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        return false;

    // Закрыть файл после чтения данных
    const QByteArray raw_data=file.readAll();
    file.close();

    // Разобрать документ в формате Json
    QJsonParseError json_error;
    QJsonDocument json_doc=QJsonDocument::fromJson(raw_data,&json_error);

    // Следует ли нормально анализировать данные Json
    if(json_doc.isNull()||json_doc.isEmpty()||json_error.error!=QJsonParseError::NoError)
        return false;

    beginResetModel();
    theRootItem->deleteAllChild(); // Очистить предыдущую модель

    // Определите, является ли это объектом {} или массивом [] Json.
    if(json_doc.isObject()){
        // Разобрать объект в документе
        parseObject("[Root]",json_doc.object(),theRootItem);
    }else if(json_doc.isArray()){
        // Разобрать массив в документе
        parseArray("[Root]",json_doc.array(),theRootItem);
    }
    endResetModel();

    qDebug()<<"load json file";
    return true;
}

bool JsonTreeModel::dumpJson(const QString &filepath)
{
    if(filepath.isEmpty())
        return false;

    // У корня не более одного дочернего элемента
    if(!theRootItem||theRootItem->childCount()==0)
        return false;
    JsonTreeItem *top_level_item=theRootItem->childItem(0);
    if(!top_level_item)
        return false;

    // Обход дерева для создания JsonDocument
    QJsonDocument json_doc;
    switch (top_level_item->type()) {
    case JsonTreeItem::Object:
        json_doc=QJsonDocument::fromVariant(dumpObject(top_level_item));
        break;
    case JsonTreeItem::Array:
        json_doc=QJsonDocument::fromVariant(dumpArray(top_level_item));
        break;
    default:
        break;
    }

    // Экспорт json
    QFile file(filepath);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
        return false;
    // Преобразовать в байтовый массив, Compact не имеет разрывов строк, Indented имеет разрывы строк и более читабелен.
    file.write(json_doc.toJson(QJsonDocument::Indented));
    file.close();

    qDebug()<<"dump json file";
    return true;
}

QModelIndex JsonTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    JsonTreeItem *parentItem=getItem(parent);
    JsonTreeItem *childItem = parentItem->childItem(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex JsonTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    JsonTreeItem *childItem = getItem(index);
    JsonTreeItem *parentItem = childItem->parentItem();

    if (parentItem == theRootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int JsonTreeModel::rowCount(const QModelIndex &parent) const
{
    JsonTreeItem *parentItem = getItem(parent);
    return parentItem->childCount();
}

int JsonTreeModel::columnCount(const QModelIndex &parent) const
{
    JsonTreeItem *parentItem = getItem(parent);
    return parentItem->columnCount();
}

QVariant JsonTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    JsonTreeItem *item = getItem(index);
    return item->data(index.column());
}

Qt::ItemFlags JsonTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    JsonTreeItem *item = getItem(index);
    return (item->editable(index.column())?Qt::ItemIsEditable:Qt::NoItemFlags)
            |QAbstractItemModel::flags(index);
}

bool JsonTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    JsonTreeItem *item = getItem(index);
    item->setData(index.column(), value);
    emit dataChanged(index, index, {role});

    return true;
}

bool JsonTreeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    JsonTreeItem *parentItem=getItem(parent);

    beginInsertRows(parent, row, row+count-1);
    const bool result=parentItem->insertChildren(row,count);
    endInsertRows();

    return result;
}

bool JsonTreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    JsonTreeItem *parentItem=getItem(parent);

    beginRemoveRows(parent, row, row+count-1);
    const bool result=parentItem->removeChildren(row,count);
    endRemoveRows();

    return result;
}

JsonTreeItem *JsonTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        JsonTreeItem *item = static_cast<JsonTreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return theRootItem;
}

void JsonTreeModel::parseObject(const QString &key, const QJsonObject &obj, JsonTreeItem *&item)
{
    // Узел создания объекта
    JsonTreeItem *child=new JsonTreeItem({{0,key},{1,""}},JsonTreeItem::Object,item);
    item->appendChild(child);

    // Пройдите ключи объекта
    const QStringList keys=obj.keys();
    for(const QString &item_key:keys){
        //qDebug()<<"key:"<<item_key;
        parseValue(item_key,obj.value(item_key),child);
    }
}

void JsonTreeModel::parseArray(const QString &key, const QJsonArray &arr, JsonTreeItem *&item)
{
    // Узел создания массива
    JsonTreeItem *child=new JsonTreeItem({{0,key},{1,""}},JsonTreeItem::Array,item);
    item->appendChild(child);

    // Траверс массива
    for(int i=0;i<arr.count();i++){
        parseValue("-",arr.at(i),child);
    }
}

void JsonTreeModel::parseValue(const QString &key, const QJsonValue &val, JsonTreeItem *&item)
{
    QVariant the_val;
    // Переходим к следующему шагу в зависимости от типа значения
    // Если это объект или массив, продолжаем рекурсию
    // Если это значение, добавляем узел
    switch (val.type()) {
    case QJsonValue::Object:
        parseObject(key,val.toObject(),item);
        return;
        break;
    case QJsonValue::Array:
        parseArray(key,val.toArray(),item);
        return;
        break;
    case QJsonValue::Bool:
        the_val=val.toBool();
        break;
    case QJsonValue::Double:
        the_val=val.toDouble();
        break;
    case QJsonValue::String:
        the_val=val.toString();
        break;
    case QJsonValue::Null: break;
    case QJsonValue::Undefined: break;
    default: break;
    }

    // Создать дочерний узел
    JsonTreeItem *child=new JsonTreeItem({{0,key},{1,the_val}},JsonTreeItem::Value,item);
    item->appendChild(child);
}

QVariantMap JsonTreeModel::dumpObject(JsonTreeItem *&item) const
{
    QVariantMap json_obj; // QVariantMap соответствует QJsonObject
    const int child_count=item->childCount();
    for(int i=0;i<child_count;i++){
        JsonTreeItem *child=item->childItem(i);
        if(!child) continue;
        // Почему бы не инкапсулировать это с помощью функции, возвращающей QVariant?
        switch (child->type()) {
        case JsonTreeItem::Object:
            json_obj.insert(child->key(),dumpObject(child));
            break;
        case JsonTreeItem::Array:
            json_obj.insert(child->key(),dumpArray(child));
            break;
        case JsonTreeItem::Value:
            json_obj.insert(child->key(),dumpValue(child));
            break;
        default:
            break;
        }
    }
    return json_obj;
}

QVariantList JsonTreeModel::dumpArray(JsonTreeItem *&item) const
{
    QVariantList json_arr; // QVariantList соответствует QJsonArray
    const int child_count=item->childCount();
    for(int i=0;i<child_count;i++){
        JsonTreeItem *child=item->childItem(i);
        if(!child) continue;
        switch (child->type()) {
        case JsonTreeItem::Object:
            json_arr.append(dumpObject(child));
            break;
        case JsonTreeItem::Array:
            json_arr.append(dumpArray(child));
            break;
        case JsonTreeItem::Value:
            json_arr.append(dumpValue(child));
            break;
        default:
            break;
        }
    }
    return json_arr;
}

QVariant JsonTreeModel::dumpValue(JsonTreeItem *&item) const
{
    // QVariant соответствует QJsonValue
    return item->value();
}
