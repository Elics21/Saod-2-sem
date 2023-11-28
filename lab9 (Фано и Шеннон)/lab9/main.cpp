#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>

using namespace std;

struct chanceSymbol
{
    unsigned char ch = 0; //символ
    float chance = 0; //вероятность символа в тексте
};

struct codeShannon
{
    unsigned char ch = 0; //символ
    float Pi = 0; //вероятность символа в тексте
    float Qi = 0; //накопеленная вероятность
    unsigned short int Li = 0; //длина кодового слова
    char* codeword = nullptr; //кодовое слово
};
struct codeFano
{
    unsigned char ch = 0;
    float Pi = 0;
    unsigned short int Li = 0;
    char* codeword = nullptr; 
};

void bubbleSort(chanceSymbol*& A, int numSymbols, unsigned short int field, bool reverse) {
    for (int i = 0; i < numSymbols - 1; i++) {
        for (int j = 0; j < numSymbols - i - 1; j++) {
            float x, y;
            if (field == 0) {
                x = A[j].ch;
                y = A[j + 1].ch;
            }
            else if (field == 1) {
                x = A[j].chance;
                y = A[j + 1].chance;
            }

            if ((reverse && x < y) || (!reverse && x > y)) {
                // Swap A[j] and A[j + 1]
                chanceSymbol temp = A[j];
                A[j] = A[j + 1];
                A[j + 1] = temp;
            }
        }
    }
}

codeShannon* ShannonCode(chanceSymbol* chanceSymbols, short int numSymbols) {
    codeShannon* shannon = new codeShannon[numSymbols];

    bubbleSort(chanceSymbols, numSymbols, 1, true);

    shannon[0].ch = chanceSymbols[0].ch; //Инициализация данных для первого символа
    shannon[0].Pi = chanceSymbols[0].chance; //Присваивается вероятность первого символа в массиве вероятностей
    shannon[0].Li = ceil(-log2(shannon[0].Pi)); //Вычисляется длина кодового слова для первого символа на основе вероятности
    shannon[0].Qi = 0; //Накопленная вероятность для первого символа устанавливается в 0.

    //Цикл для остальных символов, начиная с первого
    for (int i = 1; i < numSymbols; i++) {
        shannon[i].ch = chanceSymbols[i].ch; //Копируется символ из массива вероятностей
        shannon[i].Pi = chanceSymbols[i].chance; //Присваивается вероятность символа из массива вероятностей
        shannon[i].Li = ceil(-log2(shannon[i].Pi)); //Вычисляется длина кодового слова для текущего символа
        shannon[i].Qi = shannon[i - 1].Qi + shannon[i - 1].Pi; //Накопленная вероятность для текущего символа вычисляется как сумма накопленной вероятности предыдущего символа и его вероятности
    }

    //Цикл для каждого символа
    for (int i = 0; i < numSymbols; i++) {
        float temp = shannon[i].Qi; //Инициализация временной переменной накопленной вероятности текущего символа
        shannon[i].codeword = new char[shannon[i].Li]; //Выделяется память для массива символов, представляющего кодовое слово текущего символа
        for (int j = 0; j < shannon[i].Li; j++) { //Вложенный цикл для каждого бита кодового слова
            temp = temp * 2; //Удваивается значение временной переменной
            shannon[i].codeword[j] = (char)(floor(temp) + 48); //Присваивается значение текущего бита кодового слова
            if (temp >= 1) {
                temp = temp - 1; //Если значение временной переменной больше или равно 1, то из него вычитается 1
            }
        }
    }

    return shannon;
}

// Находит медиану между границами borderL и borderR в массиве Fano-кодов
int med(codeFano* fano, int borderL, int borderR) {

    float SumL = 0;
    // Вычисление суммы вероятностей слева от медианы
    for (int i = borderL; i < borderR; i++) {
        SumL = SumL + fano[i].Pi;
    }
    float SumR = fano[borderR].Pi;
    int m = borderR;
    // Поиск точки, где сумма слева становится меньше суммы справа
    while (SumL >= SumR) {
        m = m - 1;
        SumL = SumL - fano[m].Pi;
        SumR = SumR + fano[m].Pi;
    }
    return m;
}


void FanoCode(codeFano*& fano, int borderL, int borderR, int k) {
    // k - текущая глубина рекурсии (количество бит в коде)

    if (borderL < borderR) {
        k = k + 1;
        // Находим медиану между границами
        int m = med(fano, borderL, borderR);
        // Изменяем кодовые слова для символов в левой части (меньше медианы)
        for (int i = borderL; i <= borderR; i++) {
            // Если кодовое слово уже существует, увеличиваем его длину
            if (fano[i].codeword != nullptr) {
                char* temp = new char[k];
                for (int j = 0; j < k - 1; j++)
                    temp[j] = fano[i].codeword[j];
                delete[] fano[i].codeword;
                fano[i].codeword = temp;
            }
            else
                fano[i].codeword = new char[k];

            // Присваиваем биты кодовых слов в зависимости от положения символа относительно медианы
            if (i <= m) {
                fano[i].codeword[k - 1] = '0';
                fano[i].Li = fano[i].Li + 1;
            }
            else {
                fano[i].codeword[k - 1] = '1';
                fano[i].Li = fano[i].Li + 1;
            }
        }
        // Рекурсивный вызов для левой и правой частей массива
        FanoCode(fano, borderL, m, k);
        FanoCode(fano, m + 1, borderR, k);
    }
    // Если границы равны, заканчиваем рекурсию
    else {
    }

}

// Выполняет алгоритм кодирования Фано для заданного диапазона
codeFano* FanoCode(chanceSymbol* chanceSymbols, short int numSymbols) {
    // Выделение памяти под массив Fano-кодов
    codeFano* fano = new codeFano[numSymbols];

    // Сортировка символов по вероятности
    bubbleSort(chanceSymbols, numSymbols - 1, 1, 1);

    // Инициализация массива Fano-кодов символами и их вероятностями
    for (int i = 0; i < numSymbols; i++) {
        fano[i].ch = chanceSymbols[i].ch;
        fano[i].Pi = chanceSymbols[i].chance;
    }

    // Запуск рекурсивного алгоритма кодирования Фано
    FanoCode(fano, 0, numSymbols - 1, 0);

    return fano;
}

void printChanceSymbols(chanceSymbol* A, unsigned int num, unsigned short int encoding[256]) {
    float tmp = 0;
    for (int i = 0; i < num; i++) {
        cout <<setw(4) << A[i].ch << "(" <<setw(3) << encoding[(int)A[i].ch] << ")" << " | " <<fixed << A[i].chance << "\n";
        tmp += A[i].chance;
    }
   cout << "tmp: " << tmp << "\n";
}

float calculationEntropy(chanceSymbol* A, unsigned int nums) {
    float answer = 0;
    for (int i = 0; i < nums; i++) {
        answer += (A[i].chance * log2(A[i].chance));
    }
    return -answer;
}

float calculationAverageLength(codeShannon* A, unsigned int nums) {
    float answer = 0;
    for (int i = 0; i < nums; i++) {
        answer += A[i].Li * A[i].Pi;
    }
    return answer;
}

float calculationAverageLength(codeFano* A, unsigned int nums) {
    float answer = 0;
    for (int i = 0; i < nums; i++) {
        answer += A[i].Li * A[i].Pi;
    }
    return answer;
}

int main() {
    setlocale(LC_ALL, "Russian");
    string	filename = "test.txt";
    //Инициализация переменных для подсчета символов, уникальных символов и массива для хранения количества каждого символа
    unsigned int numSymbols = 0;
    unsigned int numUniqueSymbols = 0;
    unsigned short int window1251[256] = { 0 };

    ifstream file;
    unsigned char ch;

    file.open(filename);
    if (!file) {
        return 1;
    }


    // Считывание символов из файла и подсчет их количества
    while((ch = file.get()) && !file.eof()) {
        numSymbols++;
        window1251[(int)ch]++;
    }

    // Подсчет количества уникальных символов
    for (int i = 0; i < 256; i++) {
        if (window1251[i] != 0)
            numUniqueSymbols++;
    }

    cout << "numSymbols: " << numSymbols << " | " << "numUniqueSymbols: " << numUniqueSymbols << "\n";

    // Создание массива структур chanceSymbol
    chanceSymbol* chanceSymbols = new chanceSymbol[numUniqueSymbols];


    unsigned short int temp = 0;
    // Заполнение массива структур chanceSymbol
    for (int i = 0; i < 256; i++) {
        if (window1251[i] != 0) {
            chanceSymbols[temp].ch = (char)i;
            chanceSymbols[temp].chance = (float)window1251[i] / numSymbols;
            temp++;
        }
    }

    // Вывод информации о вероятностях символов
    printChanceSymbols(chanceSymbols, numUniqueSymbols, window1251);

    // Вычисление энтропии
    float entropy = calculationEntropy(chanceSymbols, numUniqueSymbols);

    // Создание кодов Шеннона
    codeShannon* shannon = nullptr;
    shannon = ShannonCode(chanceSymbols, numUniqueSymbols);

    // Вычисление средней длины кодов Шеннона
    float averageLshannon = calculationAverageLength(shannon, numUniqueSymbols);

    // Вывод информации о кодах Шеннона, энтропии и средней длине кодов
    cout << "\n" << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    cout << "\nCode Shennon:" << "\n";
    for (int i = 0; i < numUniqueSymbols; i++) {
        if (shannon[i].ch == '\n')
            cout <<setw(4) << "\\n" << " | " <<fixed << shannon[i].Pi << " | " <<fixed << shannon[i].Qi << " | " <<fixed << shannon[i].Li << " | ";
        else
            cout <<setw(4) << shannon[i].ch << " | " <<fixed << shannon[i].Pi << " | " <<fixed << shannon[i].Qi << " | " <<fixed << shannon[i].Li << " | ";

        for (int j = 0; j < shannon[i].Li; j++)
            cout << shannon[i].codeword[j];
        cout << "\n";
    }
    cout << "\n" << "entropy: " << entropy << " | " << " averageLenghtShannon: " << averageLshannon << "\n";


    bubbleSort(chanceSymbols, numUniqueSymbols - 1, 0, 0);
    codeFano* fano = nullptr;
    fano = FanoCode(chanceSymbols, numUniqueSymbols);
    float averageLfano = calculationAverageLength(fano, numUniqueSymbols);

    // Вывод информации о кодах Фано, энтропии и средней длине кодов
    cout << "\n" << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    cout << "\nCode Fano:" << "\n";
    for (int i = 0; i < numUniqueSymbols; i++) {
        if (fano[i].ch == '\n')
            cout << setw(4) << "\\n" << " | " << fixed << fano[i].Pi << " | " << fixed << fano[i].Li << " | ";
        else
            cout << setw(4) << fano[i].ch << " | " << fixed << fano[i].Pi << " | " << fixed << fano[i].Li << " | ";

        for (int j = 0; j < fano[i].Li; j++)
            cout << fano[i].codeword[j];
        cout << "\n";
    }
    cout << "\n" << "entropy: " << entropy << " | " << " averageLenghtFano: " << averageLfano << "\n";




    // Закрытие файла
    file.close();
	return 0;
}