#include "parsetree-graph.h"

ParseTreeGraph::ParseTreeGraph(ScrollableGraphicsView *pGraphivsView)
    :
      graphicsView(pGraphivsView),
      qgvScene(new QGVScene("Parse Tree"))
{

}

void ParseTreeGraph::addNode(QString lable , int frequency)
{
    QGVNode *newNode = qgvScene->addNode(lable , frequency);
    nodeTable[lable] = newNode;
}

void ParseTreeGraph::addEdge(QString node1 , QString node2 , int weight)
{
    qgvScene->addEdge(nodeTable.value(node1), nodeTable.value(node2), QString::number(weight));
}

void ParseTreeGraph::addGraphAttribute(QString name , QString value)
{
    qgvScene->setGraphAttribute(name, value);
}

void ParseTreeGraph::addNodeAttribute(QString name , QString value)
{
    qgvScene->setNodeAttribute(name, value);
}

void ParseTreeGraph::draw() {
    qgvScene->applyLayout();
    graphicsView->fitInView(qgvScene->sceneRect(), Qt::KeepAspectRatio);
}
