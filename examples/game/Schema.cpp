#include "Schema.hpp"


const std::map<QString, ParameterType> SchemaParameter::typeStrings = {
    {"String", ParameterType::String},
    {"Bool", ParameterType::Bool},
    {"Int", ParameterType::Int},
    {"Enum", ParameterType::Enum},
    {"Image", ParameterType::Image},
    {"Array", ParameterType::Array}
};

Schema::Schema(const QJsonObject& json)
    : name(json["Name"].toString()),
      nodeArrayName(json["NodeArrayName"].toString())
{
    fillVectorFromJson(parameters, json, QString("Parameters"));
    fillVectorFromJson<Schema>(nodeArray, json, nodeArrayName);
}

SchemaParameter::SchemaParameter(const QJsonObject &obj)
    : name(obj["Name"].toString()),
      type(typeStrings.at(obj["Type"].toString())),
      defaultValue(obj["Default"].toString())
{
}
