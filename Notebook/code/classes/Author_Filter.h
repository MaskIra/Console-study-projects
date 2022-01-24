#pragma once
#include "Filter.h"

class Author_Filter : public Filter {
private:
	vector<string> all_authors; // список доступных авторов
	vector<string> search_authors; // список выбранных авторов
public:
	Author_Filter(Notebook* nb, vector<string> search_authors = vector<string>()) : Filter(nb), all_authors(vector<string>()), search_authors(search_authors) {
		update_data();
	};
	Author_Filter(Notebook* nb, vector<Note> old_notes, vector<string> search_authors = vector<string>()) : Filter(nb, old_notes), all_authors(vector<string>()), search_authors(search_authors) {
		update_data();
	};

	const vector<string>* get_all_authors() { return &all_authors; }
	void set_search(vector<string> search) { search_authors = search; }

	void update_data() {
		for_each(old_notes.begin(), old_notes.end(), [&](Note n) {
			string s = n.get_author();
			if (find(all_authors.begin(), all_authors.end(), s) == all_authors.end()) {
				all_authors.push_back(s);
			}
			});
	}

	void check_data() {
		for (vector<string>::iterator it = search_authors.begin(); it != end(search_authors); )
		{
			if (find(all_authors.begin(), all_authors.end(), (*it)) == all_authors.end())
			{
				it = search_authors.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void make_filtration() {
		if (search_authors == vector<string>()) {
			filter_notes = old_notes;
			return;
		}
		check_data();

		for_each(old_notes.begin(), old_notes.end(), [&](Note n) {
			string s = n.get_author();

			for (vector<string>::iterator it_s = search_authors.begin(); it_s != end(search_authors);)
			{
				if (s == *(it_s))
				{
					filter_notes.push_back(n);
					break;
				}
				else
				{
					++it_s;
				}
			}
			});
	}
};
