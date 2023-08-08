#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "matrix.h"
#include <vector>
#include <string>
#include <utility>

using namespace std;

class Network {
private:
    int num_layers; //количество слоев сети
    vector<int> sizes; //размеры слоев
    vector<Matrix> biases; // смещения
    vector<Matrix> weights; // веса
public:

    Network(vector<int> sizes); //конструктор нейросети который принимает вектор из количеств нейронов в кажом слое
    Network(string file_name); //конструктор который принимает файл с архитектурой и с весами и смещениями

    Matrix coast_derivative(Matrix act, Matrix y); //функция потерь
    Matrix feedforward(Matrix x); //прямой распространение
    void SGD(vector<pair<Matrix, Matrix>>& data_train, int epochs, int mini_batch_size, double eta, double lmd); //стахостический градиентный спуск
    void update_mini_batch(vector<pair<Matrix, Matrix>> mini_batch, double eta, double lmd); //создаем градиент на основе пакета
    pair<vector<Matrix>, vector<Matrix>> backprop(Matrix x, Matrix y); //обратное распространение
    int number_of_correct(vector<pair<Matrix, Matrix>> test_data); //количество правильно отгаданных цифр
    void wout(string file_name); //вывод архетектуры сети а так же весов и смещений
    int pred_num_mnist(Matrix x); //предсказать цифру
};

int vec2num(Matrix y); //вектор в число
Matrix num2vec(int y); // число в вектор
vector<pair<Matrix, Matrix>> read_data(string file_name, int n, bool flag = true); //функция по счтыванию данных из файла

void sigmoid(double& z); //сигмоидальная функция
double double_sigmoid(double& z); //сигмоидальная функция
void sigmoid_prime(double& z); //производная сигмоидальной функции

#endif  // NETWORK_HPP
