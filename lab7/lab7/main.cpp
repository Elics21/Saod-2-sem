#include <iostream>
#include <time.h>
#include "someFunc.hpp"

using namespace std;

//Вычисление AW - матрицы весов :
void calc_AW(int n, int** AW, int** W) {
	for (int i = 0; i < n; i++) {
		for (int j = (i + 1); j < n; j++) {
			AW[i][j] = AW[i][j - 1] + W[1][j];
		}
	}

}

//Вычисление матриц AP и AR :
void calc_AP_AR(int n, int** AP, int** AR, int** AW) {
	for (int i = 0; i < n - 1; i++) {
		int j = i + 1;
		AP[i][j] = AW[i][j];
		AR[i][j] = j;
	}
	for (int h = 2; h < n; h++) {
		for (int i = 0; i < n - h; i++) {
			int j = i + h;
			int m = AR[i][j-1];
			int min = AP[i][m-1] + AP[m][j];
			for (int k = m + 1; k <= AR[i + 1][j]; k++) {
				int x = AP[i][k-1] + AP[k][j];
				if (x < min) {
					m = k;
					min = x;
				}
			}
			AP[i][j] = min + AW[i][j];
			AR[i][j] = m;
		}
	}
}

//Добавить В СДП
void addSDP(Tree*& root, int data, int weight) {
	if (root == NULL) {
		root = new Tree;
		root->data = data;
		root->weight = weight;
	}
	else if (data < root->data)
		addSDP(root->left, data, weight);
	else if (data > root->data)
		addSDP(root->right, data, weight);
}

//Создание дерева 
void createTree(Tree*& root, int L, int R, int** AR, int** W) {
	if (L < R) {
		int k = AR[L][R];
		addSDP(root, W[0][k], W[1][k]);
		createTree(root, L, k - 1, AR, W);
		createTree(root, k, R, AR, W);
	}
}


int main() {
	srand(time(0));

	const int size = 100;
	int** W = new int* [size];
	for (int i = 0; i < size; i++) {
		W[i] = new int[size];
	}

	// Заполение вершин неповторяющимися числами 
	bool table[2 * size] = { 0 }; //для отслеживания использовались ли числа для создания вершин
	int x;
	for (int i = 1; i < size + 1; i++) {
		//ищем элемент который еще не использовали
		while (true) {
			x = rand() % (2 * size);
			if (!table[x]) {
				break;
			}
		}

		table[x] = true;
		W[0][i] = x;
	}
	// Сортировка вершин
	for (int i = 1; i < size + 1; i++) {
		for (int j = size - 1 + 1; j > i; j--) {
			if (W[0][j] < W[0][j - 1]) {
				swap(W[0][j], W[0][j - 1]);
			}
		}
	}
	// Случайные веса для вершин
	for (int i = 1; i < size + 1; i++) {
		W[1][i] = rand() % size + 1;
	}

	cout << "*************************** Nachalnie-Dannie ***************************\n";
	
	// Вывод начальных данных и весов
	int tmp = 0;
	for (int i = 1; i < size + 1; i++) {
		cout.width(5);
		cout << W[0][i]; // выводим вершину
		cout << "[";
		cout.width(3);
		cout << W[1][i];
		cout << "]" << " ";
		tmp++;
		if (tmp == 10) {
			cout << endl;
			tmp = 0;
		}
	}

	int** AW = new int* [size + 1]; // матрица весов
	int** AP = new int* [size + 1]; // матрица взвешенных высот
	int** AR = new int* [size + 1]; // матрица индексов

	//выделение памяти и заполнение нулями
	for (int i = 0; i < size + 1; i++) {
		AW[i] = new int[size + 1];
		AP[i] = new int[size + 1];
		AR[i] = new int[size + 1];
		for (int j = 0; j < size + 1; j++)
			AW[i][j] = AP[i][j] = AR[i][j] = 0;
	}

	calc_AW(size + 1, AW, W);
	calc_AP_AR(size + 1, AP, AR, AW);

	Tree* root = nullptr;

	createTree(root, 0, size, AR, W);
	cout << "\n*************************** DOP ***************************\n";
	cout << "> : ";
	leftToRight(root);

	cout << "\n\nSize: " << sizeTree(root);
	cout << "\nHeight: " << heightTree(root);
	cout << "\nWeight: " << weightTree(root);
	cout << "\nAvg Height: " << averagePathLength(root);
	cout << "\nAvg Weight: " << weightedAverageHeightTree(root);

}