#include "someFunc.hpp"


// Обход дерева слева направо
void leftToRight(Tree* root) {
	if (root != nullptr) {
		leftToRight(root->left);
		cout << root->data << " | ";
		leftToRight(root->right);
	}
}

//Размер дерева 
int sizeTree(Tree* p) {
	if (p == nullptr)
		return 0;
	else
		return (1 + sizeTree(p->left) + sizeTree(p->right));
}

// Контрольная сумма дерева
int checkSumTree(Tree* p) {
	if (p == nullptr)
		return 0;
	else
		return (p->data + checkSumTree(p->left) + checkSumTree(p->right));
}

// Функция вычисления суммы длин путей от корня до каждой вершины на L-том уровне
float sumPathLengths(Tree* root, int L) {
	if (root == nullptr) {
		return 0;
	}

	return L + sumPathLengths(root->left, L + 1) + sumPathLengths(root->right, L + 1);
}

// Определение средней длины пути в дереве
float averagePathLength(Tree* root) {
	return (float)(sumPathLengths(root) / sizeTree(root));
}

int maxHeight(int a, int b) {
	if (a < b)
		return b;
	else
		return a;
}

/* Высота дерева */
int heightTree(Tree* p) {
	if (p == nullptr)
		return 0;
	else
		return (1 + maxHeight(heightTree(p->left), heightTree(p->right)));
}

/*Вес дерева*/
int weightTree(Tree* root) {
	int n;
	if (root == nullptr) {
		n = 0;
	}
	else {
		n = root->weight + weightTree(root->left) + weightTree(root->right);
	}
	return n;
}

/* Средневзвешенная высота дерева */
float weightedAverageHeightTree(Tree* p) {
	float h;
	h = (float)sumPathLengths(p) / weightTree(p);
	return h;

}