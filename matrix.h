#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <functional>
#include <utility>

using namespace std;

class Matrix {
private:
    int columns;
    int rows;

public:
    vector<vector<double>> matr;

    int get_columns();
    int get_rows();

    Matrix(int row, int columns);
    Matrix();

    Matrix dot(Matrix& B); //обычное преремножение матриц
    void show(); //вывод матрицы
    void set_matr(vector<double> vec); //установить матрицу с помощью вектора

    Matrix operator+(const Matrix& A); //сложение матриц
    Matrix operator-(const Matrix& A); //разность матриц
    Matrix operator*(const Matrix A); //адамарное произведение матриц
    Matrix T();//транспонирование
    void operator=(Matrix other); //присваивание матриц
    Matrix my_for_each(function<void(double&)> f); // поэлементная обработка матрицы
    friend Matrix create_matrix(vector<double> vec, int rows, int columns);
};

Matrix create_matrix(vector<double> vec, int rows, int columns); //создание матрицы
vector<Matrix> zeros(vector<Matrix> matr); //создание вектора из нулевых матриц
void my_shuffled(vector<pair<Matrix, Matrix>>& data_train); //перемешивание вектора

#endif  // MATRIX_HPP


