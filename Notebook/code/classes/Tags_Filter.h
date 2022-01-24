#pragma once
#include "Filter.h"
#include <iterator>

class Tags_Filter : public Filter {
private:
	vector<string> all_tags; // список доступных тегов
	vector<string> search_tags; // список выбранныъ тегов
public:
	Tags_Filter(Notebook* nb, vector<string> search_tags = vector<string>()) : Filter(nb), all_tags(vector<string>()), search_tags(search_tags) {
		update_data();
	};
	Tags_Filter(Notebook* nb, vector<Note> old_notes, vector<string> search_tags = vector<string>()) : Filter(nb, old_notes), all_tags(vector<string>()), search_tags(search_tags) {
		update_data();
	};

	const vector<string>* get_all_tags() { return &all_tags; }
	void set_search(vector<string> search) { search_tags = search; }

	void update_data() {
		for_each(old_notes.begin(), old_notes.end(), [&](Note n) {
			vector<string> tags = n.get_tags();
			for_each(tags.begin(), tags.end(), [&](string s) {
				if (find(all_tags.begin(), all_tags.end(), s) == all_tags.end()) {
					all_tags.push_back(s);
				}
				});
			});
	}

	void check_data() {
		for (vector<string>::iterator it = search_tags.begin(); it != end(search_tags); )
		{
			if (find(all_tags.begin(), all_tags.end(), (*it)) == all_tags.end())
			{
				it = search_tags.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void make_filtration() {
		if (search_tags == vector<string>()) {
			filter_notes = old_notes;
			return;
		}
		check_data();

		for_each(old_notes.begin(), old_notes.end(), [&](Note n) {
			vector<string> tags = n.get_tags();
			bool stop = false;

			for (vector<string>::iterator it = tags.begin(); it != end(tags);) {
				for (vector<string>::iterator it_s = search_tags.begin(); it_s != end(search_tags);)
				{
					if (*(it) == *(it_s))
					{
						filter_notes.push_back(n);
						stop = true;
						break;
					}
					else
					{
						++it_s;
					}
				}
				if (stop) {
					stop = false;
					break;
				}
				++it;
			}
			});
	}
};
