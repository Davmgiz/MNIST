#include "drawscene.h"
#include "pixel.h"
#include "screen.h"
#include "numscreen.h"
#include "network.h"
#include "matrix.h"
#include <QApplication>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QPushButton>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QWidget window;
    QVBoxLayout layout(&window);

    //создаем кнопку
    QPushButton clearButton("Clear");

    //создаем экран для цифры
    NumScreen numScreen;

    //добавляем
    layout.addWidget(&clearButton);
    layout.addWidget(&numScreen);

    //создаем нейроную сеть
    Network ura("D:\\qt\\mnist2\\res.txt");

    //создаем экран из пикселей
    Screen screen;

    //создаем графическую сцену
    DrawScene scene(nullptr, &ura, &numScreen, &screen);
    screen.populate(&scene);

    QGraphicsView view(&scene);
    view.setFixedSize(560, 560);
    //запрещаем скролить графическую сцену
    view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //добавляем графическую сцену
    layout.addStretch();
    layout.addWidget(&view);
    layout.addStretch();

    //выравнивание по центру графической сцены
    layout.setAlignment(&view, Qt::AlignCenter);

    //лямда функция кнопки
    QObject::connect(&clearButton, &QPushButton::clicked, [&](){
        screen.clearScreen();
        numScreen.setN();
    });

    window.show();

    return app.exec();
}
//996



