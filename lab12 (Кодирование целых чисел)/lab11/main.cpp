#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

// Функция для преобразования числа в двоичное представление
string toBinary(uint8_t num) {
    string answer = "";
    // Идем по битам числа от старшего к младшему
    for (int i = sizeof(num) * 8; i > 0; i--) {
        // Проверяем установлен ли i-й бит
        if ((num & (1 << (i - 1))) != 0) {
            answer += '1';
        }
        else {
            answer += '0';
        }
    }
    return answer;
}

// Функция для кодирования числа в формате Fixed + Variable
string fixedVariableCode(uint8_t num) {
    short int size = 8 * sizeof(num);
    uint8_t order = 0;
    string answer = "";

    // Определение порядка
    for (int i = size; i > 0; i--) {
        if ((num & (1 << (i - 1))) != 0) {
            order = i;
            break;
        }
    }

    int temp = 0;
    // Вычисление размера порядка
    while (pow(2, temp) <= size)
        temp++;

    // Кодирование порядка
    for (int i = temp; i > 0; i--) {
        if ((order & (1 << (i - 1))) != 0) {
            answer += '1';
        }
        else {
            answer += '0';
        }
    }
    answer += ' ';

    // Кодирование мантиссы
    for (int i = (order - 1); i > 0; i--) {
        if ((num & (1 << (i - 1))) != 0) {
            answer += '1';
        }
        else {
            answer += '0';
        }
    }
    return answer;
}

// Функция для кодирования числа в формате Гамма-кода Элиаса
string gammaCodeElias(uint8_t num) {
    short int size = 8 * sizeof(num);
    uint8_t order = 0;
    string answer = "";

    // Определение порядка
    for (int i = size; i > 0; i--) {
        if ((num & (1 << (i - 1))) != 0) {
            order = i;
            break;
        }
    }

    // Кодирование порядка
    for (int i = (order - 1); i > 0; i--)
        answer += '0';

    answer += ' ';

    // Кодирование мантиссы
    for (int i = order; i > 0; i--) {
        if ((num & (1 << (i - 1))) != 0) {
            answer += '1';
        }
        else {
            answer += '0';
        }
    }
    return answer;
}

// Функция для кодирования числа в формате Омега-кода Элиаса
string omegaCodeElias(uint8_t num) {
    if (num == 0)
        return "";

    short int size = 8 * sizeof(num);
    uint8_t order = 0;
    string temp = "";
    string answer = "0";

    while (num != 1) {
        temp = "";

        // Определение порядка
        for (int i = size; i > 0; i--) {
            if ((num & (1 << (i - 1))) != 0) {
                order = i;
                break;
            }
        }

        // Кодирование мантиссы
        for (int i = order; i > 0; i--) {
            if ((num & (1 << (i - 1))) != 0) {
                temp += '1';
            }
            else {
                temp += '0';
            }
        }
        temp += " " + answer;
        answer = temp;
        num = order - 1;
    }

    return answer;
}

int main() {
    setlocale(LC_ALL, "Russian");

    uint8_t number = 0;

    cout << "| Десятичное число | Бинарный код | Fixed + Variable | Гамма-код Элиаса | Омега-код Элиаса |" << "\n";
    cout << "----------------------------------------------------------------------------------------------" << "\n";

    // Вывод данных для каждого числа от 0 до 255
    for (int i = number; i < 256; ++i) {
        cout << left << setw(19) << static_cast<int>(number);
        cout << "|";
        cout << left << setw(14) << toBinary(number);
        cout << "|";
        cout << left << setw(20) << fixedVariableCode(number);
        cout << "|";
        cout << left << setw(18) << gammaCodeElias(number);
        cout << "|";
        cout << left << setw(18) << omegaCodeElias(number);
        cout << "|\n";

        ++number;
    }

    return 0;
}
