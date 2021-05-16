#include "Schema.hpp"

Schema::Schema(const QJsonObject& json)
    : name(json["Name"].toString()),
      nodeArrayName(json["NodeArrayName"].toString())
{
    fillVectorFromJson(parameters, json, QString("Parameters"));
    fillVectorFromJson(nodeArray, json, QString("NodeSchemes"));
}

NodeParameter::NodeParameter(const QJsonObject &obj)
    : name(obj["Name"].toString()),
      type(getType(obj)),
      defaultValue(obj["Default"].toString()),
      value(defaultValue)
{
}

ParameterType NodeParameter::getType(const QJsonObject &obj)
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
    : NodeParameter(obj)
{
    fillVectorFromJson(values, obj, QString("EnumValues"));
}

ArrayParameter::ArrayParameter(const QJsonObject &obj)
    : NodeParameter(obj)
{

}


template<>
Schema::Ptr fromJsonValueRef<Schema::Ptr>(const QJsonValueRef ref) {
    return std::make_shared<Schema>(ref.toObject());
}

template<>
NodeParameter::Ptr fromJsonValueRef<NodeParameter::Ptr>(const QJsonValueRef ref) {
    auto obj = ref.toObject();
    switch(NodeParameter::getType(obj)) {
    case ParameterType::Enum:
        return std::make_shared<EnumParameter>(obj);
    case ParameterType::Array:
        return std::make_shared<ArrayParameter>(obj);
    default:
        return std::make_shared<NodeParameter>(obj);
    }
}

template <>
QString fromJsonValueRef(const QJsonValueRef ref) {
    return ref.toString();
}
