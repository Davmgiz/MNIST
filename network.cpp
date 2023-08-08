#include "network.h"
#include "matrix.h"
#include <iostream>
#include <cmath>
#include <functional>
#include <random>
#include <fstream>
#include <utility>
#include <QDebug>

using namespace std;

//конструктор нейросети который принимает вектор, размер вектора количество слоев в сети, а значения количество нейронов в соответствующем слое
//инициализирует веса и смещения рандомным способом
Network::Network(vector<int> sizes) {
    this->num_layers = sizes.size();
    this->sizes = sizes;

    vector<Matrix> b;
    for (int i = 1; i < num_layers; i++) {
        Matrix v(sizes[i], 1);
        b.push_back(v);
    }
    this->biases = b;

    vector<Matrix> w;
    for (int i = 0; i < sizes.size() - 1; i++) {
        w.push_back(Matrix(sizes[i + 1], sizes[i]));
    }
    this->weights = w;
}

//конструктор сети который принимает файл с архитектурой сети и весами и смещениями
Network::Network(string file_name) {
    ifstream file(file_name);
    if (!file.is_open()) {
        cout << "Failed to open the file." << endl;
    }

    int nl;
    file >> nl;

    vector<int> s;
    for (int i = 0; i < nl; i++) {
        int x;
        file >> x;
        s.push_back(x);
    }

    vector<Matrix> w;
    for (int i = 0; i < nl - 1; i++) {
        std::vector<double> x;
        for (int j = 0; j < s[i + 1] * s[i]; j++) {
            double a;
            file >> a;
            x.push_back(a);
        }
        Matrix el = create_matrix(x, s[i + 1], s[i]);
        w.push_back(el);
    }

    vector<Matrix> b;
    for (int i = 1; i < nl; i++) {
        vector<double> x;
        for (int j = 0; j < s[i]; j++) {
            double a;
            file >> a;
            x.push_back(a);
        }
        Matrix el = create_matrix(x, s[i], 1);
        b.push_back(el);
    }

    this->sizes = s;
    this->biases = b;
    this->weights = w;
    this->num_layers = nl;
}

//целевая фунция потерь - кросс энтропия c регулеризацией L2
//тут только произоводная функции кросс энтропии
Matrix Network::coast_derivative(Matrix act, Matrix y) {
    return (act - y);
}
//прямое распространение
Matrix Network::feedforward(Matrix x) {


    for (int i = 0; i < num_layers - 1; i++) {
        x = ((weights[i].dot(x)) + biases[i]).my_for_each(sigmoid);
        /*cout << "&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;

        x.show();

        cout << "&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;*/

    }
    return x;

}

//стахостический градиентый спуск
//epochs количество эпох
//mini_batch_size размер пакета
//eta скорость обучения градиентного спуска
//lmd устанавливает важность весов в целевой функции, регулеризация L2
void Network::SGD(vector<pair<Matrix, Matrix>>& data_train, int epochs, int mini_batch_size, double eta, double lmd) {
    int data_train_size = data_train.size();

    for (int i = 0; i < epochs; i++) {
        //выводим эпоху
        cout << "epochs: " << i + 1 << endl;
        //перемешиваем данные это надо потому что у меня стахостический градиентый спуск
        my_shuffled(data_train);
        //формируем мини-батчи
        for (int j = 0; j < data_train_size - mini_batch_size; j += mini_batch_size) {
            vector<pair<Matrix, Matrix>> mini_batch;
            copy(data_train.begin() + j, data_train.begin() + j + mini_batch_size, back_inserter(mini_batch));
            //обновляем веса и смещения на основе данных мини-батча
            update_mini_batch(mini_batch, eta, lmd);
        }
        //мы можем выбрать такой размер мини-батча что при делении размера обучающих данных на размер мини-батча будет не нулевой остаток
        //поэтому я обрабатываю этот случай
        if (data_train_size % mini_batch_size != 0) {
            if (data_train_size % mini_batch_size == 1) {
                vector<pair<Matrix, Matrix>> mini_batch;
                mini_batch.push_back(data_train[data_train_size - 1]);
                update_mini_batch(mini_batch, eta, lmd);
            }
            else {
                vector<pair<Matrix, Matrix>> mini_batch;
                copy(data_train.begin() + (data_train.size() - data_train_size % mini_batch_size), data_train.end(), back_inserter(mini_batch));
                update_mini_batch(mini_batch, eta, lmd);
            }
        }
    }
}

//обновляем веса и смещения на основе данных мини-батча
//eta скорость обучения градиентного спуска
//lmd устанавливает важность весов в целевой функции, регулеризация L2
//мини-батч это вектор из пар, где первое значение это входные данные, а второе желаемые выходные
void Network::update_mini_batch(vector<pair<Matrix, Matrix>> mini_batch, double eta, double lmd) {
    //создаем векторы из нулевых матриц идентичные векторам весов и смещений чтобы найти градиенты
    vector<Matrix> nabla_w = zeros(weights);
    vector<Matrix> nabla_b = zeros(biases);

    for (int i = 0; i < mini_batch.size(); i++) {
        Matrix x = mini_batch[i].first;
        Matrix y = mini_batch[i].second;
        //обратное распространение
        pair<vector<Matrix>, vector<Matrix>> bw = backprop(x, y);

        vector<Matrix> delta_nabla_b = bw.first;
        vector<Matrix> delta_nabla_w = bw.second;
        //заполняем градиенты
        for (int j = 0; j < nabla_w.size(); j++) {
            nabla_w[j] = nabla_w[j] + delta_nabla_w[j];
        }

        for (int j = 0; j < nabla_b.size(); j++) {
            nabla_b[j] = nabla_b[j] + delta_nabla_b[j];
        }
    }

    double k = eta / mini_batch.size();
    //магия регуляризации
    double wk = 1 - eta * (lmd / 1000);
    // обновляем веса
    for (int j = 0; j < weights.size(); j++) {
        weights[j] = (weights[j].my_for_each([wk](double& b) { b *= wk; })) - (nabla_w[j].my_for_each([k](double& a) { a *= k; }));
    }
    //обновляем смещения
    for (int j = 0; j < biases.size(); j++) {
        biases[j] = biases[j] - (nabla_b[j].my_for_each([k](double& a) { a *= k; }));
    }
}

//обратное распространение
pair<vector<Matrix>, vector<Matrix>> Network::backprop(Matrix x, Matrix y) {
    //сюда склдываем градиент по 1 обучающему примеру
    vector<Matrix> nabla_w = zeros(weights);
    vector<Matrix> nabla_b = zeros(biases);
    //вектор из активаций для каждого слоя
    Matrix activation = x;
    vector<Matrix> activations;
    activations.push_back(activation);
    //вектор из взвешанных сумм
    vector<Matrix> zs;
    // заполняем 2 вышепреведенных вектра
    for (int i = 0; i < weights.size(); i++) {
        Matrix z = (weights[i].dot(activation)) + biases[i];

        zs.push_back(z);

        activation = z.my_for_each(sigmoid);
        activations.push_back(activation);
    }
    //оштбка для выходного слоя
    Matrix delta = coast_derivative(activations[activations.size() - 1], y);
    //ошибка слоя это производная по смещениям
    nabla_b[nabla_b.size() - 1] = delta;
    //находим с помощью ошибки производную по весам
    Matrix lat = activations[activations.size() - 2].T();
    nabla_w[nabla_w.size() - 1] = delta.dot(lat);
    //донаходим градиенты в оставшихся слоях
    for (int i = 2; i < num_layers; i++) {
        Matrix z = zs[zs.size() - i];
        delta = (weights[weights.size() - i + 1].T().dot(delta)) * (z.my_for_each(sigmoid_prime));

        nabla_b[nabla_b.size() - i] = delta;
        Matrix lat2 = activations[activations.size() - i - 1].T();
        nabla_w[nabla_w.size() - i] = delta.dot(lat2);
    }
    //возвращаем градиент по смещеням и градиент по весам
    return make_pair(nabla_b, nabla_w);
}

//проверяет количество правильно отгаданных примеров
int Network::number_of_correct(vector<pair<Matrix, Matrix>> test_data) {
    int cnt = 0;

    for (int i = 0; i < test_data.size(); i++) {
        pair<Matrix, Matrix> xy = test_data[i];
        Matrix x = xy.first;
        Matrix y = xy.second;
        Matrix y_pred = feedforward(x);

        int max_k_pred = -1;
        int max_j_pred = -1;
        double my_max = -1000000;

        for (int k = 0; k < y_pred.get_rows(); k++) {
            for (int j = 0; j < y_pred.get_columns(); j++) {
                if (y_pred.matr[k][j] > my_max) {
                    my_max = y_pred.matr[k][j];
                    max_k_pred = k;
                    max_j_pred = j;
                }
            }
        }

        int max_k = -1;
        int max_j = -1;
        my_max = -100000000;

        for (int k = 0; k < y.get_rows(); k++) {
            for (int j = 0; j < y.get_columns(); j++) {
                if (y.matr[k][j] > my_max) {
                    my_max = y.matr[k][j];
                    max_k = k;
                    max_j = j;
                }
            }
        }

        if (max_k_pred == max_k && max_j_pred == max_j) {
            cnt++;
        }
    }
    return cnt;
}

//выводим в файл ахитектуру сети и веса
void Network::wout(string file_name) {
    ofstream file(file_name);

    file << num_layers << endl;

    for (int i = 0; i < num_layers; i++) {
        file << sizes[i] << " ";
    }
    file << endl;

    for (int i = 0; i < weights.size(); i++) {
        for (int j = 0; j < weights[i].get_rows(); j++) {
            for (int k = 0; k < weights[i].get_columns(); k++) {
                file << weights[i].matr[j][k] << " ";
            }
        }
    }
    file << endl;
    file << endl;

    for (int i = 0; i < biases.size(); i++) {
        for (int j = 0; j < biases[i].get_rows(); j++) {
            for (int k = 0; k < biases[i].get_columns(); k++) {
                file << biases[i].matr[j][k] << " ";
            }
        }
    }
}

//преобразуем полученный от сети вектор в число
int vec2num(Matrix y) {
    double my_max = -1000;
    int index = -10;
    for (int i = 0; i <10 ; i++) {
        double val = y.matr[i][0];
        if (val > my_max) {
            index = i;
            my_max = val;
        }
    }
    return index;
}

//принимает входные данные возвращает предсказанное число
int Network::pred_num_mnist(Matrix x) {

    Matrix y_pred = feedforward(x);
    int res = vec2num(y_pred);
    return res;
}

//преобразует число в вектор
Matrix num2vec(int y) {
    Matrix Y = create_matrix(vector<double>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 10, 1);
    Y.matr[y][0] = 1.0;

    return Y;
}

//считывает данные из файла
vector<pair<Matrix, Matrix>> read_data(string file_name, int n, bool flag) {
    ifstream file(file_name);

    vector<pair<Matrix, Matrix>> res;
    if (flag) {
        res.reserve(60060);
    }
    else {
        res.reserve(10001);
    }

    int cnt = 0;
    int cnt0 = 0;
    for (int i = 0; i < n; i++) {
        int y;
        file >> y;
        if (y == 0) {
            cnt0++;
        }
        Matrix Y = num2vec(y);

        vector<double> x;
        for (int j = 0; j < 784; j++) {
            double buf;
            file >> buf;
            x.push_back(buf);
        }
        Matrix X = create_matrix(x, 784, 1);

        pair<Matrix, Matrix> p(X, Y);
        res.push_back(p);
        cout << cnt << endl;
        cnt++;
    }
    cout << "cnt0 = " << cnt0 << endl;

    return res;
}

//производная сигмоидальной функции
void sigmoid_prime(double& z) {
    z = double_sigmoid(z) * (1 - double_sigmoid(z));
}

//возвращет значение сигмодальной функции
double double_sigmoid(double& z) {
    return 1 / (1 + exp(-z));
}

//получает по ссылке и изменяет полученное аргумент на занчение сигмоидальной функции
void sigmoid(double& z) {
    z = 1 / (1 + exp(-z));
}
