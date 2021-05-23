#include "NodeForm.h"
#include "ui_NodeForm.h"

#include <QLabel>
#include <QPushButton>
#include <QToolBar>


QGroupBox* NodeForm::addPlaceForActOption(ActOption &actOption)
{
    auto box = new QGroupBox(this);
    auto layout = new QFormLayout(box);
    box->setLayout(layout);
    for (auto& param : actOption.parameterValues) {
        layout->addRow(param->schema->name, makeEditingWidget(param.get()));
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
    ui(new Ui::NodeForm),
    data(aData)
{
    ui->setupUi(this);

    for (auto& param : data.parameterValues) {
        ui->formLayout->addRow(param->schema->name,
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
