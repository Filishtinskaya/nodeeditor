#include "NodeForm.h"
#include "ui_NodeForm.h"

NodeForm::NodeForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NodeForm)
{
    ui->setupUi(this);
}

NodeForm::~NodeForm()
{
    delete ui;
}
