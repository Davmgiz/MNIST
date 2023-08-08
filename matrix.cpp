#include "Matrix.h"
#include <random>
#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>
#include <QDebug>

using namespace std;

int Matrix::get_columns() {
    return columns;
}

int Matrix::get_rows() {
    return rows;
}

Matrix::Matrix(int row, int columns) {


    this->rows = row;
    this->columns = columns;

    vector<vector<double>> matrix;

    random_device rd2;
    mt19937 gen2(rd2());
    normal_distribution<double> dist2(0.0, 1.0);

    for (int i = 0; i < rows; i++) {
        vector<double> v;

        for (int j = 0; j < columns; j++) {
            double elem = dist2(gen2);
            v.push_back(elem);
        }

        matrix.push_back(v);
    }
    matr = matrix;
}

Matrix::Matrix() {
    columns = 0;
    rows = 0;
}

//обычное перемножение матриц
Matrix Matrix::dot(Matrix& B) {


    Matrix C(this->rows, B.columns);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < B.columns; j++) {
            double sum = 0;

            for (int k = 0; k < columns; k++) {
                sum += matr[i][k] * B.matr[k][j];
            }
            C.matr[i][j] = sum;
        }
    }
    return C;
}

//вывод матрицы на консоль
void Matrix::show() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            cout << this->matr[i][j] << " ";
        }
        cout << endl;
    }
}

//изменить матрицу на значения из вектора
void Matrix::set_matr(vector<double> vec) {


    int k = 0;
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->columns; j++) {
            this->matr[i][j] = vec[k];
            k++;
        }
    }
}

//сложение матриц
Matrix Matrix::operator+(const Matrix& A) {


    Matrix C(rows, columns);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            C.matr[i][j] = matr[i][j] + A.matr[i][j];
        }
    }
    return C;
}

//разность матриц
Matrix Matrix::operator-(const Matrix& A) {


    Matrix C(rows, columns);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            C.matr[i][j] = matr[i][j] - A.matr[i][j];
        }
    }
    return C;
}

//адомарное произведение матриц
Matrix Matrix::operator*(const Matrix A) {


    Matrix C(rows, columns);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            C.matr[i][j] = matr[i][j] * A.matr[i][j];
        }
    }
    return C;

}

//транспонирование
Matrix Matrix::T() {
    Matrix A(columns, rows);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            A.matr[j][i] = matr[i][j];
        }
    }
    return A;
}

//присваивание
void Matrix::operator=(Matrix other) {
    rows = other.rows;
    columns = other.columns;
    matr = other.matr;
}

//изменение элементов матрицы с помощью функции которая применяется поэлементно
Matrix Matrix::my_for_each(function<void(double&)> f) {
    for (int i = 0; i < rows; i++) {
        for_each(matr[i].begin(), matr[i].end(), f);
    }
    return *this;
}

//создание матрицы из вектора
Matrix create_matrix(vector<double> vec, int rows, int columns) {
    Matrix res(rows, columns);
    int k = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            res.matr[i][j] = vec[k];
            k++;
        }
    }
    return res;
}

//перемешивание вектора
void my_shuffled(vector<pair<Matrix, Matrix>>& data_train) {

    random_device rd;
    mt19937 gen(rd());
    shuffle(data_train.begin(), data_train.end(), gen);

}

//создание вектора из матриц той же размерности, но заполненую нулями
vector<Matrix> zeros(vector<Matrix> matr) {

    for (int i = 0; i < matr.size(); i++) {
        matr[i].my_for_each([](double& a) {a = 0; });
    }
    return matr;

}
