#ifndef PARAMETERSWIDGET_H
#define PARAMETERSWIDGET_H

#include <QObject>
#include <QWidget>

#include "NodeInstance.hpp"

class ParametersWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ParametersWidget(QWidget *parent = nullptr);
    virtual ~ParametersWidget() = default;

protected:
    QWidget* makeEditingWidget(NodeParameter* val);

};

#endif // PARAMETERSWIDGET_H
