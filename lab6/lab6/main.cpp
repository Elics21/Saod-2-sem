#include <iostream>

using namespace std;

struct Tree{
	int data;
	bool balance = false;
	Tree* left = nullptr;
	Tree* right = nullptr;

};

bool DBD_insert(Tree*& p, int data, bool& VR, bool& HR) {
    if (p == nullptr) {
        p = new Tree;
        p->data = data;
        p->left = p->right = nullptr;
        p->balance = false;
        VR = true;
    }
    else if (p->data > data) {
        if (DBD_insert(p->left, data, VR, HR)) {
            if (VR) {
                if (!p->balance) {
                    Tree* q = p->left;
                    p->left = q->right;
                    q->right = p;
                    p = q;
                    q->balance = true;
                    VR = false;
                    HR = true;
                }
                else {
                    p->balance = false;
                    VR = true;
                    HR = false;
                }
            }
            else
                HR = false;
        }
        else
            return false;
    }
    else if (p->data < data)
    {
        if (DBD_insert(p->right, data, VR, HR)) {
            if (VR) {
                p->balance = true;
                HR = true;
                VR = false;
            }
            else if (HR)
            {
                if (p->balance) {
                    Tree* q = p->right;
                    p->balance = false;
                    q->balance = false;
                    p->right = q->left;
                    q->left = p;
                    p = q;
                    VR = true;
                    HR = false;
                }
                else
                    HR = false;
            }
        }
        else
            return false;
    }
    else
        return false;
    return true;
}

/* Вывод слева направо */
void leftToRight(Tree* p, bool root) {
    if (root)
        cout << endl << "> : ";
    if (p != nullptr) {
        leftToRight(p->left, 0);
        cout << p->data << " ";
        leftToRight(p->right, 0);
    }
}

int treeHeight(Tree* root) {
    if (root == nullptr) {
        return 0;
    }

    return 1 + max(treeHeight(root->left), treeHeight(root->right));
}

int treeSize(Tree* root) {
    if (root == nullptr)
        return 0;

    return treeSize(root->left) + 1 + treeSize(root->right);
}

int sumTreeNodes(Tree* root) {
    if (root == nullptr) {
        return 0;
    }

    return root->data + sumTreeNodes(root->left) + sumTreeNodes(root->right);
}

float sumPathLengths(Tree* root, int L = 1) {
    if (root == nullptr) {
        return 0;
    }

    return L + sumPathLengths(root->left, L + 1) + sumPathLengths(root->right, L + 1);
}

float averagePathLength(Tree* root) {
    return (float)(sumPathLengths(root) / treeSize(root));
}

void createRandomArr(int arr[], int size, bool display) {
    for (int i = 0; i < size; i++) {
        arr[i] = i;
    }
    for (int i = 0; i < size; i++) {
        int randomId = rand() % size;
        int temp = arr[i];
        arr[i] = arr[randomId];
        arr[randomId] = temp;
    }
    if (display) {
        cout << "Kak podautsa dannie v derevo" << endl;
        for (int i = 0; i < size; i++) {
            cout << arr[i] << " ";
        }
        cout << endl << endl;
    }
    
}

int main() {
    srand(time(0));
    Tree* root = NULL;
    bool VR = 0;
    bool HR = 0;

    int const size = 100;
    int arr[size];
    createRandomArr(arr, size, 0);

    for (int i : arr) {
        DBD_insert(root, i, VR, HR);
    }

    leftToRight(root, 1);
    cout << "\nsize: " << treeSize(root) << "\n";
    cout << "avg_height: " << averagePathLength(root) << "\n";
    cout << "height: " << treeHeight(root) << "\n";

}
