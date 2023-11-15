#ifndef JSONTREEMODEL_H
#define JSONTREEMODEL_H

#include <QAbstractItemModel>

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QVariant>

#include "JsonTreeItem.h"

class JsonTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit JsonTreeModel(QObject *parent = nullptr);
    ~JsonTreeModel();

    // Импорт и экспорт данных
    bool loadTxt(const QByteArray& raw_data);
    bool loadJson(const QByteArray& raw_data);
    bool dumpJson(const QString &filepath);

    bool loadData(const QByteArray& raw_data);
    
    // Отображение данных дерева
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Поддержка редактирования
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    bool insertRows(int row, int count,
                    const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count,
                    const QModelIndex &parent = QModelIndex()) override;

    bool insertColumns(int position,
                       int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int position,
                       int columns,
                       const QModelIndex &parent = QModelIndex()) override;

    // выдаёт заголовок столбца
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // установить данные заголовка столбца
    bool setHeaderData(int section,
                       Qt::Orientation orientation,
                       const QVariant& value,
                       int role = Qt::DisplayRole) override;

    void addHeaders(QStringList headers);
    void addTreeItemEmpty(const QString key, const int count_сolumns, JsonTreeItem *);
    bool findByKey(const QString key,
                   QVector<int>& indexes,
                   JsonTreeItem *item = nullptr) const;

private:
    JsonTreeItem *getItem(const QModelIndex &index) const;
    // Разбираем json-файл и генерируем дерево
    void parseObject(const QString &key,const QJsonObject& obj,JsonTreeItem* item);
    void parseArray(const QString &key,const QJsonArray& arr,JsonTreeItem* item);
    void parseValue(const QString &key,const QJsonValue& val,JsonTreeItem* item);
    void parseTxt(const QString& data, JsonTreeItem* item);
    void parseData(const QString& text, JsonTreeItem* parent);

    // Создаем узел JSON
    QVariantMap dumpObject(JsonTreeItem *&item) const;
    QVariantList dumpArray(JsonTreeItem *&item) const;
    QVariant dumpValue(JsonTreeItem *&item) const;

private:
    JsonTreeItem* theRootItem;
};

#endif // JSONTREEMODEL_H
