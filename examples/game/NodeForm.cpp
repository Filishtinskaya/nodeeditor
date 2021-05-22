#include "NodeForm.h"
#include "ui_NodeForm.h"

#include <QLineEdit>
#include <QLabel>
#include <QRadioButton>
#include <QSpinBox>
#include <QComboBox>
#include <QFileDialog>
#include <QPushButton>
#include <QToolBar>

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
        auto enumParam = dynamic_cast<const EnumParameter*>(&val->schema);
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

QGroupBox* NodeForm::addPlaceForActOption(ActOption &actOption)
{
    auto box = new QGroupBox(this);
    auto layout = new QFormLayout(box);
    box->setLayout(layout);
    for (auto& param : actOption.parameterValues) {
        layout->addRow(param->schema.name, makeEditingWidget(param.get()));
    }
    auto btn = new QPushButton(box);
    btn->setText("Delete");
    layout->addWidget(btn);
    connect(btn, &QPushButton::pressed, [form = this, actOption = &actOption, box](){
        box->setParent(NULL);
        delete box;
        auto& vec = form->data.actOptions;
        for (auto it = vec.begin(); it != vec.end(); it++) {
            if (&(*it) == actOption) {
                vec.erase(it);
                break;
            }
        }
    });
    return box;
}

void NodeForm::newActOption()
{
    auto option = data.actOptions.emplace_back(data.schema->actOptionsSchema);
    ui->formLayout_2->addWidget(addPlaceForActOption(option));
}

NodeForm::NodeForm(NodeData& aData) :
    QWidget(),
    ui(new Ui::NodeForm),
    data(aData)
{
    ui->setupUi(this);

    for (auto& param : data.parameterValues) {
        ui->formLayout->addRow(param->schema.name,
                                 makeEditingWidget(param.get()));
    }
\
    auto toolBar = new QToolBar();
    ui->formLayout_2->addWidget(toolBar);
    QAction* addActOption = toolBar->addAction("Add");

    connect(addActOption, &QAction::triggered, this, &NodeForm::newActOption);

    for (auto& actOption : data.actOptions) {
        ui->formLayout_2->addWidget(addPlaceForActOption(actOption));
    }
}

NodeForm::~NodeForm()
{
    delete ui;
}
