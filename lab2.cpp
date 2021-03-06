/*16. Имеется  некоторое  алгебраическое выражение.Операнды
заданы идентификаторами, операции выполняются по приоритетам,
допускается использование круглых скобок.Требуется построить
и выдать в  наглядном  виде  бинарное  дерево, представляющее
данное выражение(13).*/

#include "pch.h"
#include <windows.h>
#include <iostream> //поток ввода-вывода (Input/Output Stream)
#include <fstream> //для чтения и записи в файл
#include <iomanip> //инструменты для работы с форматированием вывода
#include <cstdlib> /*функции динамического управления памятью, 
генерации случайных чисел, целой арифметики, поиска, сортировки и преобразования типов данных*/
#include <vector>
using namespace std;
ofstream output;

struct Tree
{
	char partExpression = 0;
	int center;
	Tree *left, *right, *parent;
	int urov = 0;
};

void del(Tree *&p) {
	if (p != NULL)                //Пока не встретится пустое звено
	{
		del(p->left);                //Рекурсивная функция прохода по левому поддереву
		del(p->right);                //Рекурсивная функци для прохода по правому поддереву
		delete p;                 
		p = NULL;                
	}
};

int oddBracket(char str1[], int start, int end)
{
	int bracket = 0, outerBrackets = 0, innerBrackets = 0, brackets = 0;
	bool closeBracket = false;

	for (int i = start; i <= end; i++)
	{
		if ((str1[i] == '(') && (str1[end - i + start] == ')')) {
			if (str1[i + 1] != '(') {
				outerBrackets++;
				break;
			}
			else {
				outerBrackets++;
			}
		}
	}
	if (outerBrackets != 0) {
		for (int j = start + outerBrackets; j <= (end - outerBrackets); j++)
		{
			if (str1[j] == '(') {
				innerBrackets++;
			}
			else if ((str1[j] == ')') && (innerBrackets != 0)) {
				innerBrackets--;
			}
			else if ((str1[j] == ')') && (innerBrackets == 0)) {
				outerBrackets--;
			}
		}
	}
	
	return outerBrackets;
}

pair<char, int> pair1;

pair<char, int> expressionDivision(char str2[], int start, int end)
{
	int zeroLevel = 0, centerEquation = 0;
	bool znak = false;

	for (int k = end; k >= start; k--)
	{
		if (str2[k] == ')') {
			zeroLevel++;
		}
		else if (str2[k] == '(') {
			zeroLevel--;
		}
		else if ((zeroLevel == 0) && ((str2[k] == '+') || (str2[k] == '-'))) {
			centerEquation = k;
			znak = true;
			break;
		}
	}
	if (!znak) {
		for (int k = end; k >= start; k--)
		{
			if (str2[k] == ')') {
				zeroLevel++;
			}
			else if (str2[k] == '(') {
				zeroLevel--;
			}
			else if ((zeroLevel == 0) && ((str2[k] == '/') || (str2[k] == '*'))) {
				centerEquation = k;
				znak = true;
				break;
			}
		}
	}

	if ((end == start) || (!znak)) {
		pair1.first = str2[start];
		pair1.second = 0;
	} 
	else if (zeroLevel == 0) {
		pair1.first = str2[centerEquation];
		pair1.second = centerEquation;
	} 

	return pair1;
}

pair<char*, char*> pair2;

pair<char*, char*> twoStr(int center, char str2[])
{
	char *p1;
	char *p2;

	char strLeft[255];
	for (int i = 0; i < center; i++) {
		for (int j = 0; j < strlen(strLeft); j++) {
			strLeft[j] = str2[i];
		}
	}
	p1 = strLeft;

	char strRight[255];
	for (int i = center + 1; i <= strlen(str2); i++) {
		for (int j = 0; j < strlen(strRight); j++) {
			strRight[j] = str2[i];
		}
	}
	p2 = strRight;

	pair2.first = p1;
	pair2.second = p2;

	return pair2;
}

void createElementTree(char str2[], Tree *&parent, int start, int end, int urov)
{
	Tree *curr;
	int kolvoBracket = 0, center;

	if (parent == NULL)
	{
		parent = new Tree;
		kolvoBracket = oddBracket(str2, start, end);
		expressionDivision(str2, start + kolvoBracket, end - kolvoBracket);
		curr = parent;
		curr->left = NULL;
		curr->right = NULL;
		curr->partExpression = pair1.first;
		curr->urov = urov;
		urov++;
		center = pair1.second;

		if (center > 0)
		{
			createElementTree(str2, curr->left, start + kolvoBracket, center - 1, urov);
			createElementTree(str2, curr->right, center + 1, end - kolvoBracket, urov);
		}
		else if (center = 0) {
			curr->urov = urov;
			urov++;
			createElementTree(str2, curr->left, center, center, urov);
			createElementTree(str2, curr->right, center, center, urov);
			curr->partExpression = pair1.first;
		}
	}
}

void printTree(Tree *p, ofstream &output)
{
	if (p != NULL)
	{
		for (int i = 1; i <= p->urov; i++) {
			output << ".";
		}
		output << p->partExpression << endl;
		printTree(p->left, output);
		printTree(p->right, output);
	}
}

int main(int argc, char * argv[])
{
	/*чтобы распознавался русский язык в консоли и в файле*/
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	string str;
	char str1[255];
	char str2[255];

	//vector<char> v;

	char ch = 0;
	int bracket = 0, lenStr = 0, outerBrackets = 0, innerBrackets = 0;

	if (argc != 3) {
		cout << "Invalid arguments count\n"
			<< "Usage: copyfile.exe <input file> <output file>\n";
		return 1;
	}

	/*Объявили переменную типа ifstream (input file stream, поток для чтения из файла),
	проинициализировав его именем входного файла*/

	ifstream input(argv[1]);

	// вызываем метод is_open() у объекта input, который вернет true, если файл был открыт
	if (!input.is_open()) {
		cout << "Failed to open " << argv[1] << " for reading\n";
		return 1;
	}

	// создали поток для записи в выходной файл
	ofstream output(argv[2]);
	if (!output.is_open()) {
		output << "Failed to open " << argv[2] << " for writing\n";
		return 1;
	}

	while (!input.eof())
	{
		input >> str1;
		lenStr = strlen(str1);
		output << str1 << endl;

		if (lenStr == 0) {
			output << "Error, файл пустой";
		}

		for (int i = 0; i < lenStr; i++)
		{
			if (str1[i] == '(') {
				bracket++;
			} 
			else if (str1[i] == ')') {
				bracket--;
			}
		};

		if (bracket < 0) {
			output << "Не хватает (" << endl;
		} 
		else if (bracket > 0) {
			output << "Не хватает )" << endl;
		}
	};

	if (bracket == 0) {
		Tree *parent = NULL;
		int start = 0;
		int end = strlen(str1) - 1;
		int urov = 0;
		createElementTree(str1, parent, start, end, urov);
		printTree(parent, output);
	}

	input.close(); // закрытие потока
	while (input.get(ch))
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