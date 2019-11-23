#pragma once
#include <string>
#include <vector>
#include "picojson.h"
using namespace std;

class RegionData {
private:
public:
	const string group_name;
	const string region_name;
	vector<int> all_point_x;
	vector<int> all_point_y;

	RegionData();
	RegionData(const string g_name, const string r_name);
	RegionData(const string g_name, const string r_name, const vector<int>& points_x, const vector<int>& points_y);
	RegionData(const string g_name, const string r_name, const picojson::array& points_x, const picojson::array& points_y);
	RegionData(const RegionData& r);
	~RegionData();
	void ShowInfo() const;
};

class ImageData {
private:
public:
	string image_dir;
	string image_ID;
	string image_file_name;
	unsigned int image_file_size;
	vector<RegionData*> regions;

	ImageData();
	ImageData(const string& dir, const string& id, const string& f_n, const unsigned int& f_s);
	ImageData(picojson::object& obj_img, const string& dir);
	ImageData(const ImageData& image_data);
	ImageData(const string& dir, const string& id, const string& f_n, const ImageData& image_data);
	~ImageData();

	void RegisterRegion(const string g_name, const string r_name, const vector<int>& points_x, const vector<int>& points_y);
	void RegisterRegion(const string g_name, const string r_name, const picojson::array& points_x, const picojson::array& points_y);
	void RegisterRegion(const RegionData* region);
	void ShowInfo() const;
	void ShowRegions() const;
};