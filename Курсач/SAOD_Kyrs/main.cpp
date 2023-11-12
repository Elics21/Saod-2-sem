#include <iostream>
#include <cstdio>
#include <queue>
#include <stdio.h>
#include <cmath>
#define N 4000

using namespace std;

struct spis {
	char FIO[30];
	short int deprt;
	char post[22];
	char birthdate[10];
	spis* next = nullptr;
	spis* prev;
};

struct Que {
	int data;
	int sizeQue;
	spis* spisElement;
	Que* next;
};

struct IndexArray{
	spis* indexSpisElement;
	int data;
};

struct Tree
{
	int data;
	int weight;

	spis* spisHead;

	Tree* left = nullptr;
	Tree* right = nullptr;
};

struct Weight
{
	int vertex;
	int weight;
	
	spis* head;
};



////////////////////////˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜/////////////////////////////////////////////
spis* Split(spis* head, int step);
spis* Merge(spis* left, spis* right, spis* tail);
spis* MergeSort(spis* head);
void OutputDatabase(spis* head);
void OutputDatabaseByPages(spis* head);
void CreateBase(FILE* fp, spis*& head, spis*& tail);
void EndlessMenu(spis*& head, spis*& tail);
void BinSearch(spis* head, int key, Que*& First, Que*& Last);
//////////////////////////////////////////////////////////////////////////////////////

void printElementsFromQueue(spis*& head, Que*& first) {
	int sizeQue = first->sizeQue;
	Que* elementQue = new Que;
	elementQue = first;
	spis* spisElement;
	int i = 0;

	while (sizeQue--) {
		spisElement = elementQue->spisElement;
		cout << spisElement->FIO << "\t"
			<< spisElement->deprt << "\t" << spisElement->post << "\t"
			<< spisElement->birthdate << endl;
		elementQue = elementQue->next;
	}
}

//˜˜˜˜˜˜˜˜ ˜ ˜˜˜
void addSDP(Tree*& root, int data, int weight, Weight** weightArr) {
	if (root == NULL) {
		root = new Tree;
		root->data = data;
		root->weight = weight;

		int j = 0;
		for (j = 0; j < N; j++) {
			if (weightArr[j]->vertex == data) {
				break;
			}
		}
		root->spisHead = weightArr[j]->head;
	}
	else if (data < root->data)
		addSDP(root->left, data, weight, weightArr);
	else if (data > root->data)
		addSDP(root->right, data, weight, weightArr);
}

void A2(Tree*& root, int L, int R, int** W, Weight **weightArr) {
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

		addSDP(root, W[0][i], W[1][i], weightArr);
		A2(root, L, i - 1, W, weightArr);
		A2(root, i + 1, R, W, weightArr);
	}
}

void leftToRight(Tree* root) {
	if (root != nullptr) {
		leftToRight(root->left);
		cout.width(3);
		cout << root->data; // ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜
		cout.width(3);
		cout << "[";
		cout << root->weight;
		cout << "]" << "\n";
		leftToRight(root->right);
	}
}

int sizeTree(Tree* p) {
	if (p == nullptr)
		return 0;
	else
		return (1 + sizeTree(p->left) + sizeTree(p->right));
}

void searchInTree(Tree*& root, int key) {
	if (root == nullptr) {
		cout << "\nNone\n";
	}
	else if (root->data == key) {
		spis* currElemnt = root->spisHead;
		while (currElemnt->next) {
			cout << currElemnt->FIO << "\t" << currElemnt->deprt << "\t" << currElemnt->post << "\t" << currElemnt->birthdate << endl;
			currElemnt = currElemnt->next;
		}
	}
	else if (key < root->data)
		searchInTree(root->left, key);
	else if (key > root->data)
		searchInTree(root->right, key);
		
}


int main()
{
	FILE* fp = fopen("base.dat", "rb"); //˜˜˜˜˜˜˜˜˜ ˜˜˜ ˜˜˜˜˜˜

	//˜˜˜˜˜˜˜˜ ˜˜ ˜˜˜˜˜˜˜˜
	if (fp == NULL) { 
		cout << "Error with open base.dat" << endl;
		return -1;
	}

	spis* head = NULL; // ˜˜˜˜˜˜
	spis* tail = NULL; // ˜˜˜˜˜

	// ˜˜˜˜˜˜˜˜˜ ˜˜˜˜ ˜˜˜˜˜˜
	CreateBase(fp, head, tail);
	
	//˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜
	EndlessMenu(head, tail);

	return 0;

}
void EndlessMenu(spis*& head, spis*& tail) {
	while (true)
	{
		int menu;
		system("cls");
		cout << "----Menu----\n\n"
			<< "1 - Vse zapisi\n" //˜˜˜˜˜˜˜˜˜ ˜˜˜ ˜˜˜˜˜˜
			<< "2 - Po stranicam\n" //˜˜˜˜˜˜˜˜ ˜˜ ˜˜˜˜˜˜˜˜˜, ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜ ˜˜˜˜˜˜˜
			<< "3 - Sortizovanie zapisi\n" //˜˜˜˜˜˜˜˜˜˜
			<< "4 - Poisk po klychy\n" //˜˜˜˜˜˜˜˜˜˜
			<< "9 - Exit\n"; //˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜ ˜˜˜˜˜

		cin >> menu;

		//˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜ ˜˜˜˜˜
		if (menu == 9) {
			spis* p;
			p = head;
			while (p) {
				spis* pp = p->next;
				delete p;
				p = pp;
			}
			break;
		}
		//˜˜˜˜˜˜˜˜˜ ˜˜˜ ˜˜˜˜˜˜
		else if (menu == 1) {
			OutputDatabase(head);
		}
		//˜˜˜˜˜˜˜˜ ˜˜ ˜˜˜˜˜˜˜˜˜, ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜ ˜˜˜˜˜˜˜
		else if (menu == 2)
		{
			OutputDatabaseByPages(head);
		}
		else if (menu == 3)
		{
			head = MergeSort(head);
		}
		else if (menu == 4) {
			system("cls");
			int key;
			cout << "Vvedite key: ";
			cin >> key;

			//˜˜˜˜˜˜˜˜˜
			head = MergeSort(head);

			//˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜
			Que* First = nullptr; // ˜˜˜˜˜˜
			Que* Last = nullptr;

			BinSearch(head, key, First, Last);


			while (true)
			{
				system("cls");
				cout << "1. Vivesti Rezultati\n"
					<< "2. Postoit Derevo po deprt\n"
					<< "5. Back\n";
				int menu_input;
				cin >> menu_input;

				if (menu_input == 1) {
					system("cls");
					printElementsFromQueue(head, First);
					system("pause");
				}
				else if (menu_input == 2) {
					// ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜
					system("cls");

					// ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜˜
					int sizeQue = First->sizeQue;

					// ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜ ˜˜˜˜˜˜˜
					IndexArray* indexArray = new IndexArray[sizeQue];
					Que* curr = new Que;
					curr = First;
					int i = 0;

					int* indexArrUnqTemp = new int[sizeQue]; // ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜

					// ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜
					for (i = 0; i < sizeQue; i++) {
						indexArray[i].data = curr->spisElement->deprt;
						indexArray[i].indexSpisElement = curr->spisElement;
						indexArrUnqTemp[i] = curr->spisElement->deprt;
						curr = curr->next;
					}

					// ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜ ˜˜ ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜
					for (int i = 0; i < sizeQue; i++) {
						for (int j = i + 1; j < sizeQue; j++) {
							if (indexArrUnqTemp[i] == indexArrUnqTemp[j]) {
								indexArrUnqTemp[j] = -1;
							}
						}
					}

					// ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜
					int k = 0;
					for (int i = 0; i < sizeQue; i++) {
						if (indexArrUnqTemp[i] != -1) {
							k++;
						}
					}

					// ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜ ˜˜˜ ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜
					Weight** weightArr = new Weight * [k];
					for (int i = 0; i < k; i++) {
						weightArr[i] = new Weight[k];
					}

					// ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜ ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜ ˜˜ ˜˜˜˜˜˜˜˜˜˜
					int** indexArrUnq = new int* [k];
					for (int i = 0; i < 2; i++) {
						indexArrUnq[i] = new int[k];
					}

					// ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜
					for (int i = 1; i < k + 1; i++) {
						for (int j = 0; j < sizeQue; j++) {
							if (indexArrUnqTemp[j] != -1) {
								indexArrUnq[0][i] = indexArrUnqTemp[j];
								indexArrUnqTemp[j] = -1;
								break;
							}
						}
					}

					// ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜
					for (int i = 1; i < k + 1; i++) {
						for (int j = k; j > i; j--) {
							if (indexArrUnq[0][j] < indexArrUnq[0][j - 1]) {
								swap(indexArrUnq[0][j], indexArrUnq[0][j - 1]);
							}
						}
					}

					// ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜
					for (int i = 1; i < k + 1; i++) {
						int s = 0;
						for (int j = 0; j < sizeQue; j++) {
							if (indexArray[j].indexSpisElement->deprt == indexArrUnq[0][i]) {
								s++;
							}
						}
						indexArrUnq[1][i] = s;
					}

					// ˜˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜ ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜
					for (int i = 0; i < sizeQue; i++) {
						for (int j = 0; j < k; j++) {
							if (indexArray[i].indexSpisElement->deprt == indexArrUnq[0][j]) {
								indexArray[i].data = indexArrUnq[1][j];
								break;
							}
						}
					}

					// ˜˜˜˜˜˜˜ ˜˜˜˜ ˜ ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜
					for (int i = 0; i < k; i++) {
						weightArr[i]->vertex = indexArrUnq[0][i + 1];
						weightArr[i]->weight = indexArrUnq[1][i + 1];
						spis* headTemp = new spis;
						headTemp->next = nullptr;
						for (int j = 0; j < sizeQue; j++) {
							if (indexArray[j].indexSpisElement->deprt == weightArr[i]->vertex) {
								spis* p = new spis;
								p = indexArray[j].indexSpisElement;
								p->next = headTemp;
								headTemp = p;
								weightArr[i]->head = headTemp;
							}
						}
					}

					// ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜ ˜˜˜˜˜˜
					Tree* root = nullptr;

					// ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜ ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜
					A2(root, 1, k, indexArrUnq, weightArr);

					cout << " key";
					cout << " size\n";

					// ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜ ˜˜˜˜˜˜˜
					leftToRight(root);

					// ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜
					cout << "\n\nSize Tree: " << sizeTree(root) << endl;

					// ˜˜˜˜˜˜ ˜˜˜˜ ˜˜˜ ˜˜˜˜˜˜ ˜ ˜˜˜˜˜˜
					cout << "Vvedite Key >: ";
					int keyTreeSearch;
					cin >> keyTreeSearch;

					// ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜ ˜˜˜˜˜˜
					cout << endl;
					searchInTree(root, keyTreeSearch);

					system("pause");
				}
				else if (menu_input == 5) {
					break;
				}
			}
			
		}

	}
}

void BinSearch(spis* head, int key, Que*& First, Que*& Last) {
	IndexArray *indexArray[N];
	spis* current = head;
	int i = 0;
	int Year;

	// ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜ ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜
	while (current->next && i < N) {
		sscanf(current->birthdate, "%*d-%*d-%d", &Year);
		indexArray[i] = new IndexArray; // ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜
		indexArray[i]->data = Year;
		indexArray[i]->indexSpisElement = current;
		current = current->next;
		i++;
	}

	int L = 0;
	int R = N - 1;
	
	while (L < R) {
		int M = (L + R) / 2;
		Year = indexArray[M]->data;
		if (Year < key) {
			L = M + 1;
		}
		if (Year < key) {
			L = M + 1;
		}
		else {
			R = M;
		}
	}

	// ˜˜˜˜˜˜˜˜˜, ˜˜˜ ˜˜˜˜˜˜˜˜ ˜ ˜˜˜˜˜˜ key ˜˜˜˜˜˜˜
	if (R >= N) {
		cout << "No elements with key " << key << " found." << endl;
		return;
	}

	// ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜
	Que* elemetQue = new Que;
	elemetQue->spisElement = indexArray[R]->indexSpisElement;
	First = elemetQue;
	First->sizeQue = 1;
	Last = elemetQue;
	R++;

	// ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜ ˜ ˜˜˜˜˜˜ key ˜ ˜˜˜˜˜˜˜
	while (indexArray[R]->data == key) {
		Que* elemetQue = new Que;
		elemetQue->spisElement = indexArray[R]->indexSpisElement;
		Last->next = elemetQue;
		Last = elemetQue;
		First->sizeQue += 1;
		R++;
	}
}

spis* Split(spis* head, int step) {
	if (!head) return NULL;

	// ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜ head
	for (int i = 1; head->next && i < step; i++) {
		head = head->next;
	}

	// ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜ ˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜ ˜˜˜˜˜˜
	spis* rest = head->next;

	// ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜ ˜˜˜˜˜ ˜˜˜˜˜˜ ˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜
	head->next = NULL;
	return rest;
}

spis* Merge(spis* left, spis* right, spis* tail) {
	spis temp;
	spis* current = &temp;

	while (left && right) {
		// ˜˜˜˜˜˜˜˜˜ ˜˜˜˜ ˜˜ ˜˜˜˜˜˜˜˜˜˜˜˜: ˜˜˜˜, ˜˜˜˜˜ ˜ ˜˜˜
		int leftDay, leftMonth, leftYear;

		sscanf(left->birthdate, "%d-%d-%d", &leftDay, &leftMonth, &leftYear);

		int rightDay, rightMonth, rightYear;

		sscanf(right->birthdate, "%d-%d-%d", &rightDay, &rightMonth, &rightYear);

		bool flag = false;

		// ˜˜˜˜˜˜˜˜˜˜ ˜˜˜, ˜˜˜˜˜ ˜ ˜˜˜˜
		if (leftYear < rightYear) {
			flag = true;
		}
		else if (leftYear == rightYear) {
			if (leftMonth < rightMonth) {
				flag = true;
			}
			else if (leftMonth == rightMonth) {
				if (leftDay < rightDay) {
					flag = true;
				}
			}
		}


		if (flag) {
			current->next = left;
			left = left->next;
		}
		else {
			current->next = right;
			right = right->next;
		}
		current = current->next;
	}

	current->next = left ? left : right;
	while (current->next) {
		current = current->next;
	}

	tail->next = temp.next;
	return current;
}

spis* MergeSort(spis* head) {
	// ˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜ ˜˜˜ ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜ ˜˜˜˜˜˜˜
	if (!head || !head->next) return head;

	int size = 0;
	spis* spisok = head;

	// ˜˜˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜˜
	while (spisok) {
		size++;
		spisok = spisok->next;
	}

	spis temp; // ˜˜˜ ˜˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜
	temp.next = head;
	spis* left, * right, * tail;

	for (int step = 1; step < size; step *= 2) {
		spisok = temp.next;
		tail = &temp;
		while (spisok) {
			left = spisok;
			right = Split(left, step);
			spisok = Split(right, step);
			tail = Merge(left, right, tail);
		}
	}

	// ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜ head ˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜˜
	head = temp.next;

	return head;
}

void OutputDatabase(spis* head)
{
	system("cls");
	int i = 0;
	spis* pp = head;
	while (pp)
	{
		i++;
		cout <<  pp->FIO << "\t" << pp->deprt << "\t" << pp->post << "\t" << pp->birthdate << endl;
		pp = pp->next;
	}

	cout << "\n";
	system("pause");
}

void OutputDatabaseByPages(spis* head) {
	int k = 20, i = 0;
	spis* p = head;
	int numberStarn = 0;
	while (true) {
		system("cls");
		int temp = 0;
		numberStarn++;
		cout << "---------Stranica number " << numberStarn << "---------\n\n";
		while (temp != k) {
			i++;
			cout << p->FIO << "\t" << p->deprt << "\t" << p->post << "\t" << p->birthdate << endl;
			p = p->next;
			temp++;
		}
		int menuStranic = 0;
		cout << "\n\n1 - Next\n2 - Exit\n";
		cin >> menuStranic;
		if (menuStranic == 2) {
			break;
		}
	}
}

void CreateBase(FILE* fp, spis*& head, spis*& tail) {
	int i = 0;
	int k = 0;
	while ((i++) < N)
	{
		spis* p = new spis;
		fread(&(p->FIO), sizeof(char), 30, fp);
		fread(&(p->deprt), sizeof(short int), 1, fp);
		fread(&(p->post), sizeof(char), 22, fp);
		fread(&(p->birthdate), sizeof(char), 10, fp);
		p->next = NULL;
		p->prev = tail;
		if (tail != NULL) {
			tail->next = p;
		}
		tail = p; // ˜˜˜˜˜˜˜˜˜˜ tail ˜˜ ˜˜˜˜˜ ˜˜˜˜˜˜˜

		if (head == NULL) {
			head = tail; // ˜˜˜˜ ˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜˜, ˜˜˜˜˜˜˜˜˜˜˜˜˜ head ˜˜ ˜˜˜˜
		}
	}
}


