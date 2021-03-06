//В файле имеется телефонный справочник, включающий имена
//владельцев телефонов.Организовать быстрый поиск  по  номерам
//телефонов   с   помощью   хеширования.Обеспечить  дополнение
//справочника(10). Белякова Мария

#include "pch.h"
#include <string>
#include <iterator>
#include <algorithm>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <string.h>
#include <cstddef>
#include <windows.h>
#include <ctype.h>
#include <stdio.h>
#include <iostream> //поток ввода-вывода (Input/Output Stream)
#include <fstream> //для чтения и записи в файл
#include <iomanip> //инструменты для работы с форматированием вывода
#include <cstdlib> //функции динамического управления памятью
#include <cstring>
using namespace std;
//int M = 101;
int M;

struct notes {
	char phone[20];
	char name[30];
	bool isStr;
};
int structWeight = 51;

bool DirectoryExists(char fileName[260])
{
	int Code = GetFileAttributesA(fileName);
	return (Code != -1) && (FILE_ATTRIBUTE_DIRECTORY & Code);
}

int HashFun0(int key, int M)
{
	return (key % M);
}

int HashFunction(int i, int h0, int M)
{
	return ((h0 + i * i) % M);
}

bool isPrimeNumber(int M)
{
	bool isPrime = true;
	for (int i = 2; i < sqrt(M); i++) {
		if (M % i == 0) {
			isPrime = false;
		}
	}

	return isPrime;
}

bool addStruct(notes &NoteTmp, notes &phoneAndName, string str, string fileName, int M)
{
	int key0, h0, numHash, newHash;
	key0 = atoi(phoneAndName.phone);
	h0 = HashFun0(key0, M) * structWeight;
	fstream ofile;
	ofile.open(fileName + ".bin", ios::in | ios::out | ios::ate | ios::binary);
	bool keyIsfound = false;
	numHash = 0;
	if (ofile)
	{
		ofile.seekg(h0, ios::beg);
		ofile.read((char*)&NoteTmp, sizeof(notes));
		if (NoteTmp.isStr != true) {
			ofile.seekp(h0, ios::beg);
			ofile.write((char*)&phoneAndName, sizeof(notes));
			phoneAndName = { 0 };
			str.clear();
			NoteTmp = { 0 };
			ofile.close();
			return true;
		} else if (NoteTmp.isStr == true) {
			while (!keyIsfound) {
				numHash++;
				newHash = HashFunction(numHash, h0, M) * structWeight;
				ofile.seekg(abs(newHash), ios::beg);
				ofile.read((char*)&NoteTmp, sizeof(notes));
				if (NoteTmp.isStr != true) {
					ofile.seekp(abs(newHash), ios::beg);
					ofile.write((char*)&phoneAndName, sizeof(notes));
					phoneAndName = { 0 };
					str.clear();
					keyIsfound = true;
					NoteTmp = { 0 };
					return true;
				}
				if (numHash == M) {
					cout << "Не нашлось пустого места для записи телефона" << endl;
					return false;
				}
			}
			ofile.close();
		}
	}
}

int main(int argc, char * argv[])
{
	/*чтобы распознавался русский язык в консоли и в файле*/
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_CTYPE, "rus");// Поддержка кириллицы в консоли Windows

	string str, fileName, fileName2, phoneName, name;
	char phoneName1[20] = {0};
	notes phoneAndName = { 0 }, NoteTmp;
	vector<string> phonebooks;
	char ch = 0;
	int numActions = 0, numPhonebook = 0, h0 = 0, numHash, key = 0, key0 = 0, newHash = 0;
	ifstream file;
	bool keyIsfound = false, firstRecord = false;

	cout << "Выберите дальнейшее действие (наберите номер):" << endl;
	cout << "1 Новый справочник" << endl;
	cout << "2 Открыть справочник" << endl;
	cout << "0 Выход" << endl;
	cin >> numActions;

	while ((numActions < 0) && (numActions > 2)) {
		cout << "Выберите дальнейшее действие (наберите номер):" << endl;
		cout << "1 Новый справочник" << endl;
		cout << "2 Открыть справочник" << endl;
		cout << "0 Выход" << endl;
		cin >> numActions;
	}

	while (numActions != 0) {
		if (numActions == 1) {
			cout << "Введите название текстого файла с которого нужно сосчитать данные:" << endl;
			cin >> fileName2;
			file.open("C:\\Users\\Manya\\source\\repos\\lab4\\Debug\\" + fileName2 + ".txt");
			if (!file.is_open()) {
				cout << "Файл не открылся";
				return 1;
			};
			int countLine = 0;
			if (file) {
				while (!file.eof())
				{
					getline(file, str);
					countLine++;
				}
			}
			file.close();
			cout << "countLine = " << countLine << endl;
			int x = 1;
			bool isPrime = false;
			M = countLine * 2 + x;
			while (!isPrime) {
				x++;
				M = countLine * 2 + x;
				isPrime = isPrimeNumber(M);
			}
			cout << "M = " << M << endl;
			
			file.open("C:\\Users\\Manya\\source\\repos\\lab4\\Debug\\" + fileName2 + ".txt");
			bool nameFileNew = false;
			cout << "Введите название для справочника(не более 10 символов без пробелов):" << endl;
			cin >> fileName;
			while (!nameFileNew) {
				ifstream filePhonebooks("phonebooks.txt");
				int strNum = 0;
				string temp;
				bool spravIs = false;
				if (filePhonebooks.is_open())
				{
					while (!filePhonebooks.eof())
					{
						filePhonebooks >> temp;
						if (temp == fileName)
						{
							cout << "Такой справочник уже есть!" << endl;
							spravIs = true;
							cout << "Введите название справочника(не более 10 символов без пробелов):" << endl;
							cin >> fileName;
						}
						temp.clear();
					}
					if (filePhonebooks.eof() && (!spravIs)) {
						break;
					}
				};
				filePhonebooks.close();
			}
			fstream  ofile(fileName + ".bin", ios::in | ios::out | ios::ate | ios::binary);
			notes emptyStruct = { "", "", false };
			ofile.open(fileName + ".bin", ios::out | ios::ate | ios::binary);
			int r = 0;
			while (r < M) {
				ofile.write((char*)&emptyStruct, sizeof(notes));
				r++;
			};
			ofile.close();
			ofstream filePhonebooks("phonebooks.txt", ios::app);
			if (filePhonebooks.is_open())
			{
				filePhonebooks << fileName << endl;
			}
			filePhonebooks.close();

			int probel = 0, j;
			while (getline(file, str))
			{
				j = 0;
				int strLen = str.length();
				for (int i = 0; i < strLen; i++) {
					if ((str[i] != ' ') && (i < 21) && (isdigit(str[i]))) {
						phoneAndName.phone[j] = str[i];
						j++;
					}
					else if ((str[0] == '+') && (i == 0)) {
						phoneAndName.phone[j] = str[i];
						j++;
					}
					else if (str[i] == ' ') {
						probel = i;
						break;
					}
					else if ((i >= 21) && (probel == 0)) {
						continue;
					}
				};
				int k = 0;
				for (int i = probel + 1; i < strLen; i++) {
					if (i < 31) {
						phoneAndName.name[k] = str[i];
						k++;
					}
					else {
						break;
					}
				};
				phoneAndName.isStr = true;
				ofile.open(fileName + ".bin", ios::in | ios::out | ios::ate | ios::binary);
				addStruct(NoteTmp, phoneAndName, str, fileName, M);
			}
			if (file) {
				file.close();
			}
			if (ofile) {
				ofile.close();
			}
		}
		else if (numActions == 2) {
			bool trueVar = false;
			string line;
			fstream file;
			int numberPhoneBook = 1, numPhonebook = 0;
			ifstream filePhonebooks("phonebooks.txt");
			if (filePhonebooks.is_open())
			{
				long file_size;
				filePhonebooks.seekg(0, ios::end);
				file_size = filePhonebooks.tellg();
				if (file_size == 0) {
					cout << "Нет еще телефонных справочников" << endl;
					filePhonebooks.close();
				}
				else {
					cout << "Выберите нужный справочник:" << endl;
					filePhonebooks.seekg(0);
					while (getline(filePhonebooks, line))
					{
						cout << numberPhoneBook << " - " << line << endl;
						numberPhoneBook++;
						phonebooks.push_back(line);
					}
					filePhonebooks.close();
					cin >> numPhonebook;
					if ((numPhonebook > numberPhoneBook) || (numPhonebook < 1)) {
						cout << "Вы ввели не верный вариант!" << endl;
						cout << "Выберите нужный справочник:" << endl;
						for (int i = 0; i < phonebooks.size(); i++) {
							cout << i + 1 << ") " << phonebooks[i] << endl;
						}
						cin >> numPhonebook;
						while ((numPhonebook < 1) && (numPhonebook > phonebooks.size())) {
							cout << "Вы ввели не верный вариант!" << endl;
							cout << "Выберите нужный справочник:" << endl;
							for (int i = 0; i < phonebooks.size(); i++) {
								cout << i + 1 << ") " << phonebooks[i] << endl;
							}
							cin >> numPhonebook;
						};
					};
					file.open(phonebooks[numPhonebook - 1] + ".bin", ios::ate | ios::in | ios::out | ios::binary);
					M = 0;
					if (file) {
						file.seekg(0, ios::end); // если курсор в начале файла, перемещаем курсор в конец файла.
						int sizef = file.tellg(); // функция выдаст конечное положнние курсора относительно начала файла в байтах.
						file.seekg(0, ios::beg); // возвращаем курсор в начало файла.
						M = sizef / structWeight;
					}
					cout << "M = " << M << endl;
					file.close();
					file.open(phonebooks[numPhonebook - 1] + ".bin", ios::ate | ios::in | ios::out | ios::binary);
					if (file) {
						cout << "Выберите дальнейшее действие (наберите номер):" << endl;
						cout << "1 Справка" << endl;
						cout << "2 Дополнить" << endl;
						cout << "3 Удалить номер" << endl;
						cout << "0 Cохранить изменения и вернуться на уровень назад" << endl;
						cin >> numActions;

						while ((numActions < 0) && (numActions > 3)) {
							cout << "Выберите дальнейшее действие (наберите номер):" << endl;
							cout << "1 Справка" << endl;
							cout << "2 Дополнить" << endl;
							cout << "3 Удалить номер" << endl;
							cout << "0 Cохранить изменения и вернуться на уровень назад" << endl;
							cin >> numActions;
						}

						while (numActions != 0) {
							if (numActions == 1) {
								NoteTmp = { 0 };
								cout << "Введите номер телефона: " << endl;
								cin >> phoneName;
								int j = 0;
								for (int i = 0; i < phoneName.length(); i++) {
									if ((isdigit(phoneName[i])) && (j < 20)) {
										phoneName1[j] = phoneName[i];
										j++;
									}
								};
								key0 = atoi(phoneName1);
								h0 = HashFun0(key0, M) * structWeight;
								file.seekg(h0);
								file.read((char*)&NoteTmp, sizeof(notes));
								if ((NoteTmp.isStr == true) && (atoi(NoteTmp.phone) == atoi(phoneName1))) {
									cout << "ФИО: " << NoteTmp.name << endl;
								}
								else {
									numHash = 0;
									keyIsfound = false;
									while (!keyIsfound) {
										if (numHash < M / 2) {
											numHash++;
											newHash = HashFunction(numHash, h0, M) * structWeight;
											file.seekg(abs(newHash), ios::beg);
											file.read((char*)&NoteTmp, sizeof(notes));
											if ((NoteTmp.isStr == true) && (atoi(NoteTmp.phone) == atoi(phoneName1))) {
												cout << "ФИО: " << NoteTmp.name << endl;
												keyIsfound = true;
												break;
											}
											NoteTmp = { 0 };
										}
										else {
											cout << "Нет такого номера телефона." << endl;
											break;
										}

									}
								}
							}
							else if (numActions == 2) {
								NoteTmp = { 0 };
								char name[30];
								cout << "Введите номер телефона:" << endl;
								cin >> phoneName;
								int j = 0;
								for (int i = 0; i < phoneName.length(); i++) {
									if ((isdigit(phoneName[i])) && (j < 20)) {
										phoneAndName.phone[j] = phoneName[i];
										j++;
									}
									else if ((phoneName[0] == '+') && (i == 0)) {
										phoneAndName.phone[j] = phoneName[i];
										j++;
									}
									else if (i >= 21) {
										break;
									}
								};
								cout << "Введите ФИО (не более 30 символов):" << endl;
								cin.get();
								cin.getline(name, 30);
								for (int i = 0; i < strlen(name); i++) {
									if (i < 30) {
										phoneAndName.name[i] = name[i];
									}
									else {
										break;
									}
								};
								
								phoneAndName.isStr = true;
								if (addStruct(NoteTmp, phoneAndName, str, phonebooks[numPhonebook - 1], M)) {
									cout << "В телефонный справочник добавлен." << endl;
								}
								else {
									break;
								};
								
							}
							else if (numActions == 3) {
								NoteTmp = { 0 };
								cout << "Введите номер телефона, который нужно удалить из телефонного справочника:" << endl;
								cin >> phoneName;
								int j = 0;
								for (int i = 0; i < phoneName.length(); i++) {
									if ((isdigit(phoneName[i])) && (j < 20)) {
										phoneName1[j] = phoneName[i];
										j++;
									}
								};
								key0 = atoi(phoneName1);
								h0 = HashFun0(key0, M) * structWeight;
								file.seekg(h0);
								file.read((char*)&NoteTmp, sizeof(notes));
								if ((NoteTmp.isStr == true) && (atoi(NoteTmp.phone) == atoi(phoneName1))) {
									NoteTmp = { "", "", false };
									file.seekp(h0, ios::beg);
									file.write((char*)&NoteTmp, sizeof(notes));
									cout << "Номер удален." << endl;
								}
								else {
									numHash = 0;
									keyIsfound = false;
									while (!keyIsfound) {
										if (numHash < M / 2) {
											numHash++;
											newHash = HashFunction(numHash, h0, M) * structWeight;
											file.seekg(abs(newHash), ios::beg);
											file.read((char*)&NoteTmp, sizeof(notes));
											if ((NoteTmp.isStr == true) && (atoi(NoteTmp.phone) == atoi(phoneName1))) {
												NoteTmp = { "", "", false };
												file.seekp(abs(newHash), ios::beg);
												file.write((char*)&NoteTmp, sizeof(notes));
												cout << "Номер удален." << endl;
												keyIsfound = true;
												break;
											}
											NoteTmp = { 0 };
										}
										else {
											cout << "Нет такого номера телефона." << endl;
											break;
										}

									}
								}
							}
							else if (numActions == 0) {
								if (file) {
									file.close();
								};
								break;
							}
							cout << endl;
							cout << "Выберите дальнейшее действие (наберите номер):" << endl;
							cout << "1 Справка" << endl;
							cout << "2 Дополнить" << endl;
							cout << "3 Удалить номер" << endl;
							cout << "0 Cохранить изменения и вернуться на уровень назад" << endl;
							cin >> numActions;

							while ((numActions < 0) && (numActions > 3)) {
								cout << endl;
								cout << "Выберите дальнейшее действие (наберите номер):" << endl;
								cout << "1 Справка" << endl;
								cout << "2 Дополнить" << endl;
								cout << "3 Удалить номер" << endl;
								cout << "0 Cохранить изменения и вернуться на уровень назад" << endl;
								cin >> numActions;
							}
						}
						
						if (file) {
							file.close();
						}
					}
					if (file) {
						file.close();
					}
				}
			}
		};
		if (file) {
			file.close();
		}
		cout << endl;
		cout << "Выберите дальнейшее действие (наберите номер):" << endl;
		cout << "1 Новый справочник" << endl;
		cout << "2 Открыть справочник" << endl;
		cout << "0 Выход" << endl;
		cin >> numActions;

		while ((numActions < 0) && (numActions > 2)) {
			cout << endl;
			cout << endl;
			cout << "Выберите дальнейшее действие (наберите номер):" << endl;
			cout << "1 Новый справочник" << endl;
			cout << "2 Открыть справочник" << endl;
			cout << "0 Выход" << endl;
			cin >> numActions;
		}
	};

	return 0;
};