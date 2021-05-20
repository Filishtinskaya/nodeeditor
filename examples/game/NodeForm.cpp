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

QWidget* NodeForm::makeEditingWidget(NodeParameter* val)
{
    switch (val->schema.type) {

    case ParameterType::String: {
        auto lineEd = new QLineEdit(this);
        lineEd->setText(val->getValue());
        connect(lineEd, &QLineEdit::textEdited, [val](const QString &text){
            val->setValue(text);
        });
        connect(val, &NodeParameter::valueLoaded, [lineEd](const QString& value){
            lineEd->setText(value);
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

        if (val->getValue() == "True")
            rdTrue->setChecked(true);
        else
            rdFalse->setChecked(true);
        groupBox->setLayout(layout);

        connect(rdTrue, &QRadioButton::clicked, [val]() {
            val->setValue("True");
        });
        connect(rdFalse, &QRadioButton::clicked, [val]() {
            val->setValue("False");
        });

        connect(val, &NodeParameter::valueLoaded, [rdTrue, rdFalse](const QString& value){
            if (value == "True")
                rdTrue->setChecked(true);
            else
                rdFalse->setChecked(true);
        });

        return groupBox;
    }

    case ParameterType::Int: {
        auto spinBox = new QSpinBox(this);
        spinBox->setValue(val->getValue().toInt());
        connect(spinBox, &QSpinBox::textChanged, [val](const QString &text) {
            val->setValue(text);
        });
        connect(val, &NodeParameter::valueLoaded, [spinBox](const QString& value){
            spinBox->setValue(value.toInt());
        });
        return spinBox;
    }
    case ParameterType::Enum: {
        auto comboBox = new QComboBox(this);
        auto enumParam = dynamic_cast<const EnumParameter*>(&val->schema);
        for (auto& value : enumParam->values) {
            comboBox->addItem(value);
        }
        comboBox->setCurrentText(val->getValue());
        connect(comboBox, &QComboBox::currentTextChanged, [val](const QString &text) {
            val->setValue(text);
        });
        connect(val, &NodeParameter::valueLoaded, [comboBox](const QString& value){
            comboBox->setCurrentText(value);
        });
        return comboBox;
    }

    case ParameterType::Image: {
        auto label = new ClickableLabel(this);
        fitPixmapInLabel(val->getValue(), *label);

        connect(label, &ClickableLabel::clicked, [label, val](){
            QString fileName = QFileDialog::getOpenFileName(nullptr, "Choose image",
                val->getValue(), "Image Files (*.png *.jpg *.bmp)");
            if (!fileName.isEmpty()) {
                fitPixmapInLabel(fileName, *label);
                val->setValue(fileName);
            }
        });

        connect(val, &NodeParameter::valueLoaded, [label](const QString& value){
            fitPixmapInLabel(value, *label);
        });

        return label;
    }

    case ParameterType::Array: {
        auto lineEd = new QLineEdit(this);
        lineEd->setText(val->schema.defaultValue);
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
    for (auto& param : data.parameterValues) {
        ui->formLayout->addRow(param->schema.name,
                                 makeEditingWidget(param.get()));
    }
}

NodeForm::~NodeForm()
{
    delete ui;
}
