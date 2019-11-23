#include "AugmentManager.h"
#include "MainSystem.h"
#include <direct.h>//_mkdir
#include "Keybord.h"
#include <fstream>
#include <sstream>

AugmentManager::AugmentManager() {
	cout << "------AugmentmentStart-------" << endl;
	if (isDebugClassManegement) cout << "MMRCNNAugmentment's constructer is called." << endl;

	// true:出力フォルダ名に現在時刻を表記
	bool written_time = true;
	if (written_time) {
		this->dir_save = "data/save_augmentation/" + start_up_time + "/";
		assert(!_mkdir(dir_save.c_str()) && "MMRCNNAugmentation::MMRCNNAugmentation() : Making file directory failed.");
	}
	else {
		this->dir_save = "data/save_augmentation/";
	}
}
AugmentManager::~AugmentManager() {
	cout << "------Augmentment End-------" << endl;
	if (isDebugClassManegement) cout << "MMRCNNAugmentment's destructer is called." << endl;
}

string AugmentManager::InputSameDirectory() const {
	return "save_reinforcement/" + start_up_time;
}

AugmentManagerInputMode::AugmentManagerInputMode() {}
AugmentManagerInputMode::~AugmentManagerInputMode() {}

void AugmentManagerInputMode::EditPlay() {
	size_t num_imgs = json_data_input.GetNumObjects();
	for (size_t img_itr = 0; img_itr < num_imgs; img_itr++) {
		cout << img_itr + 1 << "/" << num_imgs << endl;
		augment_data = new AugmentDataInputMode(dir_tar, dir_save);
		augment_data->SetTargetImage(json_data_input, img_itr);

		bool edit_done = augment_data->Editing();

		if (edit_done) {
			augment_data->WriteToJson(json_dst);
			delete augment_data;
		}
		else {
			delete augment_data;
			continue;
		}
		if (EditSameImage()) img_itr--;
	}
}

AugmentManagerCSVMode::AugmentManagerCSVMode(bool correctMode) : isCorrectMode(correctMode) {}
AugmentManagerCSVMode::~AugmentManagerCSVMode() {}

void AugmentManagerCSVMode::EditPlay() {

	//結果表示のパラメータ
	showResult = false;

	CSVInput::Initialize();
	
	while (getline(ifile, line_buf)) {

		augment_data = new AugmentDataCSVMode(dir_tar, dir_save, isCorrectMode, showResult);

		istringstream line_separator(line_buf);
		getline(line_separator, index, delimeter);//1列目のindexの読み飛ばし
		cout << "index:" << index << endl;
		getline(line_separator, img_num, delimeter);
		size_t img_itr = static_cast<unsigned int>(stoi(img_num));
		augment_data->SetTargetImage(json_data_input, img_itr);

		getline(line_separator, s_pos_x, delimeter);
		getline(line_separator, s_pos_y, delimeter);
		getline(line_separator, s_ang_deg, delimeter);
		getline(line_separator, s_scale, delimeter);
		if(showResult) ShowValues();
		if (isRelativePosMode) augment_data->ReadParametersRatioMode(stod(s_pos_x), stod(s_pos_y), stoi(s_ang_deg), stod(s_scale));
		else augment_data->ReadParametersPixelMode(stoi(s_pos_x), stoi(s_pos_y), stoi(s_ang_deg), stod(s_scale));
	
		augment_data->Editing();
		augment_data->WriteToJson(json_dst);
		delete augment_data;
	}
}