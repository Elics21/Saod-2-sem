#include <iostream>

using namespace std;
bool Rost = false;
bool Umen = false;

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

void LL1_turn(Tree*& p) {
    Tree* q = p->right;
    p->right = q->left;
    q->left = p;
    if (q->bal == 0) {
        q->bal = 1;
        p->bal = -1;
        Umen = false;
    }
    else {
        q->bal = 0;
        p->bal = 0;
    }

    p = q;
}

void RR1_turn(Tree*& p) {
    Tree* q = p->left;
    p->left = q->right;
    q->right = p;

    if (q->bal == 0) {
        q->bal = 1;
        p->bal = -1;
        Umen = false;
    }
    else {
        q->bal = 0;
        p->bal = 0;
    }

    p = q;
}

void BL(Tree*& p) {
    if (p->bal == -1) {
        p->bal = 0;
    }
    else if (p->bal == 0) {
        p->bal = 1;
        Umen = false;
    }
    else if (p->bal == 1) {
        if (p->right->bal >= 0) {
            RR1_turn(p);
        }
        else {
            RL_turn(p);
        }
    }
}

void BR(Tree*& p) {
    if (p->bal == 1) {
        p->bal = 0;
    }
    else if (p->bal == 0) {
        p->bal = -1;
        Umen = false;
    }
    else if (p->bal == -1) {
        if (p->left->bal <= 0) {
            LL1_turn(p);
        }
        else {
            LR_turn(p);
        }
    }
}

void del(Tree*& r) {
    Tree* q;
    if (r->right != nullptr) {
        del(r->right);
        if (Umen) {
            BR(r);
        }
    }
    else {
        q = r;
        r = r->left;
        Umen = true;
        delete q;
    }
}

void deleteElementToAVLTree(Tree*& p, int x) {
    Tree* q = p;
    if (p == nullptr) {
        return;
    }
    else if (x < p->data) {
        deleteElementToAVLTree(p->left, x);
        if (Umen) {
            BL(p);
        }
    }
    else if (x > p->data) {
        deleteElementToAVLTree(p->right, x);
        if (Umen) {
            BR(p);
        }
    }
    else {
        if (q->left == nullptr) {
            p = q->right;
            Umen = true;
            delete q;
        }
        else if (q->right == nullptr) {
            p = q->left;
            Umen = true;
            delete q;
        }
        else {
            del(q->left);
            if (Umen) {
                BL(p);
            }
        }
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
    //srand(time(0));
    Tree* head = nullptr;
    int arr[9];
    createRandomArr(arr, 9);

    for (int i : arr) {
        addElementToTree(head, i);
    }
    cout << "\nAVL-Tree: \n";
    leftToRight(head);

   for (int i : arr) {
        deleteElementToAVLTree(head, i);
        cout << "\n\n";
        if (head) {
            leftToRight(head);
        }
    }

    return 0;
}