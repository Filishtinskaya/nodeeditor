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

ModelFromSchema::ModelFromSchema()
    : widget(new NodeForm()),
      schema(fetchJson())
{

}

