#include <iostream>

using namespace std;
bool Rost = false;

struct Tree {
    int data;
    int bal;
    Tree* left;
    Tree* right;
};

// Определение высоты дерева
int treeHeight(Tree* root) {
    if (root == nullptr) {
        return 0;
    }

    return 1 + max(treeHeight(root->left), treeHeight(root->right));
}

void LL_turn(Tree*& p) {
    Tree* q = p->left;
    p->left = q->right;
    q->right = p;

    p->bal = 0;
    q->bal = 0;

    p = q;
}

void RR_turn(Tree*& p) {
    Tree* q = p->right;
    p->right = q->left;
    q->left = p;

    p->bal = 0;
    q->bal = 0;

    p = q;
}

void LR_turn(Tree*& p) {
    Tree* q = p->left;
    Tree* r = q->right;

    p->left = r->right;
    q->right = r->left;
    r->left = q;
    r->right = p;

    if (r->bal < 0) {
        p->bal = 1;
        q->bal = 0;
    }
    else if (r->bal > 0) {
        p->bal = 0;
        q->bal = -1;
    }
    else {
        p->bal = 0;
        q->bal = 0;
    }

    r->bal = 0;

    p = r;
}


void RL_turn(Tree*& p) {
    Tree* q = p->right;
    Tree* r = q->left;

    p->right = r->left;
    q->left = r->right;
    r->left = p;
    r->right = q;

    if (r->bal < 0) {
        p->bal = 0;
        q->bal = 1;
    }
    else if (r->bal > 0) {
        p->bal = -1;
        q->bal = 0;
    }
    else {
        p->bal = 0;
        q->bal = 0;
    }

    r->bal = 0;

    p = r;
}

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
                }
                else {
                    LR_turn(p);
                }
                Rost = false;
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
                }
                else {
                    RL_turn(p);
                }
                Rost = false;
            }
        }
    }
    else {
        return;
    }
}

void LL1(Tree*& p, bool& decrease) {
    Tree* q = p->left;

    if (q->bal == 0) {
        p->bal = -1;
        q->bal = 1;
        decrease = false;
    }
    else {
        p->bal = 0;
        q->bal = 0;
    }
    p->left = q->right;
    q->right = p;
    p = q;
}

void RR1(Tree*& p, bool& decrease) {
    Tree* q = p->right;
    if (q->bal == 0) {
        p->bal = 1;
        q->bal = -1;
        decrease = false;
    }
    else {
        p->bal = 0;
        q->bal = 0;
    }
    p->right = q->left;
    q->left = p;
    p = q;
}

void LR1(Tree*& p) {
    Tree* q = p->left;
    Tree* r = q->right;
    if (r->bal < 0)
        p->bal = 1;
    else
        p->bal= 0;

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

void RL1(Tree*& p) {
    Tree* q = p->right;
    Tree* r = q->left;
    if (r->bal > 0)
        p->bal = -1;
    else
        p->bal = 0;

    if (r->bal < 0)
        q->bal = 1;
    else
        q->bal = 0;

    r->bal = 0;
    q->left = r->right;
    p->right = r->left;
    r->right = q;
    r->left = p;
    p = r;
}


void BL(Tree*& p, bool& decrease){
    if (p->bal == -1)
        p->bal = 0;
    else if (p->bal == 0) {
        p->bal = 1;
        decrease = false;
    }
    else if (p->bal == 1) {
        if (p->right->bal >= 0)
            RR1(p, decrease);
        else
            RL1(p);
    }
}

void BR(Tree*& p, bool& decrease) {
    if (p->bal == 1)
        p->bal = 0;
    else if (p->bal == 0) {
        p->bal = -1;
        decrease = false;
    }
    else if (p->bal == -1) {
        if (p->left->bal <= 0)
            LL1(p, decrease);
        else
            LR1(p);
    }
}

void del(Tree*& r, Tree*& q, bool& decrease) {
    if (r->right != NULL) {
        del(r->right, q, decrease);
        if (decrease)
            BR(r, decrease);
    }
    else {
        q->data = r->data;
        q = r;
        r = r->left;
        decrease = true;
    }
}

bool deleteElementToAVLTree(Tree*& p, int x, bool& decrease) {
    Tree* q = NULL;
    if (p == NULL)
        return 1;
    else if (p->data > x) {
        if (deleteElementToAVLTree(p->left, x, decrease))
            return 1;
        if (decrease)
            BL(p, decrease);
    }
    else if (p->data < x) {
        if (deleteElementToAVLTree(p->right, x, decrease))
            return 1;
        if (decrease)
            BR(p, decrease);
    }
    else {
        q = p;
        if (q->right == NULL) {
            p = q->left;
            decrease = true;
        }
        else if (q->left == NULL) {
            p = q->right;
            decrease = true;
        }
        else {
            del(q->left, q, decrease);
            if (decrease)
                BL(p, decrease);
        }
    }
    delete (q);
    return 0;
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
    bool decrease = 0;
    int arr[10];
    createRandomArr(arr, 10);

    for (int i : arr) {
        addElementToTree(head, i);
    }
    cout << "\nAVL-Tree: \n";
    leftToRight(head);

   for (int i : arr) {
       if (!deleteElementToAVLTree(head, i, decrease)) {
           cout << "\n\n";
           leftToRight(head);
       }
       else {
           cout << "ERROR! key = " << i;
           break;
       }
    }

    return 0;
}