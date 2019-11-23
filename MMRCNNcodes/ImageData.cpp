#include "ImageData.h"
#include "ObjectColorManager.h"
using namespace cv;

RegionData::RegionData() {}
RegionData::RegionData(const string g_name, const string r_name) : group_name(g_name), region_name(r_name) {}
RegionData::RegionData(const string g_name, const string r_name, const vector<int>& points_x, const vector<int>& points_y) : group_name(g_name), region_name(r_name), all_point_x(points_x), all_point_y(points_y) {}
RegionData::RegionData(const string g_name, const string r_name, const picojson::array& points_x, const picojson::array& points_y) : group_name(g_name), region_name(r_name) {
	for (int i = 0; i < points_x.size(); i++) {
		all_point_x.push_back((static_cast<int>(points_x[i].get<double>())));
		all_point_y.push_back((static_cast<int>(points_y[i].get<double>())));
	}
}
RegionData::RegionData(const RegionData& r) : group_name(r.group_name), region_name(r.region_name), all_point_x(r.all_point_x), all_point_y(r.all_point_y){}
RegionData::~RegionData() {}

void RegionData::ShowInfo() const{
	cout << "GroupName :" << group_name << endl;
	cout << "RegionName:" << region_name << endl;
	cout << "(x , y) = [";
	for (int i = 0; i < all_point_x.size(); i++) {
		cout << "(" << all_point_x[i] << "," << all_point_y[i] << "),";
	}
	cout << "]" << endl;
}

ImageData::ImageData() {}
ImageData::ImageData(const string& dir, const string& id, const string& f_n, const unsigned int& f_s) :image_dir(dir), image_ID(id), image_file_name(f_n), image_file_size(f_s) {}

ImageData::ImageData(picojson::object& obj_img, const string& dir) {
	
	this->image_dir = dir;
	this->image_file_name = obj_img["filename"].get<string>();
	this->image_file_size = static_cast<unsigned int>(obj_img["size"].get <double>());
	this->image_ID = image_file_name + to_string(image_file_size);

	picojson::array& ary_regions = obj_img["regions"].get<picojson::array>();
	for (int i = 0; i < ary_regions.size(); i++) {
		picojson::object& obj_region = ary_regions[i].get<picojson::object>();

		picojson::object& obj_region_attributes = obj_region["region_attributes"].get<picojson::object>();
		string group_name = obj_region_attributes.begin()->first;
		string region_name = obj_region_attributes[obj_region_attributes.begin()->first].get<string>();

		picojson::object& obj_shape_attributes = obj_region["shape_attributes"].get<picojson::object>();
		picojson::array& ary_points_x = obj_shape_attributes["all_points_x"].get<picojson::array>();
		picojson::array& ary_points_y = obj_shape_attributes["all_points_y"].get<picojson::array>();
		
		RegisterRegion(group_name, region_name, ary_points_x, ary_points_y);
	}
}
ImageData::ImageData(const ImageData& image_data) {
	this->image_dir = image_data.image_dir;
	this->image_ID = image_data.image_ID;
	this->image_file_name = image_data.image_file_name;
	this->image_file_size = image_data.image_file_size;
	for (size_t i = 0; i < image_data.regions.size(); i++) {
		this->regions.push_back(new RegionData(*(image_data.regions[i])));
	}
}

ImageData::ImageData(const string& dir, const string& id, const string& f_n, const ImageData& img_data) {
	this->image_dir = dir;
	this->image_ID = id + "_" + img_data.image_ID;
	this->image_file_name = f_n + "_" + img_data.image_file_name;
	this->image_file_size = img_data.image_file_size;
	for (size_t i = 0; i < regions.size(); i++) {
		this->regions.push_back(new RegionData(*(img_data.regions[i])));
	}
}

ImageData::~ImageData() {
	for (int i = 0; i < regions.size(); i++) {
		delete regions[i];
	}
}

void ImageData::RegisterRegion(const string g_name, const string r_name, const vector<int>& points_x, const vector<int>& points_y) {
	regions.push_back(new RegionData(g_name, r_name, points_x, points_y));
}
void ImageData::RegisterRegion(const string g_name, const string r_name, const picojson::array& points_x, const picojson::array& points_y) {
	regions.push_back(new RegionData(g_name, r_name, points_x, points_y));
}
void ImageData::RegisterRegion(const RegionData* region) {
	regions.push_back(new RegionData(*region));
}

void ImageData::ShowInfo() const {
	cout << "ImageInfo" << endl;
	cout << "ImageID:" << image_ID << endl;
	cout << "ImageName:" << image_file_name << endl;
	cout << "size:" << image_file_size << endl;
	cout << "regions" << endl;
	for (size_t i = 0; i < regions.size(); i++) {
		regions[i]->ShowInfo();
	}
	cout << endl;
}

void ImageData::ShowRegions() const{

	Mat img = imread(image_dir + image_file_name, IMREAD_COLOR);
	
	int circle_size = 3;

	for (size_t r_itr = 0; r_itr < regions.size(); r_itr++) {
		RegionData reg(*(regions[r_itr]));
		Scalar color = ObjectColorManeger::GetInstance()->GetObjColor(reg.region_name);
		for (size_t p_itr = 0; p_itr < regions[r_itr]->all_point_x.size(); p_itr++) {
			circle(img, Point(reg.all_point_x[p_itr], reg.all_point_y[p_itr]), circle_size, color, -1, CV_AA);
		}

		/*領域名の表示
		//point[0]を参照しており、point[0]が画像内に含まれてなければエラー
		
		double font_scale = 0.7;
		int line_thickness = 1;
		int group_pos_offset = 20;

		putText(img, reg.region_name, cv::Point(reg.all_point_x[0], reg.all_point_y[0]),
			CV_FONT_HERSHEY_DUPLEX | CV_FONT_ITALIC,
			font_scale, color, line_thickness, CV_AA);
		putText(img, reg.group_name, cv::Point(reg.all_point_x[0], reg.all_point_y[0] + group_pos_offset),
			CV_FONT_HERSHEY_DUPLEX | CV_FONT_ITALIC,
			font_scale, color, line_thickness, CV_AA);
		*/
	}

	string win_name = "ImageData::ShowRegions()";
	imshow(win_name, img);
}