#ifndef NODEFORM_H
#define NODEFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QGroupBox>

#include "NodeInstance.hpp"

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
    QWidget* makeEditingWidget(NodeParameter* val);
    QGroupBox* addPlaceForActOption(ActOption& actOption);

    QLineEdit* edName,
               edNodeArrayName;

    Ui::NodeForm *ui;

    NodeData& data;

//    std::vector<QWidget*> paramsEdWidgets;
private Q_SLOTS:
    void newActOption();
};

#endif // NODEFORM_H
