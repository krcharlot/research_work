#include "ReinforceManager.h"
#include "MainSystem.h"
#include <direct.h>//_mkdir

ReinforceManager::ReinforceManager() {
	cout << "------ReinforcementStart-------" << endl;
	if (isDebugClassManegement) cout << "MMRCNNReinforcement's constructer is called." << endl;

	// true:出力フォルダ名に現在時刻を表記
	bool written_time = true;
	if (written_time) {
		this->dir_save = "data/save_reinforcement/" + start_up_time + "/";
		assert(!_mkdir(dir_save.c_str()) && "MMRCNNReinforcement::MMRCNNReinforcement() : Making file directory failed.");
	}
	else {
		this->dir_save = "data/save_reinforcement/";
	}
}
ReinforceManager::~ReinforceManager() {
	cout << "------Reinforcement End-------" << endl;
	if (isDebugClassManegement) cout << "MMRCNNReinforcement's destructer is called." << endl;
}


string ReinforceManager::InputSameDirectory() const{
	return "save_reinforcement/" + start_up_time;
}

void ReinforceManager::EditPlay() {
	size_t num_imgs = json_data_input.GetNumObjects();
	for (size_t img_itr = 0; img_itr < num_imgs; img_itr++) {
		cout << img_itr + 1 << "/" << num_imgs << endl;
		reinforce_data = new ReinforceData(dir_tar, dir_save);
		reinforce_data->SetImageData(json_data_input, img_itr);
		reinforce_data->Editing();
		reinforce_data->WriteToJson(json_dst);
		delete reinforce_data;
	}
}