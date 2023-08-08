#ifndef NUMSCREEN_H
#define NUMSCREEN_H

#include <QLabel>
//экран для вывода предсказанной цифры
class NumScreen:public QLabel
{
public:
    NumScreen();
    void setNum(int num);
    void setN();
};

#endif // NUMSCREEN_H
