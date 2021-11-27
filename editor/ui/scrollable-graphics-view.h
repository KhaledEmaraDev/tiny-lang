#ifndef SCROLLABLEGRAPHICSVIEW_H
#define SCROLLABLEGRAPHICSVIEW_H

#include <QGraphicsView>

class ScrollableGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    ScrollableGraphicsView(QWidget *parent = 0);

protected:
    virtual void wheelEvent(QWheelEvent* event);
};

#endif // SCROLLABLEGRAPHICSVIEW_H
