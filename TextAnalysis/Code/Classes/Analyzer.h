#pragma once
#include <filesystem>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;
namespace fs = filesystem;
using fs::path;

class Analyzer {
private:
	path path_to_file;
protected:
	path get_path() { return path_to_file; };

	string get_content()
	{
		// check
		if (!is_regular_file(path_to_file))
			return { };
		ifstream file(path_to_file, ios::in);
		if (!file.is_open())
			return { };

		// read
		const uintmax_t& size = file_size(path_to_file);
		string content(size, '\0');
		file.read(content.data(), size);

		// close
		file.close();

		return content;
	}
public:
	Analyzer(path path_to_file) : path_to_file(path_to_file) { };
};