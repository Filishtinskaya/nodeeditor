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

struct NodeParameter : public QObject
{
    Q_OBJECT
public:
    const QString& getValue() const {
        return (!value.isEmpty() ? value : schema.defaultValue);
    }
    void setValue(const QString& str) {
        value = str;
    }
    const SchemaParameter& schema;
    NodeParameter(const SchemaParameter& aSchema)
        : schema(aSchema) {}
    NodeParameter(const SchemaParameter& aSchema, const QString& str)
        : schema(aSchema), value(str) {}
    virtual void addToJsonObject(QJsonObject& obj) const;
    virtual void restore(const QJsonObject &p);
    virtual ~NodeParameter() = default;

Q_SIGNALS:
    void valueLoaded(const QString& newValue);

private:
    QString value;

};

struct NodeArrayParameter : public NodeParameter
{
    struct Item {
        std::vector<std::unique_ptr<NodeParameter>> paramValues;

        void addToJsonArray(QJsonArray& arr) const;

        Item(const QJsonObject &p, const ArrayParameter& schema);
    };

    const ArrayParameter& schema;

    NodeArrayParameter(const ArrayParameter& aSchema)
        : NodeParameter(aSchema), schema(aSchema) {}

    void addToJsonObject(QJsonObject& obj) const override;
    void restore(const QJsonObject &p) override;

    std::vector<Item> arr;
};

struct NodeData
{
    NodeData(Schema::Ptr aSchema);
    QJsonObject toJsonObject() const;
    Schema::Ptr schema;
    std::vector<std::unique_ptr<NodeParameter>> parameterValues;
};



#endif // SCHEMA_H
