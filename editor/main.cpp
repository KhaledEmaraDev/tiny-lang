#include "ui/main-window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //Add Nodes Like This
    w.parseTree->addNode("if" , 0);
    w.parseTree->addNode("op" , 1);

    //Add Edges Like This
    w.parseTree->addEdge("if", "op", 1);

    //Add Attributes Like This
    w.parseTree->addGraphAttribute("rankdir", "TB");
    w.parseTree->addNodeAttribute("style", "filled");
    w.parseTree->addNodeAttribute("fillcolor", "white");
    w.parseTree->addNodeAttribute("height", "0.6");
    w.parseTree->addNodeAttribute("margin", "0.1");

    w.parseTree->draw();

    w.show();
    return a.exec();
}
