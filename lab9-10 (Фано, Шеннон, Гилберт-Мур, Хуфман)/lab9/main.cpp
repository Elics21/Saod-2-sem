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

struct codeGilbert
{
    unsigned char ch = 0;
    float Pi = 0;
    float Qi = 0;
    unsigned short int Li = 0;
    char* codeword = nullptr;
};

struct codeHuffman
{
    unsigned char ch = 0;
    float Pi = 0;
    unsigned short int Li = 0;
    char* codeword = nullptr;
};

void quickSortV2(chanceSymbol*& A, int R, int L, unsigned short int field, bool reverse);
codeShannon* ShannonCode(chanceSymbol* chanceSymbols, short int numSymbols);
int med(codeFano* fano, int borderL, int borderR);
void FanoCode(codeFano*& fano, int borderL, int borderR, int k);
codeFano* FanoCode(chanceSymbol* chanceSymbols, short int numSymbols);
void printChanceSymbols(chanceSymbol* A, unsigned int num, unsigned short int encoding[256]);
float calculationEntropy(chanceSymbol* A, unsigned int nums);

codeGilbert* GilbertMurCode(chanceSymbol* chanceSymbols, short int numSymbols) {
    codeGilbert* gilbertmur = new codeGilbert[numSymbols];

    quickSortV2(chanceSymbols, numSymbols - 1, 0, 0, 0);

    float pr = 0;
    for (int i = 0; i < numSymbols; i++) {
        gilbertmur[i].ch = chanceSymbols[i].ch;
        gilbertmur[i].Pi = chanceSymbols[i].chance;
        gilbertmur[i].Li = ceil(-log2(gilbertmur[i].Pi)) + 1;
        gilbertmur[i].Qi = pr + gilbertmur[i].Pi / 2;
        pr += gilbertmur[i].Pi;
    }

    for (int i = 0; i < numSymbols; i++) {
        float temp = gilbertmur[i].Qi;
        gilbertmur[i].codeword = new char[gilbertmur[i].Li];
        for (int j = 0; j < gilbertmur[i].Li; j++) {
            temp = temp * 2;
            gilbertmur[i].codeword[j] = (char)(floor(temp) + 48);
            if (temp >= 1) {
                temp = temp - 1;
            }
        }
    }
    return gilbertmur;
}


unsigned short int Up(float*& Pi, unsigned int n, float x) {
    int j = 0;
    for (int i = n - 2; i > 0; i--) {
        if (Pi[i - 1] < x) {
            Pi[i] = Pi[i - 1];
        }
        else {
            j = i;
            break;
        }
    }
    Pi[j] = x;
    return j;
}

void Down(codeHuffman*& huffman, int n, int j) {

    char* S = new char[huffman[j].Li + 1];
    for (int i = 0; i < huffman[j].Li; i++)
        S[i] = huffman[j].codeword[i];

    int L = huffman[j].Li;

    for (int i = j; i <= n - 2; i++) {
        delete[] huffman[i].codeword;
        huffman[i].codeword = new char[huffman[i + 1].Li];
        for (int t = 0; t < huffman[i + 1].Li; t++)
            huffman[i].codeword[t] = huffman[i + 1].codeword[t];
        huffman[i].Li = huffman[i + 1].Li;
    }

    delete[] huffman[n - 1].codeword;
    delete[] huffman[n].codeword;

    huffman[n - 1].Li = L + 1;
    huffman[n].Li = L + 1;
    huffman[n - 1].codeword = new char[huffman[n - 1].Li];
    huffman[n].codeword = new char[huffman[n].Li];

    for (int i = 0; i < L; i++)
        huffman[n - 1].codeword[i] = huffman[n].codeword[i] = S[i];

    huffman[n - 1].codeword[L] = '0';
    huffman[n].codeword[L] = '1';

}

void HuffmanCode(codeHuffman*& huffman, float*& Pi, int n) {
    if (n == 2) {
        huffman[0].codeword = new char[1];
        huffman[0].codeword[0] = '0';
        huffman[0].Li = 1;
        huffman[1].codeword = new char[1];
        huffman[1].codeword[0] = '1';
        huffman[1].Li = 1;
    }
    else {
        float q = Pi[n - 2] + Pi[n - 1];
        int j = Up(Pi, n, q);
        HuffmanCode(huffman, Pi, n - 1);
        Down(huffman, n - 1, j);
    }
}

codeHuffman* CreateHuffmanCode(chanceSymbol* chanceSymbols, short int numSymbols) {
    codeHuffman* huffman = new codeHuffman[numSymbols];
    float* Pi = new float[numSymbols];

    quickSortV2(chanceSymbols, numSymbols - 1, 0, 1, 1);

    for (int i = 0; i < numSymbols; i++) {
        huffman[i].ch = chanceSymbols[i].ch;
        huffman[i].Pi = Pi[i] = chanceSymbols[i].chance;
    }

    HuffmanCode(huffman, Pi, numSymbols);

    return huffman;
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

float calculationAverageLength(codeGilbert* A, unsigned int nums) {
    float answer = 0;
    for (int i = 0; i < nums; i++) {
        answer += A[i].Li * A[i].Pi;
    }
    return answer;
}

float calculationAverageLength(codeHuffman* A, unsigned int nums) {
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

    //cout << "numSymbols: " << numSymbols << " | " << "numUniqueSymbols: " << numUniqueSymbols << "\n";
    //printChanceSymbols(chanceSymbols, numUniqueSymbols, window1251);   // Вывод информации о вероятностях символов

    float entropy = calculationEntropy(chanceSymbols, numUniqueSymbols);  // Вычисление энтропии



    codeShannon* shannon = nullptr;
    shannon = ShannonCode(chanceSymbols, numUniqueSymbols);
    float averageLshannon = calculationAverageLength(shannon, numUniqueSymbols);

    quickSortV2(chanceSymbols, numUniqueSymbols - 1, 0, 0, 0);
    codeFano* fano = nullptr;
    fano = FanoCode(chanceSymbols, numUniqueSymbols);
    float averageLfano = calculationAverageLength(fano, numUniqueSymbols);

    codeGilbert* gilbertMur = nullptr;
    gilbertMur = GilbertMurCode(chanceSymbols, numUniqueSymbols);
    float averageLgilbertMur = calculationAverageLength(gilbertMur, numUniqueSymbols);


    codeHuffman* huffman = nullptr;
    huffman = CreateHuffmanCode(chanceSymbols, numUniqueSymbols);
    float averageLhuffman = calculationAverageLength(huffman, numUniqueSymbols);

    cout << "Entropy: " << entropy << endl << endl;
    cout << "Shannon: " << averageLshannon << endl;
    cout << "Fano: " << averageLfano << endl;
    cout << "GilbertMur: " << averageLgilbertMur << endl;
    cout << "Huffman: " << averageLhuffman << endl << endl;
    

    //////////////ShannonCode/////////////////

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


    //////////////FanoCode/////////////////

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

    //////////////GilbertMurCode/////////////////

    cout << "\n" << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    cout << "\nCode GilbertMur:" << "\n";
    for (int i = 0; i < numUniqueSymbols; i++) {
        if (gilbertMur[i].ch == '\n')
            cout << setw(4) << "\\n" << " | " << fixed << gilbertMur[i].Pi << " | " << fixed << gilbertMur[i].Li << " | ";
        else
            cout << setw(4) << gilbertMur[i].ch << " | " << fixed << gilbertMur[i].Pi << " | " << fixed << gilbertMur[i].Li << " | ";

        for (int j = 0; j < gilbertMur[i].Li; j++)
            cout << gilbertMur[i].codeword[j];
        cout << "\n";
    }
    cout << "\n" << "entropy: " << entropy << " | " << " averageLenghtGilbertMur: " << averageLgilbertMur << "\n";


    //////////////HuffmanCode/////////////////

    cout << "\n" << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    cout << "\nCode Huffman:" << "\n";
    for (int i = 0; i < numUniqueSymbols; i++) {
        if (huffman[i].ch == '\n')
            cout << setw(4) << "\\n" << " | " << fixed << huffman[i].Pi << " | " << fixed << huffman[i].Li << " | ";
        else
            cout << setw(4) << huffman[i].ch << " | " << fixed << huffman[i].Pi << " | " << fixed << huffman[i].Li << " | ";

        for (int j = 0; j < huffman[i].Li; j++)
            cout << huffman[i].codeword[j];
        cout << "\n";
    }
    cout << "\n" << "entropy: " << entropy << " | " << " averageLenghtHuffman: " << averageLhuffman << "\n";

    // Закрытие файла
    file.close();
	return 0;
}

void quickSortV2(chanceSymbol*& A, int R, int L, unsigned short int field, bool reverse)
{
    while (L < R) {
        float x;
        if (field == 0)
            x = A[L].ch;
        else if (field == 1)
            x = A[L].chance;

        int i = L;
        int j = R;
        while (i <= j) {
            if (field == 0) {
                if (reverse) {
                    while (A[i].ch > x)
                        i++;
                    while (A[j].ch < x)
                        j--;
                }
                else
                {
                    while (A[i].ch < x)
                        i++;
                    while (A[j].ch > x)
                        j--;
                }

            }
            else if (field == 1)
            {
                if (reverse) {
                    while (A[i].chance > x)
                        i++;
                    while (A[j].chance < x)
                        j--;
                }
                else
                {
                    while (A[i].chance < x)
                        i++;
                    while (A[j].chance > x)
                        j--;
                }

            }
            if (i <= j) {
                int temp;
                temp = A[i].ch;
                A[i].ch = A[j].ch;
                A[j].ch = temp;
                float tmp;
                tmp = A[i].chance;
                A[i].chance = A[j].chance;
                A[j].chance = tmp;
                i++;
                j--;
            }
        }
        if (j - L > R - i) {
            quickSortV2(A, R, i, field, reverse);
            R = j;
        }
        else {
            quickSortV2(A, j, L, field, reverse);
            L = i;
        }
    }
}
codeShannon* ShannonCode(chanceSymbol* chanceSymbols, short int numSymbols) {
    codeShannon* shannon = new codeShannon[numSymbols];

    quickSortV2(chanceSymbols, numSymbols - 1, 0, 1, 1);

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
    quickSortV2(chanceSymbols, numSymbols - 1, 0, 1, 1);

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
        cout << setw(4) << A[i].ch << "(" << setw(3) << encoding[(int)A[i].ch] << ")" << " | " << fixed << A[i].chance << "\n";
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