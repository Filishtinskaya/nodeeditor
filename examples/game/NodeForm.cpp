#include "NodeForm.h"
#include "ui_NodeForm.h"

#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QRadioButton>
#include <QSpinBox>
#include <QComboBox>
#include <QFileDialog>

#include "ClickableLabel.hpp"

namespace {
    void fitPixmapInLabel(const QString& path, QLabel& label) {
        QPixmap pixmap(path);
        if (pixmap.isNull())
            label.setText("Click here to choose image.");
        else
            label.setPixmap(pixmap.scaled(300, 300, Qt::AspectRatioMode::KeepAspectRatio));
    }
}

QWidget* NodeForm::makeEditingWidget(const SchemaParameter& param, ParameterValue* val)
{
    switch (param.type) {

    case ParameterType::String: {
        auto lineEd = new QLineEdit(this);
        lineEd->setText(param.defaultValue);
        connect(lineEd, &QLineEdit::textEdited, [val](const QString &text){
            val->value = text;
        });
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

        connect(rdTrue, &QRadioButton::clicked, [val]() {
            val->value = "True";
        });
        connect(rdFalse, &QRadioButton::clicked, [val]() {
            val->value = "False";
        });

        return groupBox;
    }

    case ParameterType::Int: {
        auto spinBox = new QSpinBox(this);
        spinBox->setValue(param.defaultValue.toInt());
        connect(spinBox, &QSpinBox::textChanged, [val](const QString &text) {
            val->value = text;
        });
        return spinBox;
    }
    case ParameterType::Enum: {
        auto comboBox = new QComboBox(this);
        auto enumParam = dynamic_cast<const EnumParameter*>(&param);
        for (auto& value : enumParam->values) {
            comboBox->addItem(value);
        }
        comboBox->setCurrentText(enumParam->defaultValue);
        connect(comboBox, &QComboBox::currentTextChanged, [val](const QString &text) {
            val->value = text;
        });
        return comboBox;
    }

    case ParameterType::Image: {
        auto label = new ClickableLabel(this);
        fitPixmapInLabel(param.defaultValue, *label);

        QObject::connect(label, &ClickableLabel::clicked, [label, val](){
            QString fileName = QFileDialog::getOpenFileName(nullptr, "Choose image",
                val->value, "Image Files (*.png *.jpg *.bmp)");
            if (!fileName.isEmpty()) {
                fitPixmapInLabel(fileName, *label);
                val->value = fileName;
            }
        });
        return label;
    }

    case ParameterType::Array: {
        auto lineEd = new QLineEdit(this);
        lineEd->setText(param.defaultValue);
        return lineEd;
    }

    }
}

NodeForm::NodeForm(NodeData& aData) :
    QWidget(),
    ui(new Ui::NodeForm),
    data(aData)
{
    ui->setupUi(this);

    ui->boxNodeArray->setTitle(aData.schema->nodeArrayName);
    for (size_t i = 0; i < aData.schema->parameters.size(); i++) {
        auto& param = aData.schema->parameters[i];
        ui->formLayout->addRow(param->name,
                                 makeEditingWidget(*param, &aData.parameterValues[i]));
    }
}

NodeForm::~NodeForm()
{
    delete ui;
}
