#define _USE_MATH_DEFINES
#include "ImagePasting.h"

const int ImagePasting::VALUE_TRANSLATION = 15;
const int ImagePasting::VALUE_ROTATION = 15;
const double ImagePasting::VALUE_SCALING = 0.1;
const unsigned int ImagePasting::BACK_GROUND_VALUE = 0;
const string ImagePasting::win_name = "Editting";

const int ImagePasting::DEFAULT_POS_X = 0;
const int ImagePasting::DEFAULT_POS_Y = 0;
const int ImagePasting::DEFAULT_ANGLE = 0;
const double ImagePasting::DEFAULT_SCALE = 1.0;

int ImagePasting::init_pos_x_add_img = ImagePasting::DEFAULT_POS_X;
int ImagePasting::init_pos_y_add_img = ImagePasting::DEFAULT_POS_Y;
int ImagePasting::angle_degree = ImagePasting::DEFAULT_ANGLE;
double ImagePasting::scale_ratio = ImagePasting::DEFAULT_SCALE;

ImagePasting::ImagePasting() {}
ImagePasting::~ImagePasting () {}

void ImagePasting::MoveUP() {
	this->init_pos_y_add_img -= this->VALUE_TRANSLATION;
}
void ImagePasting::MoveDown() { 
	this->init_pos_y_add_img += this->VALUE_TRANSLATION;
}
void ImagePasting::MoveRight() {
	this->init_pos_x_add_img += this->VALUE_TRANSLATION; 
}
void ImagePasting::MoveLeft() {
	this->init_pos_x_add_img -= this->VALUE_TRANSLATION; 
}
void ImagePasting::TurnClockwise() {
	this->angle_degree -= this->VALUE_ROTATION;
}
void ImagePasting::TurnCounterClockwise() {
	this->angle_degree += this->VALUE_ROTATION;
}
void ImagePasting::ScaleUP() {
	this->scale_ratio += this->VALUE_SCALING;
}
void ImagePasting::ScaleDown() {
	this->scale_ratio -= this->VALUE_SCALING;
	if (this->scale_ratio < VALUE_SCALING) this->scale_ratio = VALUE_SCALING;
}
void ImagePasting::ResetValues() {
	this->init_pos_x_add_img = this->DEFAULT_POS_X;
	this->init_pos_y_add_img = this->DEFAULT_POS_Y;
	this->angle_degree = this->DEFAULT_ANGLE;
	this->scale_ratio = this->DEFAULT_SCALE;
}
void ImagePasting::InputValues(const int pos_x, const int pos_y, const int angle_deg, const double scale_ratio) {
	this->init_pos_x_add_img = pos_x;
	this->init_pos_y_add_img = pos_y;
	this->angle_degree = angle_deg;
	this->scale_ratio = scale_ratio;
}

void ImagePasting::VisualizePreview(const Mat& dst, const Mat& paste) const {
	Mat img_show = dst.clone();
	Mat paste_rot = RotateImage(paste);
	Mat paste_rot_scale = ScaleImage(paste_rot);
	PasteImage(img_show, paste_rot_scale);
	imshow(win_name, img_show);
}

void ImagePasting::UpdateImage(Mat& dst, const Mat& paste) const {
	Mat img_rot = RotateImage(paste);
	Mat img_rot_scale = ScaleImage(img_rot);
	PasteImage(dst, img_rot_scale);
}

void ImagePasting::UpdateRegionsData(ImageData& data_dst, const ImageData& data_ori, const Mat& img_obj, const Mat& img_tar) const {
	for (size_t itr_r = 0; itr_r < data_ori.regions.size(); itr_r++) {
		RegionData r_data(*(data_ori.regions[itr_r]));
		vector<int> points_x = r_data.all_point_x;
		vector<int> points_y = r_data.all_point_y;

		int pos_x, pos_y;
		vector<int> all_points_x;
		vector<int> all_points_y;
		for (size_t itr_v = 0; itr_v < points_x.size(); itr_v++) {
			Point2d point(points_x[itr_v], points_y[itr_v]);
			Point2d point_dst = RotatePoint(img_tar, point);
			ScalePoint(point_dst);
			pos_x = static_cast<int>(point_dst.x) + this->init_pos_x_add_img;
			pos_y = static_cast<int>(point_dst.y) + this->init_pos_y_add_img;
			//３点以上を含まない場合がある（領域ができない）
			// ここでは画像表示のため書き込んでおく必要がある
			if (withinCols(img_obj.cols, pos_x))
				if (withinRows(img_obj.rows, pos_y)) {
					all_points_x.push_back(pos_x);
					all_points_y.push_back(pos_y);
				}
		}
		if (all_points_x.size() != 0) {
			data_dst.RegisterRegion(r_data.group_name, r_data.region_name, all_points_x, all_points_y);
		}
	}
}

Mat ImagePasting::GetAffineMatrix(const Mat& img) const{
	/*Affine Matrix
	|x'|    | a  b  t_x| |x|
	|y'| = | c  d  t_y| |y|
	|1 |    | 0  0    1 | |1|
	*/
	int height_ori = img.rows;
	int width_ori = img.cols;

	double angle_rad = static_cast<double>(angle_degree) / 180.0 * M_PI;

	int height_rot = static_cast<int>(static_cast<double>(height_ori * abs(cos(angle_rad))) + static_cast<double>(width_ori) * abs(sin(angle_rad)));
	int width_rot = static_cast<int>(static_cast<double>(height_ori * abs(sin(angle_rad))) + static_cast<double>(width_ori) * abs(cos(angle_rad)));

	Point2d rot_center(width_ori / 2.0, height_ori / 2.0);
	double rot_scale = 1.0;
	Mat rot_matrix;
	getRotationMatrix2D(rot_center, (double)angle_degree, 1.0).copyTo(rot_matrix);
	Mat affine_matrix = rot_matrix.clone();

	Point2d affine_center(width_rot / 2, height_rot / 2);

	affine_matrix.at<double>(0, 2) = rot_matrix.at<double>(0, 2) - static_cast<double>(rot_center.x) + static_cast<double>(affine_center.x);
	affine_matrix.at<double>(1, 2) = rot_matrix.at<double>(1, 2) - static_cast<double>(rot_center.y) + static_cast<double>(affine_center.y);

	return affine_matrix;
}
bool ImagePasting::withinCols(int width, int pos_x) const {
	if (pos_x >= 0 && pos_x < width) return true;
	else return false;
}
bool ImagePasting::withinRows(int height, int pos_y) const {
	if (pos_y >= 0 && pos_y < height) return true;
	else return false;
}

bool ImagePasting::isSubjectPixel(const Mat& img, int pos_x, int pos_y) const {
	for (int ch = 0; ch < img.channels(); ch++) {
		if (img.at<Vec3b>(pos_y, pos_x)[ch] != BACK_GROUND_VALUE) {
			return true;
		}
	}
	return false;
}

void ImagePasting::PasteImage(Mat& dst, const Mat& paste) const {
	int pos_x;
	int pos_y;
	int canvas_height = dst.rows;
	int canvas_width = dst.cols;
	for (int y = 0; y < paste.rows; y++) {
		for (int x = 0; x < paste.cols; x++) {
			pos_x = x + init_pos_x_add_img;
			pos_y = y + init_pos_y_add_img;
			if (withinCols(canvas_width, pos_x))
				if (withinRows(canvas_height, pos_y))
					if (isSubjectPixel(paste, x, y)) {
						for (int c = 0; c < paste.channels(); c++) {
							dst.at<Vec3b>(pos_y, pos_x)[c] = paste.at<Vec3b>(y, x)[c];
						}
					}
		}
	}
}

Mat ImagePasting::RotateImage(const Mat& img) const {
	Mat ret = img.clone();
	double angle_rad = angle_degree / 180.0 * M_PI;
	int height = img.rows;
	int width = img.cols;
	int height_rot = static_cast<int>(static_cast<double>(height)*abs(cos(angle_rad)) + static_cast<double>(width)*abs(sin(angle_rad)));
	int width_rot = static_cast<int>(static_cast<double>(height)*abs(sin(angle_rad)) + static_cast<double>(width)*abs(cos(angle_rad)));
	Mat affine_matrix = GetAffineMatrix(img);
	warpAffine(img, ret, affine_matrix, Size(width_rot, height_rot), CV_INTER_NN);
	//imshow("Rotation", ret);
	return ret;
}

Mat ImagePasting::ScaleImage(const Mat& img) const {
	Mat ret;
	resize(img, ret, Size(), this->scale_ratio, this->scale_ratio, INTER_NEAREST);
	return ret;
}

Point2d ImagePasting::RotatePoint(const Mat& img, const Point2d& point) const {
	double angle_rad = -this->angle_degree / 180.0 * M_PI;

	int height_ori = img.rows;
	int width_ori = img.cols;
	int height_rot = static_cast<int>(static_cast<double>(height_ori * abs(cos(angle_rad))) + static_cast<double>(width_ori) * abs(sin(angle_rad)));
	int width_rot = static_cast<int>(static_cast<double>(height_ori * abs(sin(angle_rad))) + static_cast<double>(width_ori) * abs(cos(angle_rad)));

	Point2d center_ori(width_ori / 2, height_ori / 2);
	Point2d center_rot(width_rot / 2, height_rot / 2);

	/*Rotate Matrix
	|x'| = | a  b | |x|
	|y'| = | c  d | |y|
	*/
	double a = cos(angle_rad);
	double b = -sin(angle_rad);
	double c = sin(angle_rad);
	double d = cos(angle_rad);

	Point2d point_ret;
	point_ret.x = static_cast<int>(a * ((double)(point.x) - center_ori.x)) + static_cast<int>(b * ((double)(point.y) - center_ori.y)) + (int)center_rot.x;
	point_ret.y = static_cast<int>(c * ((double)(point.x) - center_ori.x)) + static_cast<int>(d * ((double)(point.y) - center_ori.y)) + (int)center_rot.y;

	return point_ret;
}

void ImagePasting::ScalePoint(Point2d& point) const {
	point.x *= this->scale_ratio;
	point.y *= this->scale_ratio;
}