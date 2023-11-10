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

void A1(Tree*& root, int size, int** W){
	 //Сортировка по весам
	 for (int i = 1; i < size; i++) {
	 	for (int j = size - 1; j > i; j--) {
	 		if (W[1][j] > W[1][j - 1]) {
	 			swap(W[0][j], W[0][j - 1]);
	 			swap(W[1][j], W[1][j - 1]);
	 		}
	 	}
	 }

	for (int i = 1; i < size; i++)
	{
		addSDP(root, W[0][i], W[1][i]);
	}
}

void A2(Tree*& root, int L, int R, int** W){
	int wes = 0, sum = 0;
	if (L <= R)
	{
		int i = 0;
		for (i = L; i < R; i++)
			wes = wes + W[1][i];


		for (i = L; i < R; i++)
		{
			if ((sum <= (wes / 2)) && (sum + W[1][i] > (wes / 2)))
				break;
			sum = sum + W[1][i];
		}

		addSDP(root, W[0][i], W[1][i]);
		A2(root, L, i - 1, W);
		A2(root, i + 1, R, W);
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

	cout << "*************************** Nachalnie-Dannie ***************************\n\n";
	
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

	Tree* root_DOP = nullptr;
	Tree* root_A1 = nullptr;
	Tree* root_A2 = nullptr;

	createTree(root_DOP, 0, size, AR, W);
	A2(root_A1, 1, size, W);
	A1(root_A2, size + 1, W);
	
	cout.width(50);
	cout << "\n\n*************************** DOP ***************************\n";
	cout << "> : ";
	leftToRight(root_DOP);
	cout << "\n\nSize: " << sizeTree(root_DOP);
	cout << "\nHeight: " << heightTree(root_DOP);
	cout << "\nWeight: " << weightTree(root_DOP);
	cout << "\nAvg Height: " << averagePathLength(root_DOP);
	cout << "\nAvg Weight: " << weightedAverageHeightTree(root_DOP);

	cout.width(50);
	cout << "\n\n*************************** A1 ***************************\n";
	cout << "> : ";
	leftToRight(root_A1);
	cout << "\n\nSize: " << sizeTree(root_A1);
	cout << "\nHeight: " << heightTree(root_A1);
	cout << "\nWeight: " << weightTree(root_A1);
	cout << "\nAvg Height: " << averagePathLength(root_A1);
	cout << "\nAvg Weight: " << weightedAverageHeightTree(root_A1);

	cout.width(50);
	cout << "\n\n*************************** A2 ***************************\n";
	cout << "> : "; 
	leftToRight(root_A2);
	cout << "\n\nSize: " << sizeTree(root_A2);
	cout << "\nHeight: " << heightTree(root_A2);
	cout << "\nWeight: " << weightTree(root_A2);
	cout << "\nAvg Height: " << averagePathLength(root_A2);
	cout << "\nAvg Weight: " << weightedAverageHeightTree(root_A2);

	return 0;
}