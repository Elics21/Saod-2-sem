#include <iostream>

using namespace std;

struct Tree
{
    int data;
    Tree* left = NULL;
    Tree* right = NULL;
};

Tree* ISD(int arr[], int L, int R) {
    if (L > R) { return 0; }

    else
    {
        int m = (L + R) / 2;
        Tree* p = new Tree;
        p->data = arr[m];
        p->left = ISD(arr, L, m - 1);
        p->right = ISD(arr, m + 1, R);
        return p;
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
float SPD(Tree* root, int L = 1) {
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

    int const sizeArr = 100;
    int arr[sizeArr];

    for (int i = 0; i < sizeArr; i++) {
        arr[i] = i;
    }
    int L = 0;
    int R = sizeArr - 1;

    root = ISD(arr, L, R);


    int sizeTree = size(root);
    cout << "size: " << sizeTree << endl;

    int TotalsumTree = sumTree(root);
    cout << "totalsum: " << TotalsumTree << endl;

    int MaxHeightTree = heightTree(root);
    cout << "height: " << MaxHeightTree << endl;

    float HeightMid = heightMid(root);
    cout << "heightMid: " << HeightMid << endl;

    cout << "\n\n";

    cout << "\nleftToRight:" << endl;
    leftToRight(root);


    return 0;
}