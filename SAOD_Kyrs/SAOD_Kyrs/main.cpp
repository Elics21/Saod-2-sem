#include <iostream>
#include <cstdio>
#include <queue>
#define N 4000

using namespace std;

struct spis {
	char FIO[30];
	short int deprt;
	char post[22];
	char birthdate[10];
	spis* next;
	spis* prev;
};

struct Que {
	int data;
	int sizeQue;
	Que* next;
};

struct IndexArray{
	int data;
	int index;
};


////////////////////////ПРОТОТИПЫ ФУНКЦИЙ/////////////////////////////////////////////
spis* Split(spis* head, int step);
spis* Merge(spis* left, spis* right, spis* tail);
spis* MergeSort(spis* head);
void OutputDatabase(spis* head);
void OutputDatabaseByPages(spis* head);
void CreateBase(FILE* fp, spis*& head, spis*& tail);
void EndlessMenu(spis*& head, spis*& tail);
//////////////////////////////////////////////////////////////////////////////////////

void CreateIndexArray(spis* head, IndexArray indexArray[]) {
	spis* current = head;
	int Day, Month, Year;
	for (int i = 0; i < N; i++) {
		sscanf(current->birthdate, "%d-%d-%d", &Day, &Month, &Year);
		indexArray[i].data = Year;
		indexArray[i].index = i;
		current = current->next;
	}
}

void BinSearch(IndexArray indexArray[], int key, Que*& First, Que*& Last) {
	int L = 1;
	int R = N;
	while (L < R) {
		int M = (L + R) / 2;
		if (indexArray[M].data < key) {
			L = M + 1;
		}
		else {
			R = M;
		}
	}

	// Проверяем, что элементы с ключом key найдены
	if (R >= N || indexArray[R].data != key) {
		cout << "No elements with key " << key << " found." << endl;
		return;
	}

	// Определяем начальный элемент очереди
	Que* elemetQue = new Que;
	elemetQue->data = R;
	First = elemetQue;
	First->sizeQue = 1;
	Last = elemetQue;
	R++;

	// Добавляем остальные элементы с ключом key в очередь
	while (indexArray[R].data == key) {
		Que* elemetQue = new Que;
		elemetQue->data = R;
		Last->next = elemetQue;
		Last = elemetQue;
		First->sizeQue += 1;
		R++;
	}
}



int main()
{
	FILE* fp = fopen("base.dat", "rb"); //октрываем для чтения

	//проверка на открытие
	if (fp == NULL) { 
		cout << "Error with open base.dat" << endl;
		return -1;
	}

	spis* head = NULL; // начало
	spis* tail = NULL; // конец

	// Формируем базу данных
	CreateBase(fp, head, tail);
	
	//создание бесконечного меню
	EndlessMenu(head, tail);

	return 0;

}
void EndlessMenu(spis*& head, spis*& tail) {
	while (true)
	{
		int menu;
		system("cls");
		cout << "----Menu----\n\n"
			<< "1 - Vse zapisi\n" //Выгружаем все записи
			<< "2 - Po stranicam\n" //загрузка по страницам, указывыая длину страниц
			<< "3 - Sortizovanie zapisi\n" //сортировка
			<< "4 - Poisk po klychy\n" //сортировка
			<< "9 - Exit\n"; //очищаем памяти и выход

		cin >> menu;

		//очищаем памяти и выход
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
		//Выгружаем все записи
		else if (menu == 1) {
			OutputDatabase(head);
		}
		//загрузка по страницам, указывыая длину страниц
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

			//сортируем
			head = MergeSort(head);

			//формируем индексный массив
			IndexArray indexArray[N];
			CreateIndexArray(head, indexArray);

			//Объявляем очередь
			Que* First = nullptr; // Начало
			Que* Last = nullptr; // Конец

			BinSearch(indexArray, key, First, Last);

			int queSize = First->sizeQue;
			int firstIdSpis = First->data;
			Que* queElement = First;
			spis* spisElement = head;
			int i = 0;
			while (queSize--){
				//доходим до нужного элемента в списке
				while (firstIdSpis-- > 0){
					spisElement = spisElement->next;
					i++;
				}

				cout << i++ << "\t" << spisElement->FIO << "\t" 
					<< spisElement->deprt << "\t" << spisElement->post << "\t" 
					<< spisElement->birthdate << endl;

				spisElement = spisElement->next;
				queElement = queElement->next;
			}
			
			system("pause");
		}

	}
}

spis* Split(spis* head, int step) {
	if (!head) return NULL;

	// Перемещаем указатель head
	for (int i = 1; head->next && i < step; i++) {
		head = head->next;
	}

	// Сохраняем указатель на вторую часть списка
	spis* rest = head->next;

	// Разрываем связь между первой и второй частями списка
	head->next = NULL;
	return rest;
}

spis* Merge(spis* left, spis* right, spis* tail) {
	spis temp;
	spis* current = &temp;

	while (left && right) {
		// Разбиваем дату на составляющие: день, месяц и год
		int leftDay, leftMonth, leftYear;

		sscanf(left->birthdate, "%d-%d-%d", &leftDay, &leftMonth, &leftYear);

		int rightDay, rightMonth, rightYear;

		sscanf(right->birthdate, "%d-%d-%d", &rightDay, &rightMonth, &rightYear);

		bool flag = false;

		// Сравниваем год, месяц и день
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
	// Если очередь пуста или содержит только один элемент
	if (!head || !head->next) return head;

	int size = 0;
	spis* spisok = head;

	// Подсчитываем размер очереди
	while (spisok) {
		size++;
		spisok = spisok->next;
	}

	spis temp; // Для объединения очереди
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

	// Обновляем указатель head на начало очереди
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
		cout << i << "\t" << pp->FIO << "\t" << pp->deprt << "\t" << pp->post << "\t" << pp->birthdate << endl;
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
			cout << i << "\t" << p->FIO << "\t" << p->deprt << "\t" << p->post << "\t" << p->birthdate << endl;
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
		tail = p; // Перемещаем tail на конец очереди
		if (head == NULL) {
			head = tail; // Если это первый элемент, устанавливаем head на него
		}
	}
}


