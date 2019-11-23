#pragma once
#include "picojson.h"
#include <string>

class JsonData {
	picojson::value val;
	picojson::object obj_images;
public:
	JsonData();
	JsonData(const std::string& path);
	JsonData(const JsonData&);
	~JsonData();
	void ReadJson(const std::string& path);
	picojson::object GetObjImg(size_t n);
	size_t GetNumObjects();
};