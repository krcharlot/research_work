#include "EditData.h"
#include <opencv2/opencv.hpp>
using namespace cv;

int EditData::num = 0;

void EditData::ResetDataNum() {
	EditData::num = 1;
}

EditData::EditData() {}
EditData::~EditData() {}

void EditData::WriteToJson(picojson::object& json_obj) {
	picojson::object image;
	image.insert(make_pair("filename", this->data_dst->image_file_name));
	image.insert(make_pair("size", picojson::value(static_cast<double>(this->data_dst->image_file_size))));

	picojson::object file_attributes;
	image.insert(make_pair("file_attributes", picojson::value(file_attributes)));

	picojson::array regions;
	ImportRegiontoRegions(regions);
	image.insert(make_pair("regions", picojson::value(regions)));

	json_obj.insert(make_pair(this->data_dst->image_ID, picojson::value(image)));
}

void EditData::ImportRegiontoRegions(picojson::array& regions) {

	for (auto itr = this->data_dst->regions.begin(); itr != this->data_dst->regions.end(); itr++) {

		picojson::object region;

		picojson::object shape_attributes;
		picojson::array p_x, p_y;
		for (int j = 0; j < (*itr)->all_point_x.size(); j++) {
			p_x.push_back(picojson::value(static_cast<double>((*itr)->all_point_x[j])));
			p_y.push_back(picojson::value(static_cast<double>((*itr)->all_point_y[j])));
		}
		shape_attributes.insert(make_pair("name", "polygon"));
		shape_attributes.insert(make_pair("all_points_x", picojson::value(p_x)));
		shape_attributes.insert(make_pair("all_points_y", picojson::value(p_y)));

		picojson::object region_attributes;
		region_attributes.insert(make_pair((*itr)->group_name, (*itr)->region_name));

		region.insert(make_pair("shape_attributes", picojson::value(shape_attributes)));
		region.insert(make_pair("region_attributes", picojson::value(region_attributes)));

		regions.push_back(picojson::value(region));
	}
}