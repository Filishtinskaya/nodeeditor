#include <nodes/NodeData>
#include <nodes/FlowScene>
#include <nodes/FlowView>

#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QMenuBar>

#include "ModelFromSchema.hpp"

using QtNodes::DataModelRegistry;
using QtNodes::FlowScene;
using QtNodes::FlowView;

static std::shared_ptr<DataModelRegistry> registerDataModels()
{
  auto ret = std::make_shared<DataModelRegistry>();
  JsonSchemaParser jsonParser(*ret);
  jsonParser.fetchModels();
  return ret;
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget mainWidget;

    auto menuBar    = new QMenuBar();
    auto saveAction = menuBar->addAction("Save scene");
//    auto saveJsonAction = menuBar->addAction("Save JSON");
    auto loadAction = menuBar->addAction("Load scene");

    QVBoxLayout *l = new QVBoxLayout(&mainWidget);

    l->addWidget(menuBar);
    auto scene = new FlowScene(registerDataModels(), &mainWidget);
    l->addWidget(new FlowView(scene));
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(0);

    QObject::connect(saveAction, &QAction::triggered,
                     scene, &FlowScene::save);

    QObject::connect(loadAction, &QAction::triggered,
                     scene, &FlowScene::load);

    mainWidget.setWindowTitle("Node editor");
    mainWidget.resize(800, 600);
    mainWidget.showNormal();

    return app.exec();
}
