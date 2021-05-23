#ifndef INFOFORM_H
#define INFOFORM_H

#include <QWidget>
#include "ParametersWidget.h"

namespace Ui {
class InfoForm;
}

class InfoForm : public ParametersWidget
{
public:
    explicit InfoForm(GeneralInfo& aInfo);
    ~InfoForm();

private:
    Ui::InfoForm *ui;
    GeneralInfo& info;
};

#endif // INFOFORM_H
