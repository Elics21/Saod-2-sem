#include <iostream>
#include <ctime>


using namespace std;

struct Tree {
	int data;
	Tree* left = NULL;
	Tree* right = NULL;
};

int sizeTree(Tree* root); // Определение размера дерева
int sumTree(Tree* root); // Определение контрольной суммы для дерева
int heightTree(Tree* root); // Определение высоты дерева
float SPD(Tree* root, int L = 1); // функция вычисления суммы длин путей от корня до каждой вершины на L-том уровне
float heightMid(Tree* root); // Определение средней высоты дерева
void SDP_Insert(Tree** root, int data); // Вставка элемента в дерево, двойная косвенность
void insert(Tree*& root, int data); // Вставка элемента в дерево, рекурсивно
void leftToRight(Tree* root); //слева направо
void createRandomArr(int arr[], int size); ////Создание массива чисел, где все числа стоят рандомно, но не повторяются


// Удаление узла из дерева
void deleteNodeFromTree(Tree*& root, int key){
	Tree* parent = nullptr; //родитель
	Tree* curr = root; 	// начинаем с корня

	// ищем нужный элемент
	while (curr != nullptr && curr->data != key)
	{
		// обновить родителя до текущего узла
		parent = curr;

		if (key < curr->data) {
			curr = curr->left;
		}
		else {
			curr = curr->right;
		}
		// возвращаем, если ключ не найден в дереве
		if (curr == nullptr) {
			return;
		}
	}

	// 1. не имеет потомков
	if (curr->left == nullptr && curr->right == nullptr)
	{
		// если не корень, то удаляем такой элемент
		if (curr != root)
		{
			if (parent->left == curr) {
				parent->left = nullptr;
			}
			else {
				parent->right = nullptr;
			}
		}
		// если это корень, то удаляем корень
		else {
			root = nullptr;
		}
		// освобождаем память
		delete curr;
	}

	// 2. имеет двух потомков
	else if (curr->left && curr->right)
	{
		// идем сначала в 
		Tree* MinKey = curr->right; //сначала идем в краво
		Tree* tParent = curr;

		//в правом поддереве ищем самый маленький элемент
		while (MinKey->left != nullptr) {
			tParent = MinKey;
			MinKey = MinKey->left;
		}

		if (tParent->right == MinKey) {
			if (curr != root) {
				if (parent->left == curr) {
					parent->left = MinKey;
				}
				else {
					parent->right = MinKey;
				}
			}
		}
		else if(tParent->right != MinKey) {
			if (MinKey->right == nullptr) {
				tParent->left = nullptr;
				MinKey->right = curr->right;
			}
			else {
				tParent->left = MinKey->right;
				MinKey->right = curr->right;
			}

			if (curr != root) {
				if (parent->left == curr) {
					parent->left = MinKey;
				}
				else {
					parent->right = MinKey;
				}
			}
		}

		MinKey->left = curr->left;


		if (curr == root) {
			root = MinKey;
		}


		delete curr;
	}

	// 3. удаляемый узел имеет только одного потомка
	else {
		// выбираем дочерний узел
		Tree* child = (curr->left) ? curr->left : curr->right;

		//если не корень, то устанавливаем вместо curr  - child
		if (curr != root)
		{
			curr == parent->left ? parent->left = child : parent->right = child;
		}
		else {
			root = child;
		}

		// освобождаем память
		delete curr;
	}
}

int main() {
	srand(time(0));
	int i = 0; 
	Tree* root = NULL; 
	int const sizeArr = 8;
	int arrFirst[sizeArr] = {0, 2, 5, 1, 7, 3, 4, 6};
	//createRandomArr(arrFirst, sizeArr);

	///////////1/////////
	cout << "***SDP***" << endl;
	for (int i : arrFirst) {
		SDP_Insert(&root, i);
	}
	leftToRight(root);

	for (int i : arrFirst) {
		deleteNodeFromTree(root, i);
		cout << "\n\n";
		leftToRight(root);
	}
}

void SDP_Insert(Tree** root, int data) {
	Tree** p = root;
	while (*p != NULL) {
		if (data < (*p)->data) {
			p = &((*p)->left);
		}
		else if (data > (*p)->data) {
			p = &((*p)->right);
		}
		else {
			return;
		}
	}
	*p = new Tree;
	(*p)->data = data;
}
void insert(Tree*& root, int data) {
	if (root == NULL) {
		root = new Tree;
		root->data = data;
	}
	else if (data < root->data) {
		insert(root->left, data);
	}
	else if (data > root->data) {
		insert(root->right, data);
	}
	else
	{
		return;
	}
}
void leftToRight(Tree* root) {
	if (root) {
		leftToRight(root->left);
		cout << root->data << " ";
		leftToRight(root->right);
	}
}
int sizeTree(Tree* root) {
	if (root == nullptr)
		return 0;

	return sizeTree(root->left) + 1 + sizeTree(root->right);
}
int sumTree(Tree* root) {
	if (root == nullptr) {
		return 0;
	}

	return root->data + sumTree(root->left) + sumTree(root->right);
}
int heightTree(Tree* root) {
	if (root == nullptr) {
		return 0;
	}

	return 1 + max(heightTree(root->left), heightTree(root->right));
}
float SPD(Tree* root, int L) {
	if (root == nullptr) {
		return 0;
	}

	return L + SPD(root->left, L + 1) + SPD(root->right, L + 1);
}
float heightMid(Tree* root) {
	return (float)(SPD(root) / sizeTree(root));
}
void createRandomArr(int arr[], int size) {
	for (int i = 0; i < size; i++) {
		arr[i] = i;
	}
	for (int i = 0; i < size; i++) {
		int randomId = rand() % size;
		int temp = arr[i];
		arr[i] = arr[randomId];
		arr[randomId] = temp;
	}

	//Вывод массива на экран
	cout << "Kak podautsa dannie v derevo" << endl;
	for (int i = 0; i < size; i++) {
		cout << arr[i] << " ";
	}
	cout << endl << endl;
}
