#include "JsonData.h"
#include "MainSystem.h"
#include <fstream>
#include <cassert>
using namespace std;

JsonData::JsonData() {
	if (isDebugClassManegement) cout << "JsonData's constructer is called." << endl;
}

JsonData::JsonData(const string& path) {
	if (isDebugClassManegement) cout << "JsonData's constructer is called." << endl;
	ifstream ifs(path, ios::binary);
	assert(ifs && "JsonData::JsonData(const string&) : Json file is not find.");
	ifs >> this->val;
	this->obj_images = this->val.get<picojson::object>();
}

JsonData::JsonData(const JsonData& in) {
	if (isDebugClassManegement) cout << "JsonData's copy constructer is called." << endl;
	this->val = in.val;
	this->obj_images = in.obj_images;
}

JsonData::~JsonData() {
	if (isDebugClassManegement)  cout << "JsonData's destructer is called." << endl;
}

void JsonData::ReadJson(const string& path) {
	ifstream ifs(path, ios::binary);
	assert(ifs && "JsonData::ReadJson(const string&) : Json file is not find.");
	ifs >> this->val;
	this->obj_images = this->val.get<picojson::object>();
}

picojson::object JsonData::GetObjImg(size_t n){
	assert(n >= 0 && "JsonData::GetObjImg(int n) : n:0~num_img-1.");
	auto itr = obj_images.begin();
	for (int i = 0; i < n; i++) {
		itr++;
	}
	return obj_images[itr->first].get<picojson::object>();
}
size_t JsonData::GetNumObjects() {
	return obj_images.size();
}