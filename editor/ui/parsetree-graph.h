#ifndef PARSETREE_GRAPH_H
#define PARSETREE_GRAPH_H

#include <QHash>

#include "QGVScene.h"

#include "scrollable-graphics-view.h"

class ParseTreeGraph
{
public:
    ParseTreeGraph(ScrollableGraphicsView *);

    void addNode(QString lable, int frequency);
    void addEdge(QString node1, QString node2, int weight);
    void addGraphAttribute(QString name, QString value);
    void addNodeAttribute(QString name, QString value);
    void draw();

private:
    ScrollableGraphicsView *graphicsView;
    QGVScene *qgvScene;
    QHash<QString, QGVNode *> nodeTable;

    friend class MainWindow;
};

#endif // PARSETREE_GRAPH_H
