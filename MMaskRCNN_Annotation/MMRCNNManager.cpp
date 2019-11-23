#include "MMRCNNManager.h"
#include "MainSystem.h"
#include <cassert>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "Keybord.h"
#include "EditData.h"

using namespace cv;

MMRCNNManager::MMRCNNManager() {}
MMRCNNManager::~MMRCNNManager() {
	EditData::ResetDataNum();
}

void MMRCNNManager::MainProcess() {
	if (isDebug) cout << "---ReadData---" << endl;
	ReadData();
	if (isDebug) cout << "---EditData---" << endl;
	EditPlay();
	if (isDebug) cout << "---ExportJsonFile---" << endl;
	ExportJsonFile();
}

string MMRCNNManager::GetSaveDirName() {
	return dir_save;
}

void MMRCNNManager::SelectTargetDirectory() {
	InputTargetDirectory();
}

void MMRCNNManager::InputTargetDirectory() {
	//ディレクトリ名をキー入力
	string d_r;
	cout << "Please input directory which has via_region_data.json." << endl;
	cout << "'same' : the directory you made just now is chosen." << endl;
	cout << "          'Reinforce to Augment' or 'Augment to Annotation'" << endl;
	cout << "folder name:";
	cin >> d_r;
	if (d_r == "same") d_r = InputSameDirectory();
	dir_tar = "data/" + d_r + "/";
}

bool MMRCNNManager::EditSameImage() {
	Mat img_guide = imread("system_images/AugmentData_GoToNextImage.png", IMREAD_COLOR);
	imshow("Guide", img_guide);
	int key;

	while (true) {
		key = waitKey(0);
		switch (key)
		{
		case KEY_ENTER:
			destroyAllWindows();
			return true;
		case KEY_RIGHT:
			destroyAllWindows();
			return false;
		default:
			break;
		}
	}
}

void MMRCNNManager::ExportJsonFile() {
	assert(!this->dir_save.empty() && "MMRCNNManager::ExportJsonFile(): The value 'dir_save' is empty.");
	ofstream output_file(this->dir_save + "via_region_data.json");
	picojson::value val(json_dst);
	output_file << val.serialize();
	output_file.close();
}