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
#include <cstdlib> //функции динамического управления памятью, 
using namespace std;

int ncycle = 0;
const int MAXN = 255;
vector <vector<int>> g; //сам граф, заданный ребрами
vector <vector<int>> edges;
vector<int> color;
vector<int> mark;
vector<int> prior;
vector<int> p; // буфер, для хранения ребер по ходу поиска
int cycle_start, cycle_end;
bool CycleFound = false;
vector <vector<int>> cycle;
int toV = 0;

int add_cycle(int cycle_end, int cycle_start)
{
	vector <int> vCycle;
	vCycle.clear();
	vCycle.push_back(cycle_start);
	for (int v = cycle_end; v != cycle_start; v = p[v]) {
		vCycle.push_back(v);
	}
	vCycle.push_back(cycle_start);
	reverse(vCycle.begin(), vCycle.end());
	cycle.push_back(vCycle);

	return vCycle.size();
}

void dfs(int v, int &n)
{
	vector <int> vCycle;
	color[v] = 1;
	if ((g.size() == 2) && (g[0][0] == g[1][1]) && (g[0][1] == g[1][0])) {
		CycleFound = true;
		vCycle.push_back(g[0][0]);
		vCycle.push_back(g[0][1]);
		vCycle.push_back(g[1][1]);
		cycle.push_back(vCycle);
	}
	else if (g.size() > 2) {
		for (int i = 0; i < g.size(); i++) // g[v].size();
		{
			if (g[i][0] == v)
			{
				toV = g[i][1];//номер вершины, в которую собираемся пойти
				if (color[toV] != 1)
				{
					p[toV] = v;
					dfs(toV, n);
				}
				else if (color[toV] == 1)
				{
					CycleFound = true;
					if (v != toV) {
						ncycle++;
						add_cycle(v, toV);
					}
				}
			}
		}
		color[v] = 2;
	}
}

int numberI = 0, beginMIN = 0, MIN = 0;
vector <int> to_vector;

vector<int> sortVector(vector<int> &from_vector)
{
	to_vector.clear();
	MIN = from_vector[0];
	beginMIN = from_vector[0];
	for (int i = 0; i < from_vector.size(); i++) {
		if ((from_vector[i] < MIN) && (from_vector[i] != from_vector.size())) {
			MIN = from_vector[i];
			numberI = i;
		}
	};
	if (MIN != beginMIN) {
		copy(from_vector.begin(), from_vector.end(), back_inserter(to_vector));
		from_vector.clear();
		to_vector.erase(to_vector.end() - 1, to_vector.end());
		copy(to_vector.begin() + numberI, to_vector.end(), back_inserter(from_vector));
		copy(to_vector.begin(), to_vector.begin() + numberI + 1, back_inserter(from_vector));
	}

	return from_vector;
}

vector<int> stroki;
vector<int> q;

int byIncreasingNumberOfSides(vector<vector<int>> &g, vector<int> vec)
{
	stroki.clear();
	q.clear();
	int v = 0, MIN;
	bool vecEnd = false;

	for (int k = 0; k < vec.size(); k++)
	{
		for (int i = 0; i < g.size(); i++)
		{
			if ((k + 1) >= vec.size()) {
				break;
				vecEnd = true;
			}
			if ((g[i][0] == vec[k]) && (g[i][1] == vec[k + 1]) && (k < vec.size()))
			{
				q.push_back(g[i][2]);
				stroki.push_back(i);
			}
		}
	};

	MIN = q[0];
	for (int l = 0; l < q.size(); l++) {
		if (q[l] < MIN) {
			MIN = q[l];
		}
	};

	if (q.size() > 1) {
		for (int f = 0; f < stroki.size(); f++)
		{
			int i = stroki[f];
			g[i][2] -= MIN;
		}
	}
	
	return MIN;
}
pair<vector<int>, int> pair1;

pair<vector<int>, int> maxWriteOffAmount(vector<vector<int>> &g, vector<int> vec)
{
	stroki.clear();
	q.clear();
	int v = 0, MIN, MAXSum = 0, MAX, MINv;
	bool vecEnd = false;

	for (int k = 0; k < vec.size(); k++)
	{
		for (int i = 0; i < g.size(); i++)
		{
			if ((k + 1) >= vec.size()) {
				break;
				vecEnd = true;
			}
			if ((g[i][0] == vec[k]) && (g[i][1] == vec[k + 1]) && (k < vec.size()))
			{
				q.push_back(g[i][2]);
				stroki.push_back(i);
			}
		}
	};

	MIN = q[0];
	for (int l = 0; l < q.size(); l++) {
		if (q[l] < MIN) {
			MIN = q[l];
		}
	};
	
	pair1.first = stroki;
	pair1.second = MIN;
	
	return pair1;
}

void outInfo(vector<int> ivec, vector<string> nameCompany)
{
	for_each(ivec.begin(), ivec.end(), [&nameCompany](int i) {
		cout << nameCompany[i - 1] << "      ";
	});
	cout << endl;
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
	setlocale(LC_CTYPE, "rus");// Поддержка кириллицы в консоли Windows

	string str;
	char ch = 0;
	int sortingNumber = 0, n = 0;
	vector<int> items;
	vector <string> companyNames;

	if (argc != 3) {
		cout << "Invalid arguments count\n"
			<< "Usage: copyfile.exe <input file 1> <input file 2>\n";
		return 1;
	}

	/*Объявили переменную типа ifstream (input file stream, поток для чтения из файла), проинициализировав его именем входного файла*/
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
	while (getline(inputFile1, str)) 
	{
		split(str, ' ', items);
		g.push_back(items);
		n++;
	}
	while (getline(inputFile2, str))
	{
		companyNames.push_back(str);
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
		if ((color[k] != 1) && (n != 1)) { 
			dfs(k, n);
		}
	}

	if (!CycleFound) {
		cout << "Нет циклов для списывания долгов." << endl;
	} else {
		sort(cycle.begin(), cycle.end());//сортируем
		cycle.resize(unique(cycle.begin(), cycle.end()) - cycle.begin());//удаляем дубликаты
		for_each(cycle.begin(), cycle.end(), [](vector<int>& ivec)
		{
			sortVector(ivec);
		});
		sort(cycle.begin(), cycle.end());//сортируем
		cycle.resize(unique(cycle.begin(), cycle.end()) - cycle.begin());//удаляем дубликаты

		cout << "Все существующие циклы: " << endl;
		for_each(cycle.begin(), cycle.end(), [](vector<int>& ivec)
		{
			for_each(ivec.begin(), ivec.end(), [](int i)
			{
				cout << left << setw(5) << i;
			});
			cout << endl;
		});

		cout << "Каким способом провести списание долгов? Введите номер под которым находится метод (если нужно закончить сортировку напишите 0):" << endl;
		cout << "1. По возрастанию числа сторон" << endl;
		cout << "2. По максимальной сумме списания" << endl;
		cout << "3. По выбору пользователя" << endl;
		cin >> sortingNumber;

		while (sortingNumber != 0) {
			if (sortingNumber == 1) {
				for_each(cycle.begin(), cycle.end(), [](vector<int>& ivec)
				{
					int MIN = byIncreasingNumberOfSides(g, ivec);
					cout << "Цикл по которому произвели списание: " << endl;
					for_each(ivec.begin(), ivec.end(), [](int i)
					{
						cout << left << setw(5) << i;
					});
					cout << endl;
					cout << "Списали с каждой пары долги в размере: " << MIN << endl;
				});
			}
			else if (sortingNumber == 2) {
				int MAXSum, MAX, MINv;
				vector<int> maxSumma;
				vector <vector<int>> maxSumStr;
				vector<int> strForMaxsum;
				vector<int> MINvalue;
				for_each(cycle.begin(), cycle.end(), [&MAXSum, &maxSumma, &MINvalue, &maxSumStr](vector<int>& ivec)
				{
					maxWriteOffAmount(g, ivec);
					MAXSum = pair1.second * (ivec.size() - 1);
					maxSumma.push_back(MAXSum);
					MINvalue.push_back(pair1.second); // минимум
					maxSumStr.push_back(pair1.first); // строка
				});
				MAX = maxSumma[0];
				MINv = MINvalue[0];
				strForMaxsum = maxSumStr[0];
				for (int l = 0; l < maxSumma.size(); l++) {
					if (maxSumma[l] > MAX) {
						MAX = maxSumma[l];
						strForMaxsum = maxSumStr[l];
						MINv = MINvalue[l];
					}
				};
				cout << "Списали по такому пути долги в размере: " << MINv << endl;
				for (int c = 0; c < strForMaxsum.size(); c++)
				{
					int i = strForMaxsum[c];
					g[i][2] -= MINv;
					cout << companyNames[g[i][0] - 1] << "      ";
				}
				cout << endl;
			}
			else if (sortingNumber == 3) {
				int put = 1, userPut;
				cout << "Выберите по какому пути сделать списание:" << endl;
				for_each(cycle.begin(), cycle.end(), [&companyNames, &put](vector<int>& ivec)
				{
					cout << put << ":    ";
					outInfo(ivec, companyNames);
					put++;
				});

				cin >> userPut;
				maxWriteOffAmount(g, cycle[userPut - 1]);
				int MINvector = pair1.second;
				vector<int> stroki = pair1.first;
				for (int c = 0; c < stroki.size(); c++)
				{
					int i = stroki[c];
					g[i][2] -= MINvector;
				}
				cout << "Списали с каждой пары долги в размере: " << MINvector << endl;
			}
			else {
				cout << "Вы ввели неверный номер!" << endl;
			}

			cout << "Результат: " << endl;
			for_each(g.begin(), g.end(), [](vector<int>& ivec)
			{
				for_each(ivec.begin(), ivec.end(), [](int i)
				{
					cout << left << setw(5) << i;
				});
				cout << endl;
			});
			cout << endl;
			cout << "Каким способом провести списание долгов? Введите номер под которым находится метод (если нужно закончить сортировку и получить конечный результат напишите 0):" << endl;
			cout << "1. По возрастанию числа сторон" << endl;
			cout << "2. По максимальной сумме списания" << endl;
			cout << "3. По выбору пользователя" << endl;
			cin >> sortingNumber;
		}
		cout << "Конечный результат: " << endl;
		for_each(g.begin(), g.end(), [](vector<int>& ivec)
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

	return 0;
}