#include "TargetDataManager.h"
#include "MainSystem.h"

TargetDataManager::TargetDataManager() {}
TargetDataManager::~TargetDataManager() {}

void TargetDataManager::ReadData() {
	SelectTargetDirectory();
	string json_file_name = "via_region_data.json";
	cout << dir_tar << endl;
	ReadTargetData(dir_tar + json_file_name);
}
void TargetDataManager::ReadTargetData(const string& path) {
	json_data_input.ReadJson(path);
}