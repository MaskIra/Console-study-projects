#pragma once
#include "Note.h"

class Notebook {
private:
	vector<Note> notes;
public:
	Notebook() : notes(vector<Note>()) { };

	const vector<Note>* get_notes() { return &notes; };
	unsigned int size() { return notes.size(); }

	void add_note(string note, string author, vector<string> tags = vector<string>())
	{
		Note n = Note(note, author, tags);
		notes.push_back(n);
	}

	void add_note(string note, string author, time_t creation_date, time_t change_date, vector<string> tags = vector<string>())
	{
		Note n = Note(note, author, tags, creation_date, change_date);
		notes.push_back(n);
	}

	void modify_note(int index, string note, string author, vector<string> tags = vector<string>())
	{
		notes[index].modification(note, author, tags);
	}

	void delete_note(int index)
	{
		notes.erase(notes.begin() + index);
	}

	// обнуление notebook (очищение от записей)
	void set_null() {
		notes = vector<Note>();
	}
};