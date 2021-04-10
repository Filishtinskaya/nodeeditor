#ifndef SCHEMA_H
#define SCHEMA_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>

#include <map>
#include <vector>

//vector's objects should be constructible from const QJsonObject& to use this functions
template<typename VectorType>
void fillVectorFromJson(std::vector<VectorType>& v, const QJsonObject& obj, QStringView arrayName)
{
    for (QJsonValueRef parameter : obj[arrayName].toArray())
    {
        QJsonObject obj = parameter.toObject();
        v.emplace_back(parameter.toObject());
    }
}


enum class ParameterType {
    String, Bool, Int, Enum, Image, Array
};

struct SchemaParameter
{
    static const std::map<QString, ParameterType> typeStrings;
    QString name;
    ParameterType type;
    QString defaultValue; //std::variant???

    SchemaParameter(const QJsonObject& obj);
};

struct ArrayParameter : public SchemaParameter
{
    ArrayParameter(const QJsonObject& obj);
    std::vector<SchemaParameter> parameters;
};

struct EnumParameter : public SchemaParameter
{
    std::vector<QString> values;
};

// C++ representation of the schema structure
struct Schema {
    Schema(const QJsonObject& json);

    QString name, nodeArrayName;
    std::vector<SchemaParameter> parameters;
    std::vector<Schema> nodeSchemes;
};

#endif // SCHEMA_H
