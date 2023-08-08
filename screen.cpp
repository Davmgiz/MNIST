#include "screen.h"
#include "pixel.h"
//класс экрана
//экран состоит из реализованных пикселей
Screen::Screen()
{
    for (int i = 0; i < HEIGHT; ++i)
    {
        for (int j = 0; j < WIDTH; ++j)
        {
            pixels_[i][j] = new Pixel(j, i);
        }
    }
}

Screen::~Screen()
{
    for (int i = 0; i < HEIGHT; ++i)
    {
        for (int j = 0; j < WIDTH; ++j)
        {
            delete pixels_[i][j];
        }
    }
}
//добаввляет экран на графическую сцену
void Screen::populate(QGraphicsScene *scene)
{
    for (int i = 0; i < HEIGHT; ++i)
    {
        for (int j = 0; j < WIDTH; ++j)
        {
            scene->addItem(pixels_[i][j]);
        }
    }
}
//если хотябы 1 пиксель черный на экране то возвращает true, иначе false
bool Screen::isAnyPixelBlack()
{
    for (int i = 0; i < HEIGHT; ++i)
    {
        for (int j = 0; j < WIDTH; ++j)
        {
            if (pixels_[i][j]->colorState() == 1)
                return true;
        }
    }
    return false;
}
//функция отчищает экран
void Screen::clearScreen()
{
    for (int i = 0; i < HEIGHT; ++i)
    {
        for (int j = 0; j < WIDTH; ++j)
        {
            pixels_[i][j]->setColor(Qt::white);
        }
    }
}
//создаем матрицу из занчений пикселей
Matrix Screen::getNum()
{
    vector<double> buf;

    for (int i = 0; i < HEIGHT; ++i)
    {
        for (int j = 0; j < WIDTH; ++j)
        {
            buf.push_back(pixels_[i][j]->colorState());
        }
    }
    Matrix num = create_matrix(buf, 784, 1);
    return num;
}
