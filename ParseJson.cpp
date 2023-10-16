#include "ParseJson.h"

#include <QFile>
#include <QDebug>

ParseJson::ParseJson(QObject *parent)
    :QObject(parent)
{

}

bool ParseJson::loadJson(const QString &filepath)
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

    // Определите, является ли это объектом {} или массивом [] Json.
    if(json_doc.isObject()){
        // Разобрать объект в документе
        parseObject(json_doc.object());
    }else if(json_doc.isArray()){
        // Разобрать массив в документе
        parseArray(json_doc.array());
    }

    // Если данные указанного ключа читаются, то сначала определите, содержит ли Объект этот ключ.
    //    const QJsonObject root_obj=json_doc.object();
    //    if(root_obj.contains("name")){
    //        const QJsonValue json_value=root_obj.value("name");
    //        // Если используется root_obj.take("name"), этот элемент исходного объекта будет удален.
    //        if(json_value.isString()){
    //            qDebug()<<"name:"<<json_value.toString();
    //        }
    //    }

    qDebug()<<"load json end:"<<filepath;

    return true;
}

bool ParseJson::dumpJson(const QString &filepath)
{
    // Определить путь
    if(filepath.isEmpty())
        return false;

    // Создайте здесь Json и добавьте смоделированные данные.
    QJsonDocument json_doc;

    QJsonArray json_arr;
    json_arr.insert(0,"Gong");
    json_arr.insert(1,"Jian");
    json_arr.insert(2,"Bo");
    json_arr.append("1992");

    QJsonObject json_obj;
    json_obj.insert("name","Qt");
    QJsonObject json_subobj{{"major",5},{"minor",12},{"patch",6}};
    json_obj.insert("version",json_subobj);
    json_obj.insert("array",json_arr);
    json_doc.setObject(json_obj);

    // Преобразовать в байтовый массив, Compact не имеет разрывов строк, Indented имеет разрывы строк и более читабелен.
    QByteArray json_data=json_doc.toJson(QJsonDocument::Indented);

    // Определите, нормально ли он открывается
    QFile file(filepath);
    if(!file.open(QIODevice::WriteOnly))
        return false;
    file.write(json_data);
    //const bool result=file.waitForBytesWritten(0);
    //qDebug()<<file.error();
    file.close();

    qDebug()<<"dump json end:"<<filepath;

    return true;
}

void ParseJson::parseObject(const QJsonObject &obj)
{
    qDebug()<<"object:";
    // Получите ключи в Объекте и затем пройдите
    const QStringList keys=obj.keys();
    for(const QString &item_key:keys){
        qDebug()<<"key:"<<item_key;
        // Обратите внимание: если это функция значения, QJsonValue возвращает
        // Если это доступ [], есть две ситуации: функция const возвращает QJsonValue.
        //QJsonValue operator[](const QString &key) const
        //QJsonValueRef	operator[](const QString &key)
        // QJsonValueRef — вспомогательный класс для QJsonArray и QJsonObject.
        parseValue(obj.value(item_key));
    }
}

void ParseJson::parseArray(const QJsonArray &arr)
{
    qDebug()<<"array:";
    for(int i=0;i<arr.count();i++){
        parseValue(arr.at(i));
    }
}

void ParseJson::parseValue(const QJsonValue &val)
{
    qDebug()<<"value:";
    QVariant item_val;
    switch (val.type()) {
    case QJsonValue::Object:
        // Если это Object, продолжить рекурсивный анализ
        parseObject(val.toObject());
        return;
        break;
    case QJsonValue::Array:
        // Если это массив, продолжить рекурсивный анализ
        parseArray(val.toArray());
        return;
        break;
    case QJsonValue::Bool:
        item_val=val.toBool();
        break;
    case QJsonValue::Double:
        item_val=val.toDouble();
        break;
    case QJsonValue::String:
        item_val=val.toString();
        break;
    case QJsonValue::Null: break;
    case QJsonValue::Undefined: break;
    default: break;
    }

    qDebug()<<item_val;
}
