#include <iostream>

using namespace std;

struct Tree
{
    int data;
    Tree* left = NULL;
    Tree* right = NULL;
};

// Вставка элемента в дерево
void insert(Tree*& root, int data) {
    if (root == NULL) {
        root = new Tree;
        root->data = data;
    }
    else if (data <= root->data) {
        insert(root->left, data);
    }
    else {
        insert(root->right, data);
    }
}

//слева направо
void leftToRight(Tree* root) {
    if (root != NULL) {
        leftToRight(root->left);
        cout << root->data << " ";
        leftToRight(root->right);
    }
}

// сверху вниз
void topDown(Tree* root) {
    if (root == NULL)
        return;

    cout << root->data << " ";
    topDown(root->left);
    topDown(root->right);
}

// снизу вверх
void bottomUp(Tree* root) {
    if (root == NULL)
        return;

    bottomUp(root->left);
    bottomUp(root->right);
    cout << root->data << " ";
}

// Определение размера дерева
int size(Tree* root) {
    if (root == nullptr)
        return 0;

    return size(root->left) + 1 + size(root->right);
}

// Определение контрольной суммы для дерева
int sumTree(Tree* root) {
    if (root == nullptr) {
        return 0;
    }

    return root->data + sumTree(root->left) + sumTree(root->right);
}

// Определение высоты дерева
int heightTree(Tree* root) {
    if (root == nullptr) {
        return 0;
    }

    return 1 + max(heightTree(root->left), heightTree(root->right));
}

// функция вычисления суммы длин путей от корня до каждой вершины на L-том уровне
float SPD(Tree* root, int L=1) {
    if (root == nullptr) {
        return 0;
    }

    return L + SPD(root->left, L + 1) + SPD(root->right, L + 1);
}

// Определение средней высоты дерева
float heightMid(Tree* root) {
    return (float)(SPD(root) / size(root));
}

int main() {
    Tree* root = NULL;

    insert(root, 10);
    insert(root, 7);
    insert(root, 13);
    insert(root, 8);
    insert(root, 5);
    insert(root, 6);

    int sizeTree = size(root);
    cout << "size: " << sizeTree << endl;

    int TotalsumTree = sumTree(root);
    cout << "totalsum: " << TotalsumTree << endl;

    int MaxHeightTree = heightTree(root);
    cout << "height: " << MaxHeightTree << endl;

    float HeightMid = heightMid(root);
    cout << "heightMid: " << HeightMid << endl;

    cout << "SDP: " << SPD(root) << endl;

    cout << "\n\n";

    cout << "topDown:" << endl;
    topDown(root);

    cout << "\nleftToRight:" << endl;
    leftToRight(root);

    cout << "\nbottomUp:" << endl;
    bottomUp(root);

    return 0;
}