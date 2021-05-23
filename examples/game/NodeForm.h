#ifndef NODEFORM_H
#define NODEFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QGroupBox>

#include "ParametersWidget.h"

namespace Ui {
class NodeForm;
}

class NodeForm : public ParametersWidget
{
public:
    explicit NodeForm(NodeData& aData);
    virtual ~NodeForm();

private:
    QGroupBox* addPlaceForActOption(ActOption& actOption);
    using ParametersWidget::makeEditingWidget;

    Ui::NodeForm *ui;

    NodeData& data;

//    std::vector<QWidget*> paramsEdWidgets;
private Q_SLOTS:
    void newActOption();
};

#endif // NODEFORM_H
