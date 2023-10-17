#include <iostream>

using namespace std;

struct  Tree
{
	int data;
	int bal;
	Tree* left;
	Tree* right;
};

void LL_turn(Tree*& p) {
	Tree* q = p->left;
	p->bal = 0;
	q->bal = 0;
	p->left = q->right;
	q->right = p;
	p = q;
}

void RR_turn(Tree*& p) {
	Tree* q = p->right;
	p->bal = 0;
	q->bal = 0;
	p->right = q->left;
	q->left = p;
	p = q;
}

void LR_turn(Tree*& p) {
	Tree* q = p->left;
	Tree* r = q->right;
	if (r->bal < 0)
		p->bal = 1;
	else
		p->bal = 0;
	if (r->bal > 0)
		q->bal = -1;
	else
		q->bal = 0;

	r->bal = 0;
	q->right = r->left;
	p->left = r->right;
	r->left = q;
	r->right = p;
	p = r;
}

void RL_turn(Tree*& p) {
	Tree* q = p->right;
	Tree* r = q->left;
	if (r->bal < 0)
		p->bal = 1;
	else
		p->bal = 0;
	if (r->bal > 0)
		q->bal = -1;
	else
		q->bal = 0;

	r->bal = 0;
	q->left = r->right;
	p->right = r->left;
	r->right = q;
	r->left = p;
	p = r;
}

bool Rost=false;

void addElementToTree(Tree*& p, int data) {
    if (p == nullptr) {
        p = new Tree;
        p->data = data;
        p->left = nullptr;
        p->right = nullptr;
        p->bal = 0;
        Rost = true;
    }
    else if (p->data > data) {
        addElementToTree(p->left, data);
        if (Rost) {
            if (p->bal > 0) {
                p->bal = 0;
                Rost = false;
            }
            else if (p->bal == 0) {
                p->bal = -1;
                Rost = true;
            }
            else {
                if (p->left->bal < 0) {
                    LL_turn(p);
                    Rost = false;
                }
                else {
                    LR_turn(p);
                    Rost = false;
                }
            }
        }
    }
    else if (p->data < data) {
        addElementToTree(p->right, data);
        if (Rost) {
            if (p->bal < 0) {
                p->bal = 0;
                Rost = false;
            }
            else if (p->bal == 0) {
                p->bal = 1;
                Rost = true;
            }
            else {
                if (p->right->bal > 0) {
                    RR_turn(p);
                    Rost = false;
                }
                else {
                    RL_turn(p);
                    Rost = false;
                }
            }
        }
    }
    else {
		return;
    }
}

//слева направо
void leftToRight(Tree* root) {
	if (root) {
		leftToRight(root->left);
		cout << root->data << " ";
		leftToRight(root->right);
	}
}

// Определение размера дерева
int treeSize(Tree* root) {
	if (root == nullptr)
		return 0;

	return treeSize(root->left) + 1 + treeSize(root->right);
}

// Определение суммы значений узлов дерева
int sumTreeNodes(Tree* root) {
	if (root == nullptr) {
		return 0;
	}

	return root->data + sumTreeNodes(root->left) + sumTreeNodes(root->right);
}

// Определение высоты дерева
int treeHeight(Tree* root) {
	if (root == nullptr) {
		return 0;
	}

	return 1 + max(treeHeight(root->left), treeHeight(root->right));
}

// Функция вычисления суммы длин путей от корня до каждой вершины на L-том уровне
float sumPathLengths(Tree* root, int L = 1) {
	if (root == nullptr) {
		return 0;
	}

	return L + sumPathLengths(root->left, L + 1) + sumPathLengths(root->right, L + 1);
}

// Определение средней длины пути в дереве
float averagePathLength(Tree* root) {
	return (float)(sumPathLengths(root) / treeSize(root));
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

	cout << "Kak podautsa dannie v derevo" << endl;
	for (int i = 0; i < size; i++) {
		cout << arr[i] << " ";
	}
	cout << endl << endl;
}

int main() {
	srand(time(0));
	Tree* head = nullptr;
	int arr[100] = { 53, 20,  4,  9, 16,  5, 87, 74, 13, 26, 50, 11,
  25,  7, 48,  1, 19, 18, 95, 44, 66, 54, 22, 96,
  49, 64, 51, 93,  3, 59, 28, 32, 85, 78, 56, 27,
  83, 12, 65, 82, 37, 91, 80, 10, 70, 57, 99, 33,
  14, 76, 84, 41,  2, 81, 97, 86,  8, 36, 46, 34,
  72, 79, 17, 71, 89, 29, 23, 69, 38, 35, 73, 94,
  61, 88, 68, 21, 55, 42, 62, 90, 75, 15, 58, 63,
  47, 52, 40,  6, 24, 39, 92, 31, 60, 45,  0, 43,
  67, 98, 30, 77 };
	cout << "Kak podautsa dannie v derevo" << endl;
	for (int i = 0; i < 100; i++) {
		cout << arr[i] << " ";
	}
	cout << endl << endl;
	//createRandomArr(arr, 100);
	for (int i : arr) {
		addElementToTree(head, i);
	}
	leftToRight(head);
	cout << "\navg_size: " << averagePathLength(head);
}