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

struct SchemaParameter
{
    const QString name;
    const ParameterType type;
    const QString defaultValue; //std::variant???

    static ParameterType getType(const QJsonObject& obj);
    SchemaParameter(const QJsonObject& obj);

    virtual ~SchemaParameter() = default;

    using Ptr = std::shared_ptr<SchemaParameter>;
};


// C++ representation of the schema structure
struct Schema
{
    Schema(const QJsonObject& json);

    using Ptr = std::shared_ptr<Schema>;

    const QString name, nodeArrayName;
    std::vector<SchemaParameter::Ptr> parameters;
    std::vector<Schema::Ptr> nodeArray;
};

struct ArrayParameter : public SchemaParameter
{
    ArrayParameter(const QJsonObject& obj);
    std::vector<SchemaParameter::Ptr> properties;
};

struct EnumParameter : public SchemaParameter
{
    EnumParameter(const QJsonObject& obj);
    std::vector<QString> values;
};

///////for concrete nodes///////////////////

struct ParameterValue
{
    QString value;
    const QString& name;
    ParameterValue(const QString& aName)
        : name(aName) {}
    virtual void addToJsonObject(QJsonObject& obj) const {
        obj[name] = value;
    }
    virtual ~ParameterValue() = default;
};

struct ArrayParameterValue : public ParameterValue
{
    struct Item {
        std::vector<ParameterValue> paramValues;
        void addToJsonArray(QJsonArray& arr) const {
            QJsonObject obj;
            for (auto& param : paramValues) {
                obj[param.name] = param.value;
            }
            arr.push_back(obj);
        };
    };
    void addToJsonObject(QJsonObject& obj) const override {
        QJsonArray jsonArray;
        for (auto& item : arr) {
            item.addToJsonArray(jsonArray);
        }
        obj[name] = jsonArray;
    }
    std::vector<Item> arr;
};

struct NodeData
{
    NodeData(Schema::Ptr aSchema)
        : schema(aSchema) {
        parameterValues.reserve(aSchema->parameters.size());
        for (size_t i = 0; i < schema->parameters.size(); i++) {
            parameterValues.emplace_back(schema->parameters[i]->name);
        }
    }
    QJsonObject toJsonObject() const;
    Schema::Ptr schema;
    std::vector<ParameterValue> parameterValues;
};



#endif // SCHEMA_H
