#ifndef PIXEL_H
#define PIXEL_H

#include <QGraphicsRectItem>
#include <QBrush>

class Pixel : public QGraphicsRectItem
{
public:
    Pixel(int x, int y);//констуктор

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override; //обработчик нажатий на мышь
    int colorState(); //гетер
    void setColor(QColor color); //сеттер

private:
    QBrush brush; //кисть
    int color_state; //состояние пикселя 0 для белого, 1 черного
};

#endif // PIXEL_H
