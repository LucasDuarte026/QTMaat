#include "circlewidget.h"
#include <QPainter>
#include <QtMath>

CircleWidget::CircleWidget(QWidget *parent)
    : QWidget(parent), startAngle(0.0), arriveAngle(0.0), turnDirection("Clockwise")
{
}

void CircleWidget::setStartAngle(float angle)
{
    startAngle = angle;
    update();  // Trigger a repaint
}

void CircleWidget::setArriveAngle(float angle)
{
    arriveAngle = angle;
    update();  // Trigger a repaint
}

void CircleWidget::setTurnDirection(const QString &direction)
{
    turnDirection = direction;
    update();  // Trigger a repaint
}

void CircleWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Circle parameters
    float radius = qMin(width(), height()) / 2 - 10;  // Ensure it fits within the widget
    QPointF center(width() / 2, height() / 2);        // Center of the circle

    // Draw main circle
    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(center, radius, radius);

    // Calculate points for start and arrive angles
    QPointF startPoint = calculatePointOnCircle(startAngle, radius, center);
    QPointF arrivePoint = calculatePointOnCircle(arriveAngle, radius, center);

    // Draw start point
    painter.setBrush(Qt::green);
    painter.drawEllipse(startPoint, 10, 10);  // Small circle at start point

    // Draw arrive point
    painter.setBrush(Qt::red);
    painter.drawEllipse(arrivePoint, 10, 10);  // Small circle at arrive point
}

QPointF CircleWidget::calculatePointOnCircle(float angle, float radius, const QPointF &center)
{
    // Convert angle from degrees to radians
    float radians = qDegreesToRadians(angle);

    // Calculate x and y based on the angle
    float x = center.x() + radius * qCos(radians);
    float y = center.y() - radius * qSin(radians);  // Y is inverted in Qt's coordinate system

    return QPointF(x, y);
}
