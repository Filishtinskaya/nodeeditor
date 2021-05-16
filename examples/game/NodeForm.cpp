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

QWidget* NodeForm::makeEditingWidget(NodeParameter& param)
{
    switch (param.type) {

    case ParameterType::String: {
        auto lineEd = new QLineEdit(this);
        lineEd->setText(param.defaultValue);
        connect(lineEd, &QLineEdit::textEdited, [paramPtr = &param](const QString &text){
            paramPtr->value = text;
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

        connect(rdTrue, &QRadioButton::clicked, [paramPtr = &param]() {
            paramPtr->value = "True";
        });
        connect(rdFalse, &QRadioButton::clicked, [paramPtr = &param]() {
            paramPtr->value = "False";
        });

        return groupBox;
    }

    case ParameterType::Int: {
        auto spinBox = new QSpinBox(this);
        spinBox->setValue(param.defaultValue.toInt());
        connect(spinBox, &QSpinBox::textChanged, [paramPtr = &param](const QString &text) {
            paramPtr->value = text;
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
        connect(comboBox, &QComboBox::currentTextChanged, [paramPtr = &param](const QString &text) {
            paramPtr->value = text;
        });
        return comboBox;
    }

    case ParameterType::Image: {
        auto label = new ClickableLabel(this);
        fitPixmapInLabel(param.defaultValue, *label);

        QObject::connect(label, &ClickableLabel::clicked, [label, paramPtr = &param](){
            QString fileName = QFileDialog::getOpenFileName(nullptr, "Choose image",
                paramPtr->value, "Image Files (*.png *.jpg *.bmp)");
            if (!fileName.isEmpty()) {
                fitPixmapInLabel(fileName, *label);
                paramPtr->value = fileName;
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

NodeForm::NodeForm(Schema& aSchema) :
    QWidget(),
    ui(new Ui::NodeForm)
{
    ui->setupUi(this);

    ui->boxNodeArray->setTitle(aSchema.nodeArrayName);
    for (auto& param : aSchema.parameters) {
//        paramsEdWidgets.emplace_back(edWidget);
        ui->formLayout->addRow(param->name,
                                 makeEditingWidget(*param));
    }
}

NodeForm::~NodeForm()
{
    delete ui;
}
