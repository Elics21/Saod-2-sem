#define YES true
#define NO false
#pragma warning(disable : 4996)
#include <fstream>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <stdio.h>
#include <conio.h>
#include <cmath>
#include <iomanip>

using namespace std;

#define n 256
#define M 4 + 1
#define count 6


// Структура для представления символов и соответствующих им вероятностей
struct code {
    char a;     // Символ
    float p;    // Вероятность символа в исходном тексте
    float q;    // Кумулятивная вероятность
    float l;    // Нижняя граница интервала для арифметического кодирования
    float h;    // Верхняя граница интервала для арифметического кодирования
    float r;    // Длина интервала для арифметического кодирования
};

// Функция для арифметического кодирования
void Arifm();

// Массивы и переменные
code A[n];
code B[M];
code V[count];
FILE* f;
int sum = 0;

int main() {
    // Установка русской локали
    setlocale(LC_ALL, "Russian");

    // Открытие файла для чтения
    f = fopen("text.txt", "r");

    // Инициализация массива A
    for (int i = 0; i < n; i++) {
        A[i].a = (char)(i - 128);
        A[i].p = 0;
        A[i].q = 0;
        A[i].l = 0;
        A[i].h = 0;
        A[i].r = 0;
    }

    // Подсчет вероятностей
    while (!feof(f)) {
        char c;
        fscanf(f, "%c", &c);
        A[c + 128].p++;
        sum++;
    }
    fclose(f);

    // Инициализация массива B
    int j = 1;
    for (int i = 0; i < n; i++) {
        if (A[i].p != 0) {
            A[i].p /= sum;
            B[j] = A[i];
            j++;
        }
    }

    // Вычисление кумулятивных вероятностей
    for (int i = 0; i < M; i++) {
        B[i].q = B[i - 1].q + B[i].p;
    }

    // Арифметическое кодирование
    Arifm();

    // Вывод результата
    for (int i = 1; i < M; i++) {
        cout << "| " << B[i].a << " |   " << setw(10) << B[i].p << "    |   " << setw(10) << B[i].q << "  |\n";
    }

    cout << endl;

    for (int i = 1; i < count; i++) {
        cout << "| " << V[i].a << " |   " << setw(10) << V[i].l << "    |  " << setw(10) << V[i].h << "   |   " << setw(10) << V[i].r << "|\n";
    }

    // Завершение программы
    system("pause");
    return 0;
}

// Реализация функции арифметического кодирования
void Arifm() {
    int m = 0;
    V[0].l = 0;
    V[0].h = 1;
    V[0].r = 1;
    int i = 1;

    // Повторное открытие файла для чтения
    f = fopen("text.txt", "rb");

    // Арифметическое кодирование символов
    while (!feof(f)) {
        char c;
        fscanf(f, "%c", &c);
        for (int j = 1; j < M; j++) {
            if (c == B[j].a) {
                m = j;
            }
        }
        V[i].a = B[m].a;
        V[i].l = V[i - 1].l + V[i - 1].r * B[m - 1].q;
        V[i].h = V[i - 1].l + V[i - 1].r * B[m].q;
        V[i].r = V[i].h - V[i].l;
        i++;
        if (i == count) break;
    }
    fclose(f);
}