#include <Windows.h>
#include "Facade.h"

using fs::directory_iterator;

int main()
{
	setlocale(LC_ALL, "Russian");
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	string directory;
	vector<path> files;
	
	// get files
	cout << "Введите директорию, где содержатся файлы для текстового анализа (.txt): " << endl;
	getline(cin, directory);
	if (!fs::exists(directory)) {
		cout << "Необходимо ввести существующий пусть";
		getline(cin, directory);
		return -1;
	}

	for (const auto& file : directory_iterator(directory))
		files.push_back(file.path());

	// get setup
	string temp;
	bool ignore_case;
	bool ignore_special_characters;
	bool ignore_numbers;
	bool ignore_another_language;

	bool short_statistics;
	bool get_dictionary;

	unsigned int count = 0;

	cout << "Игнорировать регистр? (1-да, иное-нет) : ";
	getline(cin, temp);
	ignore_case = (temp == "1" ? true : false);
	cout << "Игнорировать специальные символы? (1-да, иное-нет) : ";
	getline(cin, temp);
	ignore_special_characters = (temp == "1" ? true : false);
	cout << "Игнорировать цифры/числа? (1-да, иное-нет) : ";
	getline(cin, temp);
	ignore_numbers = (temp == "1" ? true : false);
	cout << "Игнорировать другие языки кроме русского? (1-да, иное-нет) : ";
	getline(cin, temp);
	ignore_another_language = (temp == "1" ? true : false);
	cout << "Желаете видеть только краткую статистику? (1-да, иное-нет) : ";
	getline(cin, temp);
	short_statistics = (temp == "1" ? true : false);
	cout << "Желаете при выводе в файл выводить полный словарь (слово-статистика)? (1-да, иное-нет) : ";
	getline(cin, temp);
	get_dictionary = (temp == "1" ? true : false);
	cout << "Для короткой статистики какое количество слов Вы хотите видеть (максимум, целое число > 0)? : ";
	getline(cin, temp);
	try {
		count = (int)stoi(temp.c_str());
	}
	catch (exception e) {
		cout << "Необходимо ввести науральное число";
		getline(cin, directory);
		return -1;
	}

	cout << endl;
	// do
	Facade facade = Facade(files, count, ignore_case, ignore_special_characters, ignore_numbers, ignore_another_language, short_statistics, get_dictionary);
	if (facade.show_statistic() == -1) {
		cout << "По данной директории нет подходящих файлов";
		getline(cin, directory);
		return -1;
	}

	cout << endl;
	// to file
	cout << "Желаете сделать вывод в файл? (1-да, иное-нет) : ";
	getline(cin, temp);
	if (temp == "1") {
		cout << "Введите путь к файлу (без названия файла). Осторожно, если такой файл существует, то он будет перезаписан! : ";
		getline(cin, directory);
		if (!filesystem::exists(directory)) {
			cout << "Необходимо ввести существующий пусть";
			getline(cin, directory);
			return -1;
		}
		string f;
		cout << "Введите название файла без расширения: ";
		getline(cin, f);
		directory += "\\";
		directory += f;
		directory += ".txt";
		cout << "Введите заголовок внутри файла: ";
		getline(cin, f);
		facade.write_down_statistic(directory, f);
	}

	cout << "Спасибо за участие!" << endl;
	getline(cin, directory);
	return 0;
}