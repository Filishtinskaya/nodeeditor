#ifndef MODELFROMSCHEMA_H
#define MODELFROMSCHEMA_H

#include <QObject>

#include <nodes/NodeDataModel>
#include "Schema.hpp"

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class ModelFromSchema : public NodeDataModel
{
    Q_OBJECT
public:
    ModelFromSchema();
    virtual ~ModelFromSchema() {}
    QString caption() const override
    {return QString("My Data Model");}

    QString name() const override
     {return QString("MyDataModel");}

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

      QWidget* embeddedWidget() override { return nullptr; }
private:
    Schema schema;
};


#endif // MODELFROMSCHEMA_H
