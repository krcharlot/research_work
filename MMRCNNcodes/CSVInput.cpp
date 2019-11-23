#include "CSVInput.h"
#include <iostream>
#include <sstream>
#include <cassert>

CSVInput::CSVInput(bool sResult) : showResult(sResult), delimeter(','), csv_dir("data/") {}
CSVInput::~CSVInput() {
	ifile.close();
}

void CSVInput::Initialize() {
	cout << "CSV file name(without extension):";
	cin >> csv_file_name;
	ifile.open(csv_dir + csv_file_name + ".csv");
	assert(ifile && "CSVInput::Initialize() : CSV file is not found.");
	getline(ifile, line_buf);
	istringstream line_separator(line_buf);
	string separator_buf;
	getline(line_separator, separator_buf, delimeter);//1—ñ–Ú‚Ìmode‚Ì“Ç‚İ”ò‚Î‚µ
	getline(line_separator, separator_buf, delimeter);
	isRelativePosMode = (separator_buf == "ratio") ? true : false;
	getline(ifile, line_buf);//1s–Ú‚Ì€–Ú–¼‚Ì“Ç‚İ”ò‚Î‚µ
}

void CSVInput::ShowValues() const {
	cout << index << " " << img_num << " " << s_pos_x << " " << s_pos_y << " " << s_ang_deg << " " << s_scale << endl;
}