#include "NodeForm.h"
#include "ui_NodeForm.h"

#include <QLineEdit>
#include <QGroupBox>
#include <QRadioButton>
#include <QSpinBox>
#include <QComboBox>

QWidget* NodeForm::makeEditingWidget(const SchemaParameter& param)
{
    switch (param.type) {

    case ParameterType::String: {
        auto lineEd = new QLineEdit(this);
        lineEd->setText(param.defaultValue);
        return lineEd;
    }

    case ParameterType::Bool: {
        auto groupBox = new QGroupBox(this);
        auto rdTrue = new QRadioButton("True"),
             rdFalse = new QRadioButton("False");
        auto layout = new QHBoxLayout();
        layout->addWidget(rdTrue);
        layout->addWidget(rdFalse);

        if (param.defaultValue == "True")
            rdTrue->setChecked(true);
        else
            rdFalse->setChecked(false);
        groupBox->setLayout(layout);
        return groupBox;
    }

    case ParameterType::Int: {
        auto spinBox = new QSpinBox(this);
        spinBox->setValue(param.defaultValue.toInt());
        return spinBox;
    }
    case ParameterType::Enum: {
        auto comboBox = new QComboBox(this);

        return comboBox;
    }

    case ParameterType::Image: {
        auto lineEd = new QLineEdit(this);
        lineEd->setText(param.defaultValue);
        return lineEd;
    }

    case ParameterType::Array: {
        auto lineEd = new QLineEdit(this);
        lineEd->setText(param.defaultValue);
        return lineEd;
    }

    }
}

NodeForm::NodeForm(Schema& schema) :
    QWidget(),
    ui(new Ui::NodeForm)
{
    ui->setupUi(this);

    ui->boxNodeArray->setTitle(schema.nodeArrayName);
    for (auto& param : schema.parameters) {
//        paramsEdWidgets.emplace_back(edWidget);
        ui->formLayout->addRow(param.name,
                                 makeEditingWidget(param));
    }
}

NodeForm::~NodeForm()
{
    delete ui;
}
