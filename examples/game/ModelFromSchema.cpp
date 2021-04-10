#include "ModelFromSchema.hpp"

#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>

const QString SCHEMA_PATH = "gameEventScheme.json";


QJsonObject fetchJson() {
    QFile file(SCHEMA_PATH);
    //@todo exception handling
    file.open(QIODevice::ReadOnly);
    QByteArray wholeFile = file.readAll();

    return QJsonDocument::fromJson(wholeFile).object();
}

ModelFromSchema::ModelFromSchema()
    : schema(fetchJson())
{
}

