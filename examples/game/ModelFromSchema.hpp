#ifndef MODELFROMSCHEMA_H
#define MODELFROMSCHEMA_H

#include <QObject>
#include <QFrame>

#include <nodes/NodeDataModel>
#include "nodes/DataModelRegistry"
#include "NodeInstance.hpp"

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::DataModelRegistry;
using QtNodes::NodeValidationState;

class NodeForm;

class JsonSchemaParser
{
public:
    JsonSchemaParser(DataModelRegistry& aReg);
private:
    void registerModelFromSchema(Schema::Ptr aSchema);
    DataModelRegistry& dmRegistry;
    GeneralInfo info;
};

class ModelFromSchema : public NodeDataModel
{
    Q_OBJECT
public:
    ModelFromSchema(Schema::Ptr aSchema);
    virtual ~ModelFromSchema();
    QString caption() const override
    {return data.schema->name;}

    bool captionVisible() const final {return true;} ;

    QString name() const override
     {return data.schema->name;}

     QJsonObject save() const override
      {
         return data.toJsonObject();
      }
     void restore(QJsonObject const &p) override {
        for (auto& param : data.parameterValues) {
            param->restore(p);
        }
     }
     unsigned int nPorts(PortType) const override
      {
        return 3;
      }

     NodeDataType dataType(PortType, PortIndex) const override
     {return {};}

     std::shared_ptr<QtNodes::NodeData>outData(PortIndex) override
      {
         return /*std::make_shared<NodeData>()*/{};
      }

      void setInData(std::shared_ptr<QtNodes::NodeData>, int) override
      {
        //
      }

      QWidget* embeddedWidget() override;
private:
    NodeData data;
    NodeForm* widget;
};


#endif // MODELFROMSCHEMA_H
