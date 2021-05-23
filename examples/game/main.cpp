#include <nodes/NodeData>
#include <nodes/FlowScene>
#include <nodes/FlowView>

#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QMenuBar>

#include "ModelFromSchema.hpp"
#include "InfoForm.h"

using QtNodes::DataModelRegistry;
using QtNodes::FlowScene;
using QtNodes::FlowView;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget mainWidget;

    auto menuBar    = new QMenuBar();
    auto saveAction = menuBar->addAction("Save scene");
//    auto saveJsonAction = menuBar->addAction("Save JSON");
    auto loadAction = menuBar->addAction("Load scene");
    auto infoAction = menuBar->addAction("Show/hide general info");
    infoAction->setCheckable(true);
    infoAction->setChecked(true);

    QVBoxLayout *l = new QVBoxLayout(&mainWidget);

    l->addWidget(menuBar);
    auto registry = std::make_shared<DataModelRegistry>();
    JsonSchemaParser jsonParser(*registry);

    auto scene = new FlowScene(registry, &mainWidget);
    auto view = new FlowView(scene);
    l->addWidget(view);
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(0);

    QObject::connect(saveAction, &QAction::triggered,
                     scene, &FlowScene::save);

    QObject::connect(loadAction, &QAction::triggered,
                     scene, &FlowScene::load);


    InfoForm infoForm(jsonParser.info);
    l->addWidget(&infoForm);
    QObject::connect(infoAction, &QAction::toggled,
                     &infoForm, &InfoForm::setVisible);

    mainWidget.setWindowTitle("Node editor");
    mainWidget.resize(800, 600);
    mainWidget.showNormal();

    return app.exec();
}
