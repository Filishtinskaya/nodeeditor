#include "ParametersWidget.h"

#include "ClickableLabel.hpp"

#include <QLineEdit>
#include <QLabel>
#include <QRadioButton>
#include <QSpinBox>
#include <QComboBox>
#include <QFileDialog>
#include <QPushButton>
#include <QGroupBox>
#include <QHBoxLayout>

ParametersWidget::ParametersWidget(QWidget *parent) : QWidget(parent)
{
}

namespace {
    void fitPixmapInLabel(const QString& path, QLabel& label) {
        QPixmap pixmap(path);
        if (pixmap.isNull())
            label.setText("Click here to choose image.");
        else
            label.setPixmap(pixmap.scaled(300, 300, Qt::AspectRatioMode::KeepAspectRatio));
    }
}

QWidget* ParametersWidget::makeEditingWidget(NodeParameter* val)
{
    switch (val->schema->type) {

    case ParameterType::String: {
        auto lineEd = new QLineEdit(this);
        lineEd->setText(val->getValue());
        connect(lineEd, &QLineEdit::textEdited, [val](const QString &text){
            val->setValue(text);
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

        return groupBox;
    }

    case ParameterType::Int: {
        auto spinBox = new QSpinBox(this);
        spinBox->setValue(val->getValue().toInt());
        connect(spinBox, &QSpinBox::textChanged, [val](const QString &text) {
            val->setValue(text);
        });
        return spinBox;
    }
    case ParameterType::Enum: {
        auto comboBox = new QComboBox(this);
        auto enumParam = dynamic_cast<const EnumParameter*>(val->schema.get());
        for (auto& value : enumParam->values) {
            comboBox->addItem(value);
        }
        comboBox->setCurrentText(val->getValue());
        connect(comboBox, &QComboBox::currentTextChanged, [val](const QString &text) {
            val->setValue(text);
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

        return label;
    }

    /*case ParameterType::Array: {
        auto lineEd = new QLineEdit(this);
        lineEd->setText(val->schema.defaultValue);
        return lineEd;
    }*/

    }
}
