#include "AnnotateManager.h"
#include "MainSystem.h"
#include <direct.h>
#include "Keybord.h"

AnnotateManager::AnnotateManager() {
	cout << "------Annotation Start-------" << endl;
	if (isDebugClassManegement) cout << "MMRCNNAnnotation's constructer is called." << endl;

	// true:出力フォルダ名に現在時刻を表記
	bool written_time = true;
	if (written_time) {
		this->dir_save = "data/save_annotation/" + start_up_time + "/";
		assert(!_mkdir(dir_save.c_str()) && "MMRCNNAnnotation::MMRCNNAnnotation() : Making file directory failed.");
	}
	else {
		this->dir_save = "data/save_annotation/";
	}
}

AnnotateManager::~AnnotateManager() {
	cout << "------Annotation End-------" << endl;
	if (isDebugClassManegement) cout << "MMRCNNAnnotation's destructer is called." << endl;
}

void AnnotateManager::ReadData() {

	SelectTargetDirectory();

	v_dir_added.push_back("data/added/r_circle/");
	v_dir_added.push_back("data/added/r_triangle/");
	v_dir_added.push_back("data/added/r_arrow/");

	string json_file_name = "via_region_data.json";

	ReadTargetData(dir_tar + json_file_name);
	for (int i = 0; i < v_dir_added.size(); i++) {
		ReadAddedData(v_dir_added[i] + json_file_name);
	}
}
void AnnotateManager::ReadTargetData(const string& path) {
	json_data_input_target.ReadJson(path);
}
void AnnotateManager::ReadAddedData(const string& path) {
	v_json_data_input_added.push_back(JsonData(path));
}

string AnnotateManager::InputSameDirectory() const {
	return "save_augmentation/" + start_up_time;
}

AnnotateManagerInputMode::AnnotateManagerInputMode() {}
AnnotateManagerInputMode::~AnnotateManagerInputMode() {}

void AnnotateManagerInputMode::EditPlay() {
	size_t num_imgs = json_data_input_target.GetNumObjects();
	for (size_t img_itr = 0; img_itr < num_imgs; img_itr++) {
		cout << img_itr + 1 << "/" << num_imgs << endl;
		annotate_data = new AnnotateDataInputMode(dir_tar, v_dir_added, dir_save);
		annotate_data->SetTargetImage(json_data_input_target, img_itr);
		annotate_data->SetAddedImages(v_json_data_input_added);

		bool edit_done = annotate_data->Editing();
		if (edit_done) {
			annotate_data->WriteToJson(json_dst);
			delete annotate_data;
		}
		else {
			delete annotate_data;
			continue;
		}
		if (EditSameImage()) img_itr--;
	}
}

AnnotateManagerCSVMode::AnnotateManagerCSVMode(bool correctMode) : isCorrectMode(correctMode) {}
AnnotateManagerCSVMode::~AnnotateManagerCSVMode() {}

void AnnotateManagerCSVMode::EditPlay() {

	//結果表示のパラメータ
	showResult = false;

	CSVInput::Initialize();

	while (getline(ifile, line_buf)) {

		annotate_data = new AnnotateDataCSVMode(dir_tar, v_dir_added, dir_save, isCorrectMode, showResult);

		istringstream line_separator(line_buf);
		getline(line_separator, index, delimeter);//1列目のindexの読み飛ばし
		cout << "index:" << index << endl;
		getline(line_separator, img_num, delimeter);
		size_t img_itr = static_cast<unsigned int>(stoi(img_num));
		annotate_data->SetTargetImage(json_data_input_target, img_itr);
		annotate_data->SetAddedImages(v_json_data_input_added);
		
		getline(line_separator, s_add_img_num, delimeter);
		while (s_add_img_num != "" && !line_separator.eof()) {
			getline(line_separator, s_pos_x, delimeter);
			getline(line_separator, s_pos_y, delimeter);
			getline(line_separator, s_ang_deg, delimeter);
			getline(line_separator, s_scale, delimeter);
			if (showResult) ShowValues();
			if (isRelativePosMode) annotate_data->ReadParametersRatioMode(stoi(s_add_img_num), stod(s_pos_x), stod(s_pos_y), stoi(s_ang_deg), stod(s_scale));
			else annotate_data->ReadParametersPixelMode(stoi(s_add_img_num), stoi(s_pos_x), stoi(s_pos_y), stoi(s_ang_deg), stod(s_scale));
			getline(line_separator, s_add_img_num, delimeter);
		}
		annotate_data->Editing();
		annotate_data->WriteToJson(json_dst);
		delete annotate_data;
	}
}

void AnnotateManagerCSVMode::ShowValues() const {
	cout << index << " " << img_num << " " << s_add_img_num << " " << s_pos_x << " " << s_pos_y << " " << s_ang_deg << " " << s_scale << endl;
}