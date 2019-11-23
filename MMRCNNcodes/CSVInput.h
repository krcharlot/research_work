#pragma once
#include <fstream>
using namespace std;

class CSVInput {
public:
protected:
	const char delimeter;
	const string csv_dir;
	string line_buf;
	bool isRelativePosMode;
	string index;
	string img_num;
	string s_pos_x;
	string s_pos_y;
	string s_ang_deg;
	string s_scale;
	string csv_file_name;
	ifstream ifile;
	bool showResult;

	CSVInput(bool showResult = true);
	~CSVInput();
	void Initialize();
	virtual  void ShowValues() const;
};