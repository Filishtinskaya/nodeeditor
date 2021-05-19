#ifndef NODEFORM_H
#define NODEFORM_H

#include <QWidget>
#include <QLineEdit>

#include "Schema.hpp"

namespace Ui {
class NodeForm;
}

class NodeForm : public QWidget
{
    Q_OBJECT

public:
    explicit NodeForm(NodeData& aData);
    ~NodeForm();

private:
    QWidget* makeEditingWidget(const SchemaParameter& param, ParameterValue* val);

    QLineEdit* edName,
               edNodeArrayName;

    Ui::NodeForm *ui;

    NodeData& data;

//    std::vector<QWidget*> paramsEdWidgets;
};

#endif // NODEFORM_H
