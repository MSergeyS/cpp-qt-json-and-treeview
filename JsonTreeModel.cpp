#include "JsonTreeModel.h"

#include <QFile>
#include <QFileInfo>
#include <QDebug>

const QString HEADERS = "[Headers]";
const QString ROOT = "[Root]";

JsonTreeModel::JsonTreeModel(QObject *parent)
    : QAbstractItemModel(parent),theRootItem(new JsonTreeItem())
{

}

JsonTreeModel::~JsonTreeModel()
{
    delete theRootItem;
}

bool JsonTreeModel::loadTxt(const QByteArray& raw_data)
{
    QStringList lines = QString(raw_data).split(QString("\n"));

    // Создаем заголовки столбцов:
    // пробуем считать из файла
    QString header_string = lines.at(0);
    QStringList headers = lines.at(0).split(QRegularExpression("[:,]"), Qt::SkipEmptyParts);

    // Создаем корневой элемент и добавляем заголовки
    addHeaders(headers);
    addTreeItemEmpty(ROOT, headers.size(), theRootItem);

    // исключаем 2 строки заголовка из текста
    uint shift_data = raw_data.size() - 
                (header_string.toUtf8().size() + lines.at(1).toUtf8().size() + 2);

    // Разобрать объект в документе
    parseTxt(raw_data.right(shift_data), theRootItem->childItem(0));

    endResetModel();

    qDebug()<<"load txt file";
    return true;
}

bool JsonTreeModel::loadJson(const QByteArray& raw_data)
{
    // Разобрать документ в формате Json
    QJsonParseError json_error;
    QJsonDocument json_doc = QJsonDocument::fromJson(raw_data, &json_error);

    // Следует ли нормально анализировать данные Json
    if (json_doc.isNull() || json_doc.isEmpty() || json_error.error != QJsonParseError::NoError)
        return false;

    beginResetModel();
    // Очистить предыдущую модель
    delete theRootItem;
    // Создаем корневой элемент
    theRootItem = new JsonTreeItem();
    theRootItem->setType(JsonTreeItem::Object);

    // разбирае заголовоки
    QJsonDocument json_header;
    if (!json_doc[HEADERS].isUndefined()) {
        json_header = QJsonDocument(json_doc[HEADERS].toArray());
        if (json_header.isArray()) {
            // Разобрать массив в документе
            parseArray(HEADERS, json_header.array(), theRootItem);
        }
        theRootItem = theRootItem->childItem(0);

        // Создаем заголовки столбцов:
        QStringList headers;
        for (int inx = 0; inx < theRootItem->childCount(); inx++) {
            headers << theRootItem->childItem(inx)->data(1).toString();
        }

        // Добавляем заголовки
        addHeaders(headers);
    }
    // корень дерева
    if (!json_doc[ROOT].isUndefined()) {
        json_doc = QJsonDocument(json_doc[ROOT].toObject());
    }

    // Определите, является ли это объектом {} или массивом [] Json.
    if (json_doc.isObject()) {
        // Разобрать объект в документе
        parseObject(ROOT, json_doc.object(), theRootItem);
    }
    else if (json_doc.isArray()) {
        // Разобрать массив в документе
        parseArray(ROOT, json_doc.array(), theRootItem);
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

    // Записываем заголовоки столбцов
    QHash<int, QVariant> headers_data;
    headers_data.insert({ {0, HEADERS} });
    for (int inx = 0; inx < theRootItem->columnCount(); inx++) {
        headers_data.insert(inx+1, headerData(inx, Qt::Horizontal).toString());
    }

    // Создаем элемент заголовка (ветвь HEADERS)
    JsonTreeItem* headerItem = new JsonTreeItem(headers_data, JsonTreeItem::Array, theRootItem);
    theRootItem->appendChild(headerItem);

    JsonTreeItem* top_level_item = theRootItem;
    if (!top_level_item)
        return false;

    // Обход дерева для создания JsonDocument
    QJsonDocument json_doc;
    switch (top_level_item->type()) {
    case JsonTreeItem::Object:
        json_doc = QJsonDocument::fromVariant(dumpObject(top_level_item));
        break;
    case JsonTreeItem::Array:
        json_doc = QJsonDocument::fromVariant(dumpArray(top_level_item));
        break;
    default:
        break;
    }

    // удаляем ветвь HEADERS (чтобы ветви HEADERS в дереве "не накапливались")
    for (int inx = 0; inx < top_level_item->childCount(); inx++) {
        if (top_level_item->childItem(inx)->key() == HEADERS) {
            top_level_item->removeChild(inx);
        }
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

bool JsonTreeModel::loadData(const QByteArray& raw_data)
{
    // Разбираем по строкам
    QStringList lines = QString(raw_data).split(QString("\n"));
    
    beginResetModel();

    bool already_have_value = false;
    int inx_value = 1;;
    for (int inx = 0; inx < theRootItem->columnCount(); inx++) {
        if (headerData(inx, Qt::Horizontal) == "VALUE") {
            already_have_value = true;
            inx_value = inx;
            break;
        }
    }

    if (!already_have_value) {
        theRootItem->insertColumns(1, 1);
        setHeaderData(inx_value, Qt::Horizontal, "VALUE");
    }

    // Разобрать объект в документе
    parseData(raw_data, theRootItem->childItem(0));

    endResetModel();

    qDebug() << "load data file";
    return true;
}

QModelIndex JsonTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    JsonTreeItem *parentItem = getItem(parent);
    JsonTreeItem *childItem = parentItem->childItem(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

bool JsonTreeModel::findByKey(const QString key,
                              QVector<int>& indexes,
                              JsonTreeItem* item) const
{
    if (item == nullptr)
        item = theRootItem;
    bool isfind = false;
    for (int inx = 0; inx < item->childCount(); inx++) {
        if (item->childItem(inx)->key() == key) {
            indexes << inx;
            isfind = true;
            break;
        }
        isfind = findByKey(key, indexes, item->childItem(inx));       
        if (isfind) {
            indexes << inx;
            break;
        }
    }
    return isfind;
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

Qt::ItemFlags JsonTreeModel::flags(const QModelIndex& index) const
{
    if (index.column() == 1)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
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
    JsonTreeItem *parentItem = getItem(parent);
    bool success;
    beginInsertRows(parent, row, row + count - 1);
    success = parentItem->insertChildren(row, count, theRootItem->columnCount());
    endInsertRows();
    return success;
}

bool JsonTreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    JsonTreeItem *parentItem = getItem(parent);
    bool success = true;
    beginRemoveRows(parent, row, row + count - 1);
    success = parentItem->removeChildren(row, count);
    endRemoveRows();
    if (theRootItem->columnCount() == 0)
        removeRows(0, rowCount());
    return success;
}

bool JsonTreeModel::insertColumns(int position, int columns, const QModelIndex& parent) {
    bool success;
    beginInsertColumns(parent, position, position + columns - 1);
    success = theRootItem->insertColumns(position, columns);
    endInsertColumns();
    return success;
}

bool JsonTreeModel::removeColumns(int position, int columns, const QModelIndex& parent) {
    bool success;
    beginRemoveColumns(parent, position, position + columns - 1);
    success = theRootItem->removeColumns(position, columns);
    endRemoveColumns();
    if (theRootItem->columnCount() == 0)
        removeRows(0, rowCount());
    return success;
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

void JsonTreeModel::parseObject(const QString &key, const QJsonObject &obj, JsonTreeItem* item)
{
    // Узел создания объекта
    addTreeItemEmpty(key, theRootItem->columnCount(), item);

    // Пройдите ключи объекта
    const QStringList keys=obj.keys();
    for(const QString &item_key:keys){
        parseValue(item_key,obj.value(item_key), item->childItem(item->childCount() - 1));
    }
}

void JsonTreeModel::parseArray(const QString &key, const QJsonArray &arr, JsonTreeItem* item)
{  
    // Узел создания массива
    addTreeItemEmpty(key, theRootItem->columnCount(), item);

    // Траверс массива
    for(int i = 0; i < arr.count(); i++){
        parseValue("-", arr.at(i), item->childItem(item->childCount() - 1));
    }
}

void JsonTreeModel::parseValue(const QString &key, const QJsonValue &val, JsonTreeItem* item)
{
    //qDebug() << key;

    QVariant the_val;
    QHash<int, QVariant> datas;
    JsonTreeItem::JsonItemType type = JsonTreeItem::Value;
    // Переходим к следующему шагу в зависимости от типа значения
    // Если это объект или массив, продолжаем рекурсию
    // Если это значение, добавляем узел
    switch (val.type()) {
    case QJsonValue::Object:
        parseObject(key, val.toObject(), item);
        return;
        break;
    case QJsonValue::Array:
        if (theRootItem->columnCount() == 2) {
            parseArray(key, val.toArray(), item);
            return;
            break;
        }
        datas = { {0,key} };
        for (int inx = 0; inx < val.toArray().size(); inx++) {
            datas.insert({ { inx + 1, QVariant(val.toArray().at(inx).toString()) } });
        }
        type = JsonTreeItem::Array;
        break;
    case QJsonValue::Bool:
        the_val=val.toBool();
        datas = { {0,key}, {1,the_val} };
        break;
    case QJsonValue::Double:
        the_val=val.toDouble();
        datas = { {0,key}, {1,the_val} };
        break;
    case QJsonValue::String:
        the_val=val.toString();
        datas = { {0,key}, {1,the_val} };
        break;
    case QJsonValue::Null: break;
    case QJsonValue::Undefined: break;
    default: break;
    }

    // Создать дочерний узел
    JsonTreeItem *child = new JsonTreeItem(datas, type, item);
    item->appendChild(child);
}

void JsonTreeModel::parseTxt(const QString& text, JsonTreeItem* parent)
{
    QStringList lines = text.split(QString("\n"));

    // Строим дерево
    QList<JsonTreeItem*> parents;
    parents << parent;

    int coloumn_count = parent->columnCount();

    for (int number = 0; number < lines.count(); number++) {
        QString line(lines.at(number)); // очередная строка
        // line = "EP/MODE3/WORK/BOT_ITERATION             : 7"

        if (!line.isEmpty()) {
            // разбиваем строку на путь и значение (разделитель ':')
            QStringList columnStrings = line.split(":", Qt::SkipEmptyParts);
            // columnStrings = {"EP/MODE3/WORK/BOT_ITERATION             ", " 7"}

            // путь для дерева
            QStringList linePathParts = columnStrings.at(0).split('/');
            int position = linePathParts.size();
            // linePathParts = {"EP", "MODE3", "WORK", "BOT_ITERATION             "}
            // position = 4

            // значения
            QStringList parameters = columnStrings.at(1).split(',');
            QJsonArray array_value;
            for (int inx = 0; inx < parameters.size(); inx++) {
                array_value.push_back(parameters.at(inx).trimmed());
            }
            // columnStrings = {"BOT_ITERATION", " 7"}

            QJsonValue value = array_value;
            int row_item = 0;
            QString key;
            for (int level = 0; level < position - 1; level++) {
                bool is_has = false; // флаг есть параметр или ещё нет
                row_item = 1;
                key = linePathParts.at(level).trimmed();
                // name_parameter = "EP"
                // по всем детям
                for (int row = 0; row < parents.last()->childCount(); row++) {
                    JsonTreeItem* tree_item = parents.last()->childItem(row);
                    row_item = row;
                    // поиск имени равному текущему имени пути параметра
                    if ((tree_item->data(0)).toString() == key) {
                        is_has = true; // нашли
                        break;         // выходим из поиска
                    }
                }

                if (!is_has) { // если нет такого параметра добовляем в лист зависимостей (в дети)
                    QHash<int, QVariant> datas = { {0,key} };
                    for (int inx = 1; inx < coloumn_count; inx++) {
                        datas.insert(inx, "");
                    }
                    JsonTreeItem* child = new JsonTreeItem(datas, JsonTreeItem::Object, parent);
                    parents.last()->appendChild(child);
                    row_item = parents.last()->childCount() - 1;
                }

                // смещаем уровень вложения на шаг вниз
                if (parents.last()->childCount() >= 0) {
                    parents << parents.last()->childItem(row_item);
                }
            }
            // добовляем параметр в лист зависимостей (в дети)
            key = linePathParts.at(qsizetype(position) - 1).trimmed();
            if (array_value.size() == 1) {
                value = array_value.at(0);  // если значение одно, то тип будет QJsonValue
            }
            parseValue(key, value, parents.last());
            // смещаем уровень вложения вверх до корня
            while (parents.count() > 1) {
                parents.pop_back();
            }
        }
    }
}

void JsonTreeModel::parseData(const QString& text, JsonTreeItem* parent)
{
    QStringList lines = text.split(QString("\n"));

    // Строим дерево
    QList<JsonTreeItem*> parents;
    parents << parent;

    for (int number = 0; number < lines.count(); number++) {
        QString line(lines.at(number)); // очередная строка
        // line = "EP/MODE3/WORK/BOT_ITERATION             : 7"

        if (!line.isEmpty()) {
            // разбиваем строку на путь и значение (разделитель ':')
            QStringList columnStrings = line.split(":", Qt::SkipEmptyParts);
            // columnStrings = {"EP/MODE3/WORK/BOT_ITERATION             ", " 7"}

            // путь для дерева
            QStringList linePathParts = columnStrings.at(0).split('/');
            int position = linePathParts.size();
            // linePathParts = {"EP", "MODE3", "WORK", "BOT_ITERATION             "}
            // position = 4

            // значения
            QStringList parameters = columnStrings.at(1).split(',');
            QJsonArray array_value;
            for (int inx = 0; inx < parameters.size(); inx++) {
                array_value.push_back(parameters.at(inx).trimmed());
            }
            // columnStrings = {"BOT_ITERATION", " 7"}

            QJsonValue value = array_value;
            int row_item = 0;
            QString key;
            for (int level = 0; level < position; level++) {
                bool is_has = false; // флаг есть параметр или ещё нет
                row_item = 1;
                key = linePathParts.at(level).trimmed();
                // name_parameter = "EP"
                // по всем детям
                for (int row = 0; row < parents.last()->childCount(); row++) {
                    JsonTreeItem* tree_item = parents.last()->childItem(row);
                    row_item = row;
                    // поиск имени равному текущему имени пути параметра
                    if ((tree_item->data(0)).toString() == key) {
                        is_has = true; // нашли
                        break;         // выходим из поиска
                    }
                }

                if (!is_has) { // если нет такого параметра добовляем в лист зависимостей (в дети)
                    qDebug() << 'n';
                }

                // смещаем уровень вложения на шаг вниз
                if (parents.last()->childCount() >= 0) {
                    parents << parents.last()->childItem(row_item);
                }
            }
            // добовляем параметр в лист зависимостей (в дети)
            key = linePathParts.at(qsizetype(position) - 1).trimmed();
            if (array_value.size() == 1) {
                value = array_value.at(0);  // если значение одно, то тип будет QJsonValue
            }
            //qDebug() << key << " : " << value;

            parents.last()->setData(1,value);
            
            // смещаем уровень вложения вверх до корня
            while (parents.count() > 1) {
                parents.pop_back();
            }
        }
    }

}

QVariantMap JsonTreeModel::dumpObject(JsonTreeItem *&item) const
{
    QVariantMap json_obj; // QVariantMap соответствует QJsonObject
    const int child_count=item->childCount();
    for(int i = 0; i < child_count; i++) {
        JsonTreeItem *child=item->childItem(i);
        if (!child) continue;
        // Почему бы не инкапсулировать это с помощью функции, возвращающей QVariant?
        switch (child->type()) {
        case JsonTreeItem::Object:
            json_obj.insert(child->key(), dumpObject(child));
            break;
        case JsonTreeItem::Array:
            json_obj.insert(child->key(), dumpArray(child));
            break;
        case JsonTreeItem::Value:
            json_obj.insert(child->key(), dumpValue(child));
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
    const int child_count = item->childCount();
    for(int i = 0; i < child_count; i++){
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
    if (child_count == 0) {
        for (int inx = 1; inx < item->columnCount(); inx++) {
            json_arr.append(item->data(inx));
        }
    }
    return json_arr;
}

QVariant JsonTreeModel::dumpValue(JsonTreeItem *&item) const
{
    // QVariant соответствует QJsonValue
    return item->value();
}

bool JsonTreeModel::setHeaderData(int section, Qt::Orientation orientation,
                                  const QVariant& value, int role)
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return false;
    bool result = theRootItem->setData(section, value);
    if (result) {
        emit headerDataChanged(orientation, section, section);
    }
    return result;
}

QVariant JsonTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return theRootItem->data(section);
    return QVariant();
}

void JsonTreeModel::addHeaders(QStringList headers)
{
    for (int inx = 0; inx < headers.size(); inx++) {
        headers[inx] = headers[inx].trimmed();
        setHeaderData(inx, Qt::Horizontal, headers[inx], Qt::EditRole);
    }
    // если нет в файле заголовка
    if (headers.size() < 2) {
        headers << tr("Key") << tr("Value");
    }

    // заполняем заголовок таблицы
    QHash<int, QVariant> headers_data;
    for (int inx = 0; inx < headers.size(); inx++) {
        headers_data.insert(inx, headers.at(inx));
    }

    // Создаем корневой элемент
    theRootItem = new JsonTreeItem(headers_data, JsonTreeItem::Object);
}

void JsonTreeModel::addTreeItemEmpty(const QString key, const int count_сolumns, JsonTreeItem *item)
{
    // Создаем узел
    QHash<int, QVariant> root_data;
    root_data.insert({ {0, key} });
    for (int inx = 1; inx < count_сolumns; inx++) {
        root_data.insert(inx, "");
    }

    JsonTreeItem* rootItem = new JsonTreeItem(root_data, JsonTreeItem::Object, item);
    item->appendChild(rootItem);
}
