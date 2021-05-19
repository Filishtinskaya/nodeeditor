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
    rootSchema = std::make_shared<Schema>(fetchJson());

    registerModelFromSchema(rootSchema);
}

void JsonSchemaParser::registerModelFromSchema(Schema::Ptr aSchema)
{
    DataModelRegistry::RegistryItemCreator creator = [=](){
        return std::make_unique<ModelFromSchema>(aSchema);
    };

    dmRegistry.registerModel<ModelFromSchema>(creator);

    for (auto& childSchema : aSchema->nodeArray) {
        registerModelFromSchema(childSchema);
    }
}


ModelFromSchema::ModelFromSchema(Schema::Ptr aSchema)
    : data(aSchema),
      widget(new NodeForm(data))
{
}

ModelFromSchema::~ModelFromSchema() {
    delete widget;
}

QWidget *ModelFromSchema::embeddedWidget() { return widget; }

