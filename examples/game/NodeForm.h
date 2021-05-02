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
    explicit NodeForm(Schema& schema);
    ~NodeForm();

private:
    QWidget* makeEditingWidget(const SchemaParameter& param);

    QLineEdit* edName,
               edNodeArrayName;

    Ui::NodeForm *ui;

//    std::vector<QWidget*> paramsEdWidgets;
};

#endif // NODEFORM_H
