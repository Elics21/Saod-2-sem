#include <iostream>
#include <ctime>

using namespace std;

struct TreeNode {
	int data;
	TreeNode* left = nullptr;
	TreeNode* right = nullptr;
};

// Вставка элемента в дерево
void insertNode(TreeNode** root, int data) {
	TreeNode** current = root;
	while (*current != nullptr) {
		if (data < (*current)->data) {
			current = &((*current)->left);
		}
		else if (data > (*current)->data) {
			current = &((*current)->right);
		}
		else {
			return;
		}
	}
	*current = new TreeNode;
	(*current)->data = data;
}

// Вставка элемента в дерево (вторая версия)
void insertNodeRecursive(TreeNode*& root, int data) {
	if (root == nullptr) {
		root = new TreeNode;
		root->data = data;
	}
	else if (data < root->data) {
		insertNodeRecursive(root->left, data);
	}
	else if (data > root->data) {
		insertNodeRecursive(root->right, data);
	}
	else {
		return;
	}
}

// Обход дерева слева направо
void traverseInOrder(TreeNode* root) {
	if (root) {
		traverseInOrder(root->left);
		cout << root->data << " ";
		traverseInOrder(root->right);
	}
}

// Определение размера дерева
int treeSize(TreeNode* root) {
	if (root == nullptr)
		return 0;

	return treeSize(root->left) + 1 + treeSize(root->right);
}

// Определение суммы значений узлов дерева
int sumTreeNodes(TreeNode* root) {
	if (root == nullptr) {
		return 0;
	}

	return root->data + sumTreeNodes(root->left) + sumTreeNodes(root->right);
}

// Определение высоты дерева
int treeHeight(TreeNode* root) {
	if (root == nullptr) {
		return 0;
	}

	return 1 + max(treeHeight(root->left), treeHeight(root->right));
}

// Функция вычисления суммы длин путей от корня до каждой вершины на L-том уровне
float sumPathLengths(TreeNode* root, int L = 1) {
	if (root == nullptr) {
		return 0;
	}

	return L + sumPathLengths(root->left, L + 1) + sumPathLengths(root->right, L + 1);
}

// Определение средней длины пути в дереве
float averagePathLength(TreeNode* root) {
	return (float)(sumPathLengths(root) / treeSize(root));
}

// Создание массива чисел, где все числа стоят рандомно, но не повторяются
void createRandomArray(int arr[], int size) {
	for (int i = 0; i < size; i++) {
		arr[i] = i;
	}
	for (int i = 0; i < size; i++) {
		int randomId = rand() % size;
		int temp = arr[i];
		arr[i] = arr[randomId];
		arr[randomId] = temp;
	}
}

int main() {
	srand(time(0));
	int i = 0;
	TreeNode* root1 = nullptr;
	const int size1 = 100;
	TreeNode* root2 = nullptr;
	const int size2 = 100;
	int arrFirst[size1];
	int arrSecond[size2];
	createRandomArray(arrFirst, size1);
	createRandomArray(arrSecond, size2);

	///////////1/////////
	cout << "***Tree 1***" << endl;
	for (int val : arrFirst) {
		insertNodeRecursive(root1, val);
	}
	traverseInOrder(root1);
	cout << "\n------------";
	cout << "\nsize: " << treeSize(root1);
	cout << "\nheight: " << treeHeight(root1);
	cout << "\nsum: " << sumTreeNodes(root1);
	cout << "\naverage path length: " << averagePathLength(root1);

	///////////2/////////
	cout << "\n\n***Tree 2***" << endl;
	for (int val : arrFirst) {
		insertNode(&root2, val);
	}
	traverseInOrder(root2);
	cout << "\n------------";
	cout << "\nsize: " << treeSize(root2);
	cout << "\nheight: " << treeHeight(root2);
	cout << "\nsum: " << sumTreeNodes(root2);
	cout << "\naverage path length: " << averagePathLength(root2);
}
