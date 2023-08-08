#ifndef DRAWSCENE_H
#define DRAWSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include "network.h"
#include "numscreen.h"
#include "screen.h"

//графическая сцена

class DrawScene : public QGraphicsScene
{
    Q_OBJECT
public:

    DrawScene(QObject *parent = nullptr, Network* net = nullptr, NumScreen* ns = nullptr, Screen* sc = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    void drawPixel(QGraphicsSceneMouseEvent *event, QColor color);

private:
    bool is_drawing;//булевая переменная отвечающая на вопрос: сейчас рисуеют?
    QColor current_color;//отвечает за цвет которым сейчас рисуют белый/черный

    Network* net; //указатель на нейросеть
    NumScreen* ns;//указатель на экран с цифрой
    Screen* sc;//указатель на экран с пикселями
};

#endif // DRAWSCENE_H
