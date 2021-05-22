#ifndef NODEINSTANCE_H
#define NODEINSTANCE_H
#include "Schema.hpp"

template<typename SchemaPtr, typename InstancePtr>
void linkParametersToSchemas(SchemaPtr& schema, InstancePtr& instance);

struct NodeParameter
{
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
    NodeParameter(SchemaParameter&& aSchema)
        : schema(std::move(aSchema)) {}
    NodeParameter(const SchemaParameter& aSchema, const QString& str)
        : schema(aSchema), value(str) {}
    virtual void addToJsonObject(QJsonObject& obj) const;
    virtual void restore(const QJsonObject &p);
    virtual ~NodeParameter() = default;

private:
    QString value;

};

struct ActOption {
    ActOption(const ActOptionSchema& schema);
    std::vector<std::shared_ptr<NodeParameter>> parameterValues;
};

struct NodeData
{
    NodeData(Schema::Ptr aSchema);
    QJsonObject toJsonObject() const;
    Schema::Ptr schema;
    std::vector<std::unique_ptr<NodeParameter>> parameterValues;
    std::vector<ActOption> actOptions;
};

struct GeneralInfo {
    GeneralInfo(const QJsonObject& json);
    const QString name, nodeArrayName;
    std::vector<NodeParameter> parameters;
    std::vector<Schema::Ptr> nodeSchemes;
};


/*struct NodeArrayParameter : public NodeParameter
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
};*/

template<typename SchemaPtr, typename InstancePtr>
void linkParametersToSchemas(SchemaPtr schema, InstancePtr instance) {
    instance->parameterValues.reserve(schema->parameters.size());
    for (auto& param : schema->parameters) {
        /*if (param->type == ParameterType::Array) {
            parameterValues.push_back(std::make_unique<NodeArrayParameter>(dynamic_cast<ArrayParameter&>(*param)));
        } else*/
            instance->parameterValues.push_back(std::make_unique<NodeParameter>(*param));

    }
}

#endif // NODEINSTANCE_H
