#include "pixel.h"
#include <QGraphicsSceneMouseEvent>

//класс моего пикселя, размер пикселя будет 20 на 20, всего 28 на 28 моих пикселей

//наследуем от класса QGraphicsRectItem, он позволяет настроить множество параметров прямоугольника,
//включая его положение, размер, цвет кисти и перо
Pixel::Pixel(int x, int y) : QGraphicsRectItem(x * 20, y * 20, 20, 20)
{
    setBrush(Qt::white); //устанавливаем белый цвет по умолчанию
    color_state = 0;
}
//обработчик нажатий на мышь
void Pixel::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsRectItem::mousePressEvent(event);
}
//геттер
int Pixel::colorState()
{
    return color_state;
}
//сеттер
//устанавливаем цвет пикселя и изменяем соответствующи поле color_state
void Pixel::setColor(QColor color)
{
    setBrush(color); //устанавливаем нужный цвет
    color_state = (color == Qt::white) ? 0 : 1;
}
