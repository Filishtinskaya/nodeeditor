#ifndef SCHEMA_H
#define SCHEMA_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>

#include <map>
#include <vector>

template <typename T>
T fromJsonValueRef(const QJsonValueRef ref);

//vector's objects should implement fromJsonValueRef specialization
template<typename VectorType>
void fillVectorFromJson(std::vector<VectorType>& v, const QJsonObject& obj, QStringView arrayName)
{
    auto arr = obj[arrayName].toArray();
    for (QJsonValueRef parameter : arr)
        v.push_back(fromJsonValueRef<VectorType>(parameter));
}

enum class ParameterType {
    String, Bool, Int, Enum, Image/*, Array*/
};

struct SchemaParameter
{
    const QString name;
    const ParameterType type;
    const QString defaultValue; //std::variant???

    static ParameterType getType(const QJsonObject& obj);

    virtual ~SchemaParameter() = default;

    using Ptr = std::shared_ptr<SchemaParameter>;

protected:
    friend SchemaParameter::Ptr fromJsonValueRef<SchemaParameter::Ptr>(const QJsonValueRef ref);
    SchemaParameter(const QJsonObject& obj); //fromJsonValueRef should be used to ensure polymorphism
};

struct ActOptionSchema {
    std::vector<SchemaParameter::Ptr> parameters;
};

struct Schema
{
    Schema(const QJsonObject& json);
    using Ptr = std::shared_ptr<Schema>;
    const QString name;
    std::vector<SchemaParameter::Ptr> parameters;
    ActOptionSchema actOptionsSchema;
};

/*struct ArrayParameter : public SchemaParameter
{
    ArrayParameter(const QJsonObject& obj);
    std::vector<SchemaParameter::Ptr> properties;
};*/

struct EnumParameter : public SchemaParameter
{
    std::vector<QString> values;
private:
    friend SchemaParameter::Ptr fromJsonValueRef<SchemaParameter::Ptr>(const QJsonValueRef ref);
    EnumParameter(const QJsonObject& obj);
};

#endif // SCHEMA_H
