#include "NodeInstance.hpp"
#include <QObject>

NodeData::NodeData(Schema::Ptr aSchema)
    : schema(aSchema)
{
    linkParametersToSchemas(schema, this);
}

QJsonObject NodeData::toJsonObject() const
{
    QJsonObject res;
    res["name"] = schema->name;
    for (auto& val : parameterValues)
        val->addToJsonObject(res);
    return res;
}


/*void NodeArrayParameter::addToJsonObject(QJsonObject &obj) const {
    QJsonArray jsonArray;
    for (auto& item : arr) {
        item.addToJsonArray(jsonArray);
    }
    obj[schema.name] = jsonArray;
//    emit valueLoaded(value);
}

void NodeArrayParameter::restore(const QJsonObject &p) {
    QJsonArray jsonArray = p[schema.name].toArray();
    for (auto item : jsonArray)
        arr.push_back(Item(item.toObject(), schema));
}

void NodeArrayParameter::Item::addToJsonArray(QJsonArray &arr) const {
    QJsonObject obj;
    for (auto& param : paramValues) {
        obj[param->schema.name] = param->getValue();
    }
    arr.push_back(obj);
}

NodeArrayParameter::Item::Item(const QJsonObject &p, const ArrayParameter& schema) {
    for (auto& property : schema.properties) {
        QString value = p[property->name].toString();
        auto newParam = std::make_unique<NodeParameter>(*property, value);
        paramValues.push_back(std::move(newParam));
    }
}*/


void NodeParameter::addToJsonObject(QJsonObject &obj) const {
    obj[schema->name] = value;
}

void NodeParameter::restore(const QJsonObject &p) {
    value = p[schema->name].toString();
}

GeneralInfo::GeneralInfo(const QJsonObject &json)
    : name(json["Name"].toString()),
      nodeArrayName(json["NodeArrayName"].toString())
{
    fillVectorFromJson(parameters, json, QString("Parameters"));
    fillVectorFromJson(nodeSchemes, json, QString("NodeSchemes"));
}

ActOption::ActOption(const ActOptionSchema &schema)
{
    linkParametersToSchemas(&schema, this);
}


template<>
NodeParameter::Ptr fromJsonValueRef<NodeParameter::Ptr>(const QJsonValueRef ref) {
    auto schema = fromJsonValueRef<SchemaParameter::Ptr>(ref);
    return std::make_shared<NodeParameter>(schema);
}
