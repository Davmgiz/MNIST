#ifndef SCREEN_H
#define SCREEN_H

#include <QGraphicsScene>
#include "matrix.h"
#include "pixel.h"

//класс экрана
//экран состоит из реализованных пикселей

class Screen
{
public:
    Screen();
    ~Screen();

    void populate(QGraphicsScene *scene);
    bool isAnyPixelBlack();//если есть хотя бы 1 черный пиксель то возвращаем true, иначе false
    void clearScreen();// делаем все пиксели белыми
    Matrix getNum();//создаем матрицу из занчений пикселей


private:
    const static int WIDTH = 28; //ширина экрана
    const static int HEIGHT = 28; //высота экрана

    Pixel* pixels_[WIDTH][HEIGHT];
};

#endif // SCREEN_H
