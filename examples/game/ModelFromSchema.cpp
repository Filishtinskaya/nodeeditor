#include "ModelFromSchema.hpp"
#include "NodeForm.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QLayout>
#include <QLabel>

#include <iostream>

const QString SCHEMA_PATH = "/home/tetiana/Documents/NodeEditor/gameEventScheme.json";

QJsonObject fetchJson()
{
    QFile file(SCHEMA_PATH);
    if (!file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("schema file not found");
    }
    QByteArray wholeFile = file.readAll();

    return QJsonDocument::fromJson(wholeFile).object();
}


JsonSchemaParser::JsonSchemaParser(DataModelRegistry& aReg)
    : dmRegistry(aReg)
{}

void JsonSchemaParser::fetchModels()
{
    auto rootSchema = std::make_shared<Schema>(fetchJson());

    registerModelFromSchema(rootSchema);
}

void JsonSchemaParser::registerModelFromSchema(std::shared_ptr<Schema> aSchema)
{
    DataModelRegistry::RegistryItemCreator creator = [&](){
        return std::make_unique<ModelFromSchema>(aSchema);
    };

    dmRegistry.registerModel<ModelFromSchema>(creator);

    for (auto& childSchema : aSchema->nodeArray) {
        auto schemaPtr = std::make_shared<Schema>(childSchema);
        registerModelFromSchema(schemaPtr);
    }
}


ModelFromSchema::ModelFromSchema(std::shared_ptr<Schema> aSchema)
    : schema(aSchema),
      widget(new NodeForm(*schema))
{
}

ModelFromSchema::~ModelFromSchema() {
    delete widget;
}

QWidget *ModelFromSchema::embeddedWidget() { return widget; }

