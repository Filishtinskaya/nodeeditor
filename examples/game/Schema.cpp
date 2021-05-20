#include "Schema.hpp"

#include <QLineEdit>

Schema::Schema(const QJsonObject& json)
    : name(json["Name"].toString()),
      nodeArrayName(json["NodeArrayName"].toString())
{
    fillVectorFromJson(parameters, json, QString("Parameters"));
    fillVectorFromJson(nodeArray, json, QString("NodeSchemes"));
}

NodeData::NodeData(Schema::Ptr aSchema)
    : schema(aSchema) {
    parameterValues.reserve(aSchema->parameters.size());
    for (auto& param : schema->parameters) {
        if (param->type == ParameterType::Array) {
            parameterValues.push_back(std::make_unique<NodeArrayParameter>(dynamic_cast<ArrayParameter&>(*param)));
        } else
            parameterValues.push_back(std::make_unique<NodeParameter>(*param));
    }
}

QJsonObject NodeData::toJsonObject() const
{
    QJsonObject res;
    res["name"] = schema->name;
    for (auto& val : parameterValues)
        val->addToJsonObject(res);
    return res;
}

SchemaParameter::SchemaParameter(const QJsonObject &obj)
    : name(obj["Name"].toString()),
      type(getType(obj)),
      defaultValue(obj["Default"].toString())
{
}

ParameterType SchemaParameter::getType(const QJsonObject &obj)
{
    static const std::map<QString, ParameterType> typeStrings = {
        {"String", ParameterType::String},
        {"Bool", ParameterType::Bool},
        {"Int", ParameterType::Int},
        {"Enum", ParameterType::Enum},
        {"Image", ParameterType::Image},
        {"Array", ParameterType::Array}
    };
    return typeStrings.at(obj["Type"].toString());
}

EnumParameter::EnumParameter(const QJsonObject &obj)
    : SchemaParameter(obj)
{
    fillVectorFromJson(values, obj, QString("EnumValues"));
}

ArrayParameter::ArrayParameter(const QJsonObject &obj)
    : SchemaParameter(obj)
{
    fillVectorFromJson(properties, obj, QString("ArrayProperties"));
}


template<>
Schema::Ptr fromJsonValueRef<Schema::Ptr>(const QJsonValueRef ref) {
    return std::make_shared<Schema>(ref.toObject());
}

template<>
SchemaParameter::Ptr fromJsonValueRef<SchemaParameter::Ptr>(const QJsonValueRef ref) {
    auto obj = ref.toObject();
    switch(SchemaParameter::getType(obj)) {
    case ParameterType::Enum:
        return std::make_shared<EnumParameter>(obj);
    case ParameterType::Array:
        return std::make_shared<ArrayParameter>(obj);
    default:
        return std::make_shared<SchemaParameter>(obj);
    }
}

template <>
QString fromJsonValueRef(const QJsonValueRef ref) {
    return ref.toString();
}

void NodeParameter::addToJsonObject(QJsonObject &obj) const {
    obj[schema.name] = value;
}

void NodeParameter::restore(const QJsonObject &p) {
    value = p[schema.name].toString();
    emit valueLoaded(value);
}

void NodeArrayParameter::addToJsonObject(QJsonObject &obj) const {
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
}
