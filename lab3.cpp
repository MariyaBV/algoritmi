//21. Имеется  информация о взаимных долгах предприятий.Если
//имеется цепочка предприятий
//A1(a1)-- > A2(a2)-- > ... -- > An(an)-- > A1(a1),
//где Ai - наименование предприятия, а ai - размер долга, то с
//каждого из предприятий можно списать долг в размере min(ai), а
//предприятие с минимальным долгом исключить из списка должников
//следующему в цепочке  предприятию.Требуется  найти  варианты
//взаимного  списания  долгов.Предложить критерии оптимальности
//и способы оптимизации(11). Белякова Мария

#include "pch.h"
//#include <limits>
#include <iterator>
#include <algorithm>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cstddef>
#include <windows.h>
#include <iostream> //поток ввода-вывода (Input/Output Stream)
#include <fstream> //для чтения и записи в файл
#include <iomanip> //инструменты для работы с форматированием вывода
#include <cstdlib> /*функции динамического управления памятью, 
генерации случайных чисел, целой арифметики, поиска, сортировки и преобразования типов данных*/

using namespace std;
ofstream output;
//vector<bool> mark;

int ncycle = 0;
const int MAXN = 255;
//const int MAXM = 255;
//int g[MAXN][MAXM];
//int cycle[MAXN];
vector <vector<int>> g; //сам граф, заданный ребрами
vector <vector<int>> edges;
vector<int> color;
vector<int> mark;
vector<int> prior;
//vector<int> cycle[MAXN];
//vector<int> g[MAXN];
//vector <vector<int>> p;
vector<int> p; // буфер, для хранения ребер по ходу поиска
int cycle_start, cycle_end;
bool CycleFound = false;
vector <vector<int>> cycle;
int toV = 0;

int add_cycle(int cycle_end, int cycle_start)
{
	/*for (int i = 0; i < p.size(); ++i) {
		cout << "p = ";
		printf("%d ", p[i]);
	}*/
	cout << "in add_cycle" << endl;
	vector <int> vCycle;
	vCycle.clear();
	vCycle.push_back(cycle_start);
	cout << "cycle_start = " << cycle_start << "; cycle_end = " << cycle_end << endl;

	for (int v = cycle_end; v != cycle_start; v = p[v]) {
		vCycle.push_back(v);
		cout << "v = " << v << "; p[v] = " << p[v];
	}
	cout << endl;
	vCycle.push_back(cycle_start);

	reverse(vCycle.begin(), vCycle.end());

	cycle.push_back(vCycle);

	for (int i = 0; i < vCycle.size(); ++i) {
		cout << "vCycle = ";
		printf("%d ", vCycle[i]);
	}

	return vCycle.size();
}

void dfs(int v, int &n)
{
	vector <int> vCycle;
	color[v] = 1;
	cout << "v = " << v << endl;
	cout << "g.size() = " << g.size() << " g[0][0] и g[1][1] " << g[0][0] << g[1][1] << " g[0][1] и g[1][0] " << g[0][1] << g[1][0] << endl;
	if ((g.size() == 2) && (g[0][0] == g[1][1]) && (g[0][1] == g[1][0])) {
		cout << "g[0][0] == g[1][1]" << endl;
		CycleFound = true;
		vCycle.push_back(g[0][0]);
		vCycle.push_back(g[0][1]);
		vCycle.push_back(g[1][1]);
		cycle.push_back(vCycle);
	}
	else if (g.size() > 2) {
		for (int i = 0; i < g.size(); i++) // g[v].size();
		{
			cout << "g[i][0] = " << g[i][0] << endl;
			if (g[i][0] == v)
			{
				cout << "g[i][1] = " << g[i][1] << " тип = " << typeid(g[i][1]).name() << endl;
				toV = g[i][1];//номер вершины, в которую собираемся пойти
				cout << "toV = " << toV << " color[toV] =  " << color[toV] << endl;

				if (color[toV] != 1)
				{
					cout << "toV = " << toV << "; v = " << v << "; p[toV] = " << p[toV] << endl;
					p[toV] = v;
					cout << "p[toV] = " << p[toV] << endl;
					dfs(toV, n);
				}
				else if (color[toV] == 1)
				{
					CycleFound = true;
					cout << "toV = " << toV << "; v = " << v << endl;
					if (v != toV) {
						cout << "vnutri" << endl;
						ncycle++;
						add_cycle(v, toV);
					}
				}
			}
		}
		color[v] = 2;
	}
}

vector<int> &split(const string &s, char delim, vector<int> &items)
{
	int i = 0;
	stringstream ss(s);
	string item;
	items.clear();
	while (getline(ss, item, delim)) {
		if (!item.empty()) {
			int result = stoi(item, nullptr, 10);
			items.push_back(result);
			i++;
		}
	}

	return items;
}


int main(int argc, char * argv[])
{
	/*чтобы распознавался русский язык в консоли и в файле*/
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	// Поддержка кириллицы в консоли Windows
	setlocale(LC_CTYPE, "rus");

	string str;
	char ch = 0;
	int sortingNumber = 0, companyNumber = 0;

	if (argc != 4) {
		cout << "Invalid arguments count\n"
			<< "Usage: copyfile.exe <input file 1> <input file 2> <output file>\n";
		return 1;
	}

	/*Объявили переменную типа ifstream (input file stream, поток для чтения из файла),
	проинициализировав его именем входного файла*/

	ifstream inputFile1(argv[1]);
	ifstream inputFile2(argv[2]);

	// вызываем метод is_open() у объекта input, который вернет true, если файл был открыт
	if (!inputFile1.is_open()) {
		cout << "Failed to open " << argv[1] << " for reading\n";
		return 1;
	}

	if (!inputFile2.is_open()) {
		cout << "Failed to open " << argv[2] << " for reading\n";
		return 1;
	}

	// создали поток для записи в выходной файл
	ofstream output(argv[3]);
	if (!output.is_open()) {
		output << "Failed to open " << argv[3] << " for writing\n";
		return 1;
	}
	
	/*cout << "Укажите номер фирмы относительно которой нужно произвести списание долгов: " << endl;
	cin >> companyNumber;

	cout << "Каким способом провести списание долгов? Введите номер под которым находится метод:" << endl;
	cout << "1. По возрастанию числа сторон" << endl;
	cout << "2. По максимальной сумме списания" << endl;
	cout << "3. По выбору пользователя" << endl;
	cin >> sortingNumber;*/

	int n = 0;
	vector<int> items;

	while (getline(inputFile1, str)) 
	{
		split(str, ' ', items);
		g.push_back(items);
		n++;
	}
	for_each(g.begin(), g.end(), [](vector<int>& ivec)
	{
		for_each(ivec.begin(), ivec.end(), [](int i)
		{
			cout << left << setw(5) << i;
		});
		cout << endl;
	});
	p.assign(n, -1);
	color.assign(n, 0);
	for (int k = 0; k < n; k++) {
		output << "k / color[k] / n = " << k << color[k] << n << endl;
		if ((color[k] != 1) && (n != 1)) {
			cout << "g[k][0] = " << g[k][0] << endl;
			dfs(k, n);
		}
	}

	if (!CycleFound) {
		cout << "Нет циклов для списывания долгов." << endl;
	} else {
		cout << "Вывод массива" << "; n = " << n << endl;
		for_each(cycle.begin(), cycle.end(), [](vector<int>& ivec)
		{
			for_each(ivec.begin(), ivec.end(), [](int i)
			{
				cout << left << setw(5) << i;
			});
			cout << endl;
		});

		sort(cycle.begin(), cycle.end());//сортируем
		cycle.resize(unique(cycle.begin(), cycle.end()) - cycle.begin());//и удаляем дубликаты

		cout << "Вывод массива после сортировки" << "; n = " << n << endl;
		for_each(cycle.begin(), cycle.end(), [](vector<int>& ivec)
		{
			for_each(ivec.begin(), ivec.end(), [](int i)
			{
				cout << left << setw(5) << i;
			});
			cout << endl;
		});
	}


	inputFile1.close();
	inputFile2.close(); // закрытие потока
	while (inputFile1.get(ch))
	{
		if (!output.put(ch))
		{
			output << "Failed to save data on disk\n";
			return 1;
		}
	}

	while (inputFile2.get(ch))
	{
		if (!output.put(ch))
		{
			output << "Failed to save data on disk\n";
			return 1;
		}
	}

	if (!output.flush()) // Если не удалось сбросить данные на диск
	{
		output << "Failed to save data on disk\n";
		return 1;
	}

	return 0;
}