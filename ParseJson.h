#ifndef PARSEJSON_H
#define PARSEJSON_H

//JSON — это формат хранения структурированных данных с шестью основными типами данных:
//bool、double、string、array、object、null
//QJsonDocument в основном модуле

//ссылки:
//http://blog.sina.com.cn/s/blog_a6fb6cc90101gnxm.html
//https://www.cnblogs.com/lifan3a/articles/7811434.html
//https://doc.qt.io/qt-5/json.html

#include <QObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

class ParseJson : public QObject
{
    Q_OBJECT
public:
    ParseJson(QObject *parent=nullptr);
    // Загрузить файл Json
    bool loadJson(const QString &filepath);
    // Экспортировать файл Json
    bool dumpJson(const QString &filepath);

private:
    // Анализ объекта при загрузке Json
    void parseObject(const QJsonObject& obj);
    // Разобрать массив при загрузке Json
    void parseArray(const QJsonArray& arr);
    // Значение анализа при загрузке Json
    void parseValue(const QJsonValue& val);

};

#endif // PARSEJSON_H
