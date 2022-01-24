#pragma once
#include "Notebook.h"
#include "Filter.h"
#include <time.h>
#include <fstream>
#include <ctime>
#include <iostream>
#include <algorithm>

using namespace std;

// класс для более простой работы с Notebook
class NbHandler {
private:
	Notebook* nb; // по ссылке, чтобы избежать копирования
	const string delimiter = " | "; // разделитель между данными для записи и чтения из файла
public:
	NbHandler(Notebook* nb) : nb(nb) { };

	void display_notebook() {
		display_notes((*nb->get_notes()));
	}
	
	static void display_notes(vector<Note> notes) {
		int i = 1;
		if (notes.size() == 0) {
			cout << "Записи отсутствуют" << endl;
			return;
		}
		cout << endl << "---" << endl;
		cout << "№\t" << "creation datetime" << "\t" << "change datetime" << "\t\t" << "author" << "\t\t" << "tags" << endl;
		for_each((notes).begin(), (notes).end(), [&](Note n) {
			cout << i << "\t";
			cout << time_t_to_string(n.get_creation_date()) << "\t";
			cout << time_t_to_string(n.get_change_date()) << "\t";
			cout << n.get_author() << "\t";
			cout << n.get_tags_to_string() << endl;
			i++;
			});
		cout << "---" << endl;
	}

	// вывести содержание одной записи. index >= 0
	void display_note(unsigned int index) {
		Note note = (*(*nb).get_notes())[index];
		cout << note.get_note() << endl;
	}

	// поиск
	void display_notebook_with_search(string filter_by_author = "", vector<string> filter_by_tags = vector<string>()) {
		const vector<Note>* notes = (*nb).get_notes();
		int i = 1;

		cout << endl << "---" << endl;
		cout << "№\t" << "creation datetime" << "\t" << "change datetime" << "\t\t" << "author" << "\t\t" << "tags" << endl;
		for_each((*notes).begin(), (*notes).end(), [&](Note n) {
			vector<string> tags = n.get_tags();
			bool fit = true; // запись подходит

			// поиск по тегам
			if (filter_by_tags.size() != 0) {
				for_each(filter_by_tags.begin(), filter_by_tags.end(), [&](string& s) {
					if (find(tags.begin(), tags.end(), s) == tags.end()) // поиск тега среди тегов записи
					{
						fit = false;
					}
					});
			}

			// поиск по автору
			if (filter_by_author != "" && filter_by_author != n.get_author()) {
				fit = false;
			}

			if (fit) {
				// если запись подходит - вывести
				cout << i << "\t";
				cout << time_t_to_string(n.get_creation_date()) << "\t";
				cout << time_t_to_string(n.get_change_date()) << "\t";
				cout << n.get_author() << "\t";
				cout << n.get_tags_to_string() << endl;
			}
			i++;
			});
		cout << "---" << endl;
	}

	// запись в файл
	bool write_notes(string path, string file = "my_notes")
	{
		string new_file = path + "\\" + file + ".notes";
		ofstream f;
		try {
			f.open(new_file);
			const vector<Note>* notes = (*nb).get_notes();

			// запись в файл каждой записи построчно
			for_each((*notes).begin(), (*notes).end(), [&](Note n) {
				f << n.get_creation_date() << delimiter;
				f << n.get_change_date() << delimiter;
				f << n.get_author() << delimiter;
				vector<string> tags = n.get_tags();
				for (auto const& el : tags) f << el + "#";
				f << delimiter;
				f << n.get_note() << endl;
				});

			f.close();
			return true;
		}
		catch (exception ex) {
			return false;
		}
	}

	// чтение из файла
	bool read_notes(string file)
	{
		string choice;
		cout << "Хотите перезаписать записную книжку или добавить записи к существующим? (п-перезаписать, иное-добавить)";
		cin >> choice;
		if (choice[0] == 'п') {
			nb->set_null(); // обнуление данных при перезаписи
		}

		const vector<Note>* notes = (*nb).get_notes(); // предварительное чтение уже существующих записей

		try {
			ifstream input(file);
			for (string line; getline(input, line); )
			{
				// раздление по разделителю
				vector<string> elements = split(line, " | ");
				vector<string> tags = split(elements[3], "#"); // разделение тегов
				tags.pop_back();
				// оставшиеся части - сама запись (если внутри записи оказался разделитель)
				string note;
				for_each(elements.begin() + 4, elements.end(), [&](const string& piece) { note += piece; });

				nb->add_note(note, elements[2], stoll(elements[0]), stoll(elements[1]), tags);
			}

			input.close();
			return true;
		}
		catch (exception ex) {
			return false;
		}
	}

	// разделение строки пример1#пример2#пример3# на подстроки
	static vector<string> split(string str, string token) {
		vector<string>result;
		while (str.size()) {
			int index = str.find(token);
			if (index != string::npos) {
				result.push_back(str.substr(0, index));
				str = str.substr(index + token.size());
				if (str.size() == 0) result.push_back(str);
			}
			else {
				result.push_back(str);
				str = "";
			}
		}
		return result;
	}

	// конвертация даты в формат для вывода
	static string time_t_to_string(time_t datetime) {
		char str[100];
		tm ltm;
		localtime_s(&ltm, &datetime);
		strftime(str, 100, "%d/%m/%Y %T", &ltm);
		return str;
	}
};