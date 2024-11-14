#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <string>
#include <fstream>

using namespace std;

#pragma region prototype
void try_input(const char message[], unsigned short& var);
#pragma endregion prototype

#pragma region stundent_struct
struct Student {
	void fill_vars(){
		cout << "Введіть прізвище: "; cin >> this->prizv;
		try_input("Введіть оцінку №1: ", this->mark_1);
		try_input("Введіть оцінку №2: ", this->mark_2);
		try_input("Введіть оцінку №3: ", this->mark_3);
	}
	double return_average() { return (this->mark_1 + this->mark_2 + this->mark_3) / 3.; }
	string return_prizv() { return this->prizv; }
	unsigned short return_mark_1() { return this->mark_1; }
	unsigned short return_mark_2() { return this->mark_2; }
	unsigned short return_mark_3() { return this->mark_3; }

private:
	string prizv;
	unsigned short mark_1;
	unsigned short mark_2;
	unsigned short mark_3;
};
#pragma endregion stundent_struct

void SaveToFile(Student* p, const int N, const char* filename)
{
	ofstream fout(filename, ios::binary); // відкрили бінарний файл запису
	fout.write((char*)&N, sizeof(N)); // записали кількість елементів
	for (int i = 0; i < N; i++)
		fout.write((char*)&p[i], sizeof(Student)); // записали елементи масиву
	fout.close();
}

void LoadFromFile(Student*& p, int& N, const char* filename)
{
	delete[] p;

	ifstream fin(filename, ios::binary);
	fin.read((char*)&N, sizeof(N)); // прочитали кількість елементів

	p = new Student[N];
	for (int i = 0; i < N; i++)
		fin.read((char*)&p[i], sizeof(Student)); // прочитали елементи масиву
	fin.close();
}

void sort(Student* arr, int n, const char criterion[]) {
	Student tmp;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			if (criterion == "average" && arr[j].return_average() > arr[j + 1].return_average()) {
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
			else if (criterion == "prizv" && arr[j].return_prizv() > arr[j + 1].return_prizv()) {
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
			else if (criterion == "mark1") {
				if (arr[j].return_mark_1() > arr[j + 1].return_mark_1()) {
					tmp = arr[j];
					arr[j] = arr[j + 1];
					arr[j + 1] = tmp;
				}
			}
			else if (criterion == "mark2") {
				if (arr[j].return_mark_2() > arr[j + 1].return_mark_2()) {
					tmp = arr[j];
					arr[j] = arr[j + 1];
					arr[j + 1] = tmp;
				}
			}
			else if (criterion == "mark3") {
				if (arr[j].return_mark_3() > arr[j + 1].return_mark_3()) {
					tmp = arr[j];
					arr[j] = arr[j + 1];
					arr[j + 1] = tmp;
				}
			}
		}
	}
}

int calc_n(int n) {
	int res = 0;
	while (n != 0) {
		n /= 10;
		res++;
	}
	return res;
}

int lenOfLongestPrizv(Student* arr, int n) {
	int res = -1;
	for (int i = 0; i < n; i++) {
		if (static_cast<int>(arr[i].return_prizv().length()) > res) {
			res = static_cast<int>(arr[i].return_prizv().length());
		}
	}
	return res;
}


void add(Student*& arr, int& n) {
	Student* newArr = new Student[n + 1];
	for (int i = 0; i < n; i++) {
		newArr[i] = arr[i];
	}

	cout << "Додайте дані нового студента:\n";
	newArr[n].fill_vars();
	delete[] arr; 
	arr = newArr; 
	n++; 

	SaveToFile(arr, n, "data.txt");
}

void correct(Student* arr, int n) {
	string prizv;
	cout << "Введіть прізвище студента, інформацію про якого потрібно редагувати: ";
	cin >> prizv;

	for (int i = 0; i < n; i++) {
		if (arr[i].return_prizv() == prizv) {
			cout << "Редагування інформації про студента " << prizv << endl;
			arr[i].fill_vars(); 
			SaveToFile(arr, n, "data.txt");
			return;
		}
	}
	cout << "Студента з прізвищем " << prizv << " не знайдено.\n";
}

void del(Student*& arr, int& n) {
	if (arr == nullptr || n <= 0) {
		cout << "Масив порожній.\n";
		return;
	}

	string prizv;
	cout << "Введіть прізвище студента, якого потрібно видалити: ";
	cin >> prizv;

	int indexToDelete = -1;
	for (int i = 0; i < n; i++) {
		if (arr[i].return_prizv() == prizv) {
			indexToDelete = i;
			break;
		}
	}

	if (indexToDelete == -1) {
		cout << "Студента з прізвищем " << prizv << " не знайдено.\n";
		return;
	}

	try {
		Student* newArr = new Student[n - 1];

		for (int i = 0, j = 0; i < n; i++) {
			if (i != indexToDelete) {
				newArr[j] = arr[i];
				j++;
			}
		}
 
		delete[] arr;
		arr = newArr;
		n--;

		SaveToFile(arr, n, "data.txt");

		cout << "Студента з прізвищем " << prizv << " видалено.\n";
	}
	catch (std::bad_alloc& e) {
		cout << "Помилка виділення пам'яті: " << e.what() << endl;

		return;
	}
	catch (...) {
		cout << "Невідома помилка при видаленні.\n";
		return;
	}
}

void printStudents(Student* arr, int n) {
	int width = lenOfLongestPrizv(arr, n);
	(width <= 8) ? (width = 8) : (NULL);
	int maxDigits = calc_n(n);

	for (int i = 0; i < (maxDigits - 1) + (width - 8) + 60; i++) {
		cout << "=";
	}
	cout << endl;

	cout << "|" << setw(maxDigits) << right << "№";
	cout << "|" << setw(width) << left << "Прізвище" << "| Оцінка 1 | Оцінка 2 | Оцінка 3 | Середній бал |" << endl;

	for (int i = 0; i < (maxDigits - 1) + (width - 8) + 60; i++) {
		cout << "-";
	}
	cout << endl;

	for (int i = 0; i < n; i++) {
		cout << "|" << setw(maxDigits) << right << i + 1;
		cout << "|" << setw(width) << left << arr[i].return_prizv();
		cout << "|" << setw(10) << left << arr[i].return_mark_1();
		cout << "|" << setw(10) << left << arr[i].return_mark_2();
		cout << "|" << setw(10) << left << arr[i].return_mark_3();
		cout << "|" << setw(14) << left << arr[i].return_average() << "|" << endl;
	}

	for (int i = 0; i < (maxDigits - 1) + (width - 8) + 60; i++) {
		cout << "-";
	}
	cout << endl;

}

void try_input(const char message[], unsigned short& var) {
	bool fail;
	do {
		fail = false;
		cout << message; cin >> var;
		if (cin.fail()) {
			fail = true;
			cout << "Неправильний ввід!\n";
			cin.clear();
			cin.ignore(65535, '\n');
		}
	} while (fail);

}

void menu(Student* arr, int n) {
	int option_1, option_2;
	bool continue_1, continue_2;
	while(true) {
		cout << "Меню:\n[1] Додати студента\n[2] Видалити студента\n[3]Редагувати інформацію про студента\n[4] Вивід списку студентів\n[5] Сортування списку студентів\n[6] Вихід з програми\n : ";
		cin >> option_1;
		
		switch (option_1) {
		case 1:
			add(arr,n);
			break;

		case 2:
			del(arr,n);
			break;

		case 3:
			correct(arr, n);
			break;

		case 4:
			printStudents(arr, n);
			break;

		case 5:
			continue_1 = true;
			while (continue_1) {
				cout << "Критерії сортування:\n[1] За середнім балом\n[2] За прізвищем\n[3] За оцінкою 1-го предмету\n[4] За оцінкою 2-го предмету\n[5] За оцінкою 3-го предмету\n[6] Назад\n : ";
				cin >> option_2;

				switch (option_2) {
				case 1:
					sort(arr, n, "average");
					continue_1 = false;
					break;
				case 2:
					sort(arr, n, "prizv");
					continue_1 = false;
					break;
				case 3:
					sort(arr, n, "mark1");
					continue_1 = false;
					break;
				case 4:
					sort(arr, n, "mark2");
					continue_1 = false;
					break;
				case 5:
					sort(arr, n, "mark3");
					continue_1 = false;
					break;

				case 6:
					continue_1 = false;
					break;

				default:
					cout << "Неправильний ввід!\n";
					break;
				}
			}
			break;

		case 6:
			return;

		default:
			cout << "Неправильний ввід!\n";
			break;
		}
	}
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	int n;
	Student* arr = nullptr;

	ifstream file_r("data.txt", ios::binary);

	if (!file_r.is_open()) {
		file_r.close();
		n = 0;
		ofstream file_w;
		file_w.open("data.txt", ios::binary);
		file_w.write((char*)&n, sizeof(n));
		file_w.close();
	}
	if (file_r.is_open()) {
		LoadFromFile(arr, n, "data.txt");
		if (n > 0) {
			printStudents(arr, n);
		}
		file_r.close();
	}

	menu(arr, n);

	delete[] arr;
}