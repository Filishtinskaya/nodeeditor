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
    String, Bool, Int, Enum, Image, Array
};

struct NodeParameter : public QObject
{
    const QString name;
    const ParameterType type;
    const QString defaultValue; //std::variant???

    static ParameterType getType(const QJsonObject& obj);
    NodeParameter(const QJsonObject& obj);

    virtual ~NodeParameter() = default;

    using Ptr = std::shared_ptr<NodeParameter>;

    //relates to concrete instance
    QString value;
};

struct ArrayParameter : public NodeParameter
{
    ArrayParameter(const QJsonObject& obj);
    std::vector<NodeParameter::Ptr> properties;
};

struct EnumParameter : public NodeParameter
{
    EnumParameter(const QJsonObject& obj);
    std::vector<QString> values;
};

// C++ representation of the schema structure
struct Schema
{
    Schema(const QJsonObject& json);

    using Ptr = std::shared_ptr<Schema>;

    const QString name, nodeArrayName;
    std::vector<NodeParameter::Ptr> parameters;
    std::vector<Schema::Ptr> nodeArray;
};


#endif // SCHEMA_H
