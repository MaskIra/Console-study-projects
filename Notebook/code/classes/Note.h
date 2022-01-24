#pragma once
#include <vector>
#include <string>
#include <time.h>

using namespace std;

class Note {
private:
	string note;
	string author;
	time_t creation_date;
	time_t change_date;
	vector<string> tags;
public:
	// новая запись (без дат)
	Note(string note, string author, vector<string> tags = vector<string>()) :
		note(note), author(author), tags(tags) {
		creation_date = time(0);
		change_date = creation_date;
	};
	// добавление уже созданной ранее записи (с датами)
	Note(string note, string author, vector<string> tags, time_t creation_date, time_t change_date) :
		note(note), author(author), tags(tags), creation_date(creation_date), change_date(change_date) { };

	string get_note() { return note; }
	string get_author() { return author; }
	time_t get_creation_date() { return creation_date; }
	time_t get_change_date() { return change_date; }
	vector<string> get_tags() {	return tags; }
	string get_tags_to_string() {
		string s;
		for (auto const& el : tags) s += "#" + el + " ";
		return s;
	}

	// изменение записи
	void modification(string note, string author, vector<string> tags = vector<string>())
	{
		this->note = note;
		this->author = author;
		this->tags = tags;
		change_date = time(0);
	}
};