#ifndef MODELFROMSCHEMA_H
#define MODELFROMSCHEMA_H

#include <QObject>
#include <QFrame>

#include <nodes/NodeDataModel>
#include "nodes/DataModelRegistry"
#include "Schema.hpp"

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::DataModelRegistry;
using QtNodes::NodeValidationState;

class NodeForm;

class JsonSchemaParser
{
public:
    JsonSchemaParser(DataModelRegistry& aReg);
    void fetchModels();
private:
    void registerModelFromSchema(std::shared_ptr<Schema> aSchema);
    DataModelRegistry& dmRegistry;
};

class ModelFromSchema : public NodeDataModel
{
    Q_OBJECT
public:
    ModelFromSchema(std::shared_ptr<Schema> aSchema);
    virtual ~ModelFromSchema();
    QString caption() const override
    {return schema->name;}

    bool captionVisible() const final {return true;} ;

    QString name() const override
     {return schema->name;}

     QJsonObject save() const override
      {
        QJsonObject modelJson;

        modelJson["name"] = name();

        return modelJson;
      }
     unsigned int nPorts(PortType) const override
      {
        return 3;
      }

     NodeDataType dataType(PortType, PortIndex) const override
     {return {};}

     std::shared_ptr<NodeData>outData(PortIndex) override
      {
         return /*std::make_shared<NodeData>()*/{};
      }

      void setInData(std::shared_ptr<NodeData>, int) override
      {
        //
      }

      QWidget* embeddedWidget() override;
private:
    std::shared_ptr<Schema> schema;
    NodeForm* widget;
};


#endif // MODELFROMSCHEMA_H
