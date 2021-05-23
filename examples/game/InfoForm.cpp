#include "InfoForm.h"
#include "ui_InfoForm.h"

InfoForm::InfoForm(GeneralInfo& aInfo) :
    ui(new Ui::InfoForm),
    info(aInfo)
{
    ui->setupUi(this);
    ui->lbName->setText("Name: " + info.name);
    ui->lbNodeArrayName->setText("Node array name: " + info.nodeArrayName);

    for (auto& param : info.parameters) {
        ui->formLayout->addRow(param->schema->name,
                                 makeEditingWidget(param.get()));
    }
}

InfoForm::~InfoForm()
{
    delete ui;
}
