//21. В   некотором   компиляторе   ПАСКАЛя  текст  программы
//включает примечания, выделенные  фигурными  скобками  '{', '}'
//либо  парами  символов  '(*'  и  '*)'.Примечания  могут быть
//вложенными друг в друга.Если примечание открыто знаком  '{',
//то оно должно быть закрыто знаком '}'.Аналогично примечание,
//начинающееся с символов '(*'  должно  заканчиваться  символами
//'*)'.Требуется:
//1) проверить правильность вложенности примечаний;
//2) переписать   файл   с   исходным   текстом   так, чтобы
//отсутствовала  вложенность  комментариев  при  сохранении   их
//содержания  и  в  качестве  ограничивающих  символов  остались
//только  фигурные  скобки.Учесть   случай, когда   символы
//примечаний находятся в апострофах(10).

#include "pch.h"
#include <windows.h>
#include <iostream> //поток ввода-вывода (Input/Output Stream)
#include <fstream> //для чтения и записи в файл
#include <iomanip> //инструменты для работы с форматированием вывода
#include <cstdlib> /*функции динамического управления памятью, 
генерации случайных чисел, целой арифметики, поиска, сортировки и преобразования типов данных*/
#include <vector>
using namespace std;

struct bracket
{
	char ch;
	int stroka;
	int simbol;
};

struct stek
{
	bracket brkt;
	struct stek *next; // указатель на следующий элемент списка (стека)
};

void push(stek *&NEXT, const bracket VALUE)
{
	stek *myStack = new stek; // объявляем новую динамическую переменную типа stek
	myStack->brkt = VALUE;   // записываем значение, которое помещается в стек
	myStack->next = NEXT;     // связываем новый элемент стека с предыдущим
	NEXT = myStack;           // новый элемент стека становится его вершиной
}

bracket pop(stek *&NEXT)
{
	bracket temp = NEXT->brkt; // извлекаем в переменную temp значение в вершине стека
	stek *myStack = NEXT;   /* запоминаем указатель на вершину стека, чтобы затем
							 освободить выделенную под него память*/
	NEXT = NEXT->next;      // вершиной становится предшествующий top элемент
	delete myStack;         // освобождаем память, тем самым удалили вершину
	return temp;            // возвращаем значение, которое было в вершине
}

int main(int argc, char * argv[])
{
	/*чтобы распознавался русский язык в консоли и в файле*/
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	stek *top = 0;

	bracket brkt;

	bool isNotComment = true;
	bool isNotError = true;

	vector<char> v;
	char ch = 0, ch0 = 0;
	int n = 0, stroka = 1, simbol = 0, simbolCh = 1;

	if (argc != 3)
	{
		cout << "Invalid arguments count\n"
			<< "Usage: copyfile.exe <input file> <output file>\n";
		return 1;
	}

	/*Объявили переменную типа ifstream (input file stream, поток для чтения из файла), 
	проинициализировав его именем входного файла*/

	ifstream input(argv[1]);

	// вызываем метод is_open() у объекта input, который вернет true, если файл был открыт
	if (!input.is_open())
	{
		cout << "Failed to open " << argv[1] << " for reading\n";
		return 1;
	}

	// создали поток для записи в выходной файл
	ofstream output(argv[2]);
	if (!output.is_open())
	{
		output << "Failed to open " << argv[2] << " for writing\n";
		return 1;
	}

	// Считываем в переменную ch символ из входного потока
	if (!input.eof())
	{
		input >> ch0;
		if (input.eof())
		{
			output << "Error, файл пустой";
			isNotError = false;
		}
		else
		{
			ch = ch0;
			simbol++;
		}
	};

	while (!input.eof())
	{
		if (ch == '\n')
		{
			stroka++;
			simbol = 0;
			output << endl;
		}
		else if ((ch == '{') && isNotComment)
		{
			if (top == 0)
			{
				output << ch;
			}
			simbol++;
			brkt.ch = ch;
			brkt.simbol = simbol;
			brkt.stroka = stroka;
			push(top, brkt);
			n++;
		}
		else if ((ch == '\"') && isNotComment)
		{
			output << ch;
			isNotComment = false;
			simbol++;
		}
		else if ((ch == '\"') && !isNotComment)
		{
			output << ch;
			isNotComment = true;
			simbol++;
		}
		else if (!isNotComment)
		{
			output << ch;
			simbol++;
		}
		else if ((ch == '}') && isNotComment)
		{
			if (top == 0)
			{
				output << endl;
				simbol++;
				output << "Error, чтение }, стек пустой. Ошибка в строке = " << stroka << " символ № " << simbol << endl;
				isNotError = false;
				input.close();
			}
			else if (top->brkt.ch == '{')
			{
				simbol++;
				pop(top);
				n--;
				if (top == 0)
				{
					output << ch;
				}
			}
			else if (top->brkt.ch == '*')
			{
				simbol++;
				output << ch;
				output << endl;
				output << "Error, чтение }, в стеке (*. Ошибка в строке = " << stroka << " символ № " << simbol << endl;
				isNotError = false;
				input.close();
			}
		}
		else if ((ch == '(') && isNotComment)
		{
			input >> ch;
			if ((ch == '*') && (top != 0))
			{
				simbol++;
				simbol++;
				brkt.ch = ch;
				brkt.simbol = simbol;
				brkt.stroka = stroka;
				push(top, brkt);
			}
			else if ((ch == '*') && (top == 0))
			{
				simbol++;
				simbol++;
				brkt.ch = ch;
				brkt.simbol = simbol;
				brkt.stroka = stroka;
				push(top, brkt);
				output << '{';
			}
			else
			{
				output << "(";
				output << ch;
				simbol++;
				simbol++;
			}
		}
		else if ((ch == '*') && isNotComment)
		{
			input >> ch;
			if ((ch == ')') && (top == 0))
			{
				simbol++;
				simbol++;
				output << endl;
				output << "Error, чтение *) в стеке пусто. Нет (*. Ошибка в строке = " << stroka << " символ № " << simbol << endl;
				isNotError = false;
				input.close();
			}
			else if ((ch == ')') && (top->brkt.ch == '*'))
			{
				simbol++;
				simbol++;
				pop(top);

				if (top == 0)
				{
					output << '}';
				}

			}
			else if ((ch == ')') && (top->brkt.ch == '{'))
			{
				simbol++;
				simbol++;
				output << endl;
				output << "Error, чтение *) в стеке {. Ошибка в строке = " << stroka << " символ № " << simbol << endl;
				isNotError = false;
				input.close();
			}
			else
			{
				output << "*";
				output << ch;
				simbol++;
				simbol++;
			}
		}
		else if (input.eof())
		{
			break;
		}
		else if (isNotComment)
		{
			//вывод значения переменной на экран
			output << ch;
			simbol++;
		}
		else if (!isNotComment)
		{
			simbol++;
			output << endl;
			output << "Error, Отсутствует "". Ошибка в строке = " << stroka << " символ № " << simbol << endl;
			isNotError = false;
			input.close();
		}

		ch = input.get();
	};

	output << endl; //перевод строки

	if ((isNotError) && (top != 0))
	{
		simbol++;
		output << endl;
		simbol = top->brkt.simbol;
		stroka = top->brkt.stroka;
		output << "Error, Количество открыв и закрыв скобок разное. Ошибка в строке = " << stroka << " символ № " << simbol << endl;
		input.close();
	};

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