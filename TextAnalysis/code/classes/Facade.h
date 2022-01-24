#pragma once
#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "LengthWordAnalyzer.h"
#include "FrequencyWordAnalyzer.h"
#include "SentenceAnalyzer.h"

using namespace std;
namespace fs = filesystem;
using fs::path;

class Facade {
private:
	vector<path> files; // vector of .txt files
	unsigned int count_for_tops;
	// statistics
	map<string, int> fwa_map;
	map<string, int> lwa_map;
	unsigned int n;
	// processing setup
	bool ignore_case;
	bool ignore_special_characters;
	bool ignore_numbers;
	bool ignore_another_language;

	// delete all not .txt files
	void clear_files() {
		vector<path>::iterator it = files.begin();
		while (it != files.end())
		{
			if (it->extension() != ".txt")
				it = files.erase(it);
			else ++it;
		}
	}

	void make_statistics() {
		FrequencyWordAnalyzer fwa = FrequencyWordAnalyzer(files[0], ignore_case, ignore_special_characters, ignore_numbers, ignore_another_language);
		fwa_map = fwa.get_words();
		LengthWordAnalyzer lwa = LengthWordAnalyzer(files[0], ignore_case, ignore_special_characters, ignore_numbers, ignore_another_language);
		lwa_map = lwa.get_words();
		SentenceAnalyzer sa = SentenceAnalyzer(files[0]);
		n = sa.get_count();

		for (int i = 1; i < files.size(); i++) {
			fwa = FrequencyWordAnalyzer(files[i], ignore_case, ignore_special_characters, ignore_numbers, ignore_another_language, fwa_map);
			fwa_map = fwa.get_words();
			lwa = LengthWordAnalyzer(files[i], ignore_case, ignore_special_characters, ignore_numbers, ignore_another_language, lwa_map);
			lwa_map = lwa.get_words();
			sa = SentenceAnalyzer(files[i]);
			n += sa.get_count();
		}
	}
public:
	// display/writing setup
	bool short_statistics;
	bool get_dictionary;

	// constructor
	Facade(vector<path> files, unsigned int count_for_tops, bool ignore_case = false, bool ignore_special_characters = false, bool ignore_numbers = false,
		bool ignore_another_language = false, bool short_statistics = false, bool get_dictionary = false)
		: files(files), count_for_tops(count_for_tops), ignore_case(ignore_case), ignore_special_characters(ignore_special_characters),
		ignore_numbers(ignore_numbers), ignore_another_language(ignore_another_language),
		short_statistics(short_statistics), get_dictionary(get_dictionary)
	{
		fwa_map = map<string, int>();
		lwa_map = map<string, int>();
		n = 0;
		clear_files();
		if (this->files.size() > 0) {
			make_statistics();
		}
	};

	int show_statistic()
	{
		if (files.size() == 0) {
			return -1;
		}
		cout << "=========================================" << endl;
		cout << "---------- Текстовый анализатор ---------" << endl;
		cout << "=========================================" << endl << endl;

		cout << "Получены следующие файлы (список множества файлов для анализа): " << endl;
		for (path f : files) {
			cout << f.filename() << endl;
		}

		cout << endl << "---------- Которткая статистика (топ " << count_for_tops << " самых-самых слов) ---------" << endl;
		cout << "Самые часто встрчаемые слова:" << endl;
		vector<pair<string, int>> top_most = WordAnalyzer::get_top_count(fwa_map, count_for_tops);
		int i = 1;
		for_each(top_most.begin(), top_most.end(),
			[&](const pair<string, int>& p) {
				cout << i << ". " << p.first << " (" << p.second << " раз(а))\n"; i++;
			});
		cout << endl << "Самые длинные слова:" << endl;
		top_most = WordAnalyzer::get_top_count(lwa_map, count_for_tops);
		i = 1;
		for_each(top_most.begin(), top_most.end(),
			[&](const pair<string, int>& p) {
				cout << i << ". " << p.first << " (" << p.second << " символа/ов)\n"; i++;
			});
		cout << endl << "Всего предложений: " << n << endl;

		if (!short_statistics) {
			cout << endl << "---------- Полная статистика (ранжированные списки) ---------" << endl;
			cout << "Самые часто встрчаемые слова:" << endl;

			map<int, string> range_map = WordAnalyzer::get_ranked_map(fwa_map);
			for_each(range_map.begin(), range_map.end(),
				[](const pair<int, string>& p) {
					cout << "(" << p.first << " раз(а)) : " << p.second << "\n";
				});
			cout << endl << "Самые длинные слова:" << endl;
			range_map = WordAnalyzer::get_ranked_map(lwa_map);
			for_each(range_map.begin(), range_map.end(),
				[](const pair<int, string>& p) {
					cout << "(" << p.first << " символа/ов) : " << p.second << "\n";
				});
		}
		return 0;
	}

	void write_down_statistic(path file, string text = "")
	{
		ofstream new_file;
		new_file.open(file);

		if (text == "")
			text = "Текстовый анализ";

		new_file << "=========================================" << endl;
		new_file << "---------- "<< text <<" -----------" << endl;
		new_file << "=========================================" << endl << endl;

		new_file << "Получены следующие файлы (список множества файлов для анализа): " << endl;
		for (path f : files) {
			new_file << f.filename() << endl;
		}

		new_file << endl << "---------- Которткая статистика (топ " << count_for_tops << " самых-самых слов) ---------" << endl;
		new_file << "Самые часто встрчаемые слова:" << endl;
		vector<pair<string, int>> top_most = WordAnalyzer::get_top_count(fwa_map, count_for_tops);
		int i = 1;
		for_each(top_most.begin(), top_most.end(),
			[&](const pair<string, int>& p) {
				new_file << i << ". " << p.first << " (" << p.second << " раз(а))\n"; i++;
			});
		new_file << endl << "Самые длинные слова:" << endl;
		top_most = WordAnalyzer::get_top_count(lwa_map, count_for_tops);
		i = 1;
		for_each(top_most.begin(), top_most.end(),
			[&](const pair<string, int>& p) {
				new_file << i << ". " << p.first << " (" << p.second << " символа/ов)\n"; i++;
			});
		new_file << endl << "Всего предложений: " << n << endl;

		if (!short_statistics) {
			new_file << endl << "---------- Полная статистика (ранжированные списки) ---------" << endl;
			new_file << "Самые часто встрчаемые слова:" << endl;

			map<int, string> range_map = WordAnalyzer::get_ranked_map(fwa_map);
			for_each(range_map.begin(), range_map.end(),
				[&](const pair<int, string>& p) {
					new_file << "(" << p.first << " раз(а)) : " << p.second << "\n";
				});
			new_file << endl << "Самые длинные слова:" << endl;
			range_map = WordAnalyzer::get_ranked_map(lwa_map);
			for_each(range_map.begin(), range_map.end(),
				[&](const pair<int, string>& p) {
					new_file << "(" << p.first << " символа/ов) : " << p.second << "\n";
				});
		}
		if (true) {
			new_file << endl << "---------- Полная статистика (НЕ ранжированные списки) ---------" << endl;
			new_file << "------------- Слова с их частотой и длинной:" << endl;

			map<string, int>::iterator it = lwa_map.begin();
			for_each(fwa_map.begin(), fwa_map.end(),
				[&](const pair<string, int>& p) {
					new_file << ". " << p.first << " (" << p.second << " раз(а)) (" << it->second << " символа/ов)\n"; ++it;
				});
		}

		new_file.close();
	}
};