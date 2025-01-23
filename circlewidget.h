#ifndef CIRCLEWIDGET_H
#define CIRCLEWIDGET_H

#include <QWidget>

class CircleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CircleWidget(QWidget *parent = nullptr);

    // Methods to update the circle's parameters
    void setStartAngle(float angle);
    void setArriveAngle(float angle);
    void setTurnDirection(const QString &direction);

protected:
    void paintEvent(QPaintEvent *event) override;  // Handle custom drawing

private:
    float startAngle;     // Start angle in degrees
    float arriveAngle;    // Arrive angle in degrees
    QString turnDirection;  // Turn direction ("Clockwise" or "Counterclockwise")

    QPointF calculatePointOnCircle(float angle, float radius, const QPointF &center);
};

#endif // CIRCLEWIDGET_H
