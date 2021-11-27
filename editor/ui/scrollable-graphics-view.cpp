#include "scrollable-graphics-view.h"
#include "moc_scrollable-graphics-view.cpp"

#include <QWheelEvent>
#include <QtMath>

ScrollableGraphicsView::ScrollableGraphicsView(QWidget* parent) : QGraphicsView(parent)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

void ScrollableGraphicsView::wheelEvent(QWheelEvent* event)
{
    qreal scaleFactor = qPow(2.0, event->angleDelta().y() / 240.0);
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();

    if (0.05 < factor && factor < 10)
        scale(scaleFactor, scaleFactor);
}
