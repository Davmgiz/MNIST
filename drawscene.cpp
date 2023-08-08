#include "drawscene.h"
#include "pixel.h"
#include "screen.h"
#include <QDebug>

//графическая сцена

//конструктор
DrawScene::DrawScene(QObject *parent, Network* net, NumScreen* ns, Screen* sc) : QGraphicsScene(parent), is_drawing(false), current_color(Qt::black)
{
    this->net = net;
    this->ns = ns;
    this->sc = sc;
}

//обработчик нажатий мыши
void DrawScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    is_drawing = true;
    //когда нажимаем на левую кнопку рисует черным, если на правую то белым
    current_color = (event->button() == Qt::LeftButton) ? Qt::black : Qt::white;
    drawPixel(event, current_color);
}

//обработчик перемещений мыши
void DrawScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
    drawPixel(event, current_color);
}
//обработчик отпусканий кнопки мыши
void DrawScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
    is_drawing = false;
}
//изменняет цвет пикселя на позиции курсора
void DrawScene::drawPixel(QGraphicsSceneMouseEvent *event, QColor color)
{
    if (is_drawing)
    {
        //если под позицией курсора нету пикселей то pixel указывает на nullptr
        //если под позицией курсора есть пиксель то pixel указывает на него
        Pixel *pixel = dynamic_cast<Pixel*>(itemAt(event->scenePos(), QTransform()));
        if (pixel)
        {
            pixel->setColor(color);
            //проверяем есть ли черный пиксель
            if (sc->isAnyPixelBlack())
            {

                //получаем матрицу из пикселей
                Matrix x = sc->getNum();
                //предсказываем число
                int y = net->pred_num_mnist(x);
                //выводим это число
                ns->setNum(y);
            }
            else
            {
                //ничего не выводим
                ns->setN();
            }
        }
    }
}
