#include "numscreen.h"

NumScreen::NumScreen():QLabel()
{
    setFont(QFont("Arial", 200));//устанавливаем шрифт и размер
    setAlignment(Qt::AlignCenter);//выравнивание по центру
    setFixedSize(580, 240);//устанавливаем размер экрана
}

//установить цифру
void NumScreen::setNum(int num){
    setText(QString::number(num));
}
//ничего не установить
void NumScreen::setN(){
    setText(" ");
}
