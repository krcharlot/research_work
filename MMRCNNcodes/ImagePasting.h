#pragma once
#include "ImageData.h"
#include <opencv2/opencv.hpp>
using namespace cv;

class ImagePasting {
public:
	ImagePasting();
	~ImagePasting();
	void MoveUP();
	void MoveDown();
	void MoveRight();
	void MoveLeft();
	void TurnClockwise();
	void TurnCounterClockwise();
	void ScaleUP();
	void ScaleDown();
	void ResetValues();
	void InputValues(const int pos_x, const int pos_y, const int angle_deg, const double scale_ratio);
	void VisualizePreview(const Mat& dst, const Mat& paste) const;
	void UpdateImage(Mat& dst, const Mat& paste) const;
	void UpdateRegionsData(ImageData& data_dst, const ImageData& data_tar, const Mat& img_obj, const Mat& img_tar) const;

private:
	static const int VALUE_TRANSLATION;
	static const int VALUE_ROTATION;
	static const double VALUE_SCALING;
	static const unsigned int BACK_GROUND_VALUE;
	static const string win_name;

	static const int DEFAULT_POS_X;
	static const int DEFAULT_POS_Y;
	static const int DEFAULT_ANGLE;
	static const double DEFAULT_SCALE;

	static int init_pos_x_add_img;
	static int init_pos_y_add_img;
	static int angle_degree;
	static double scale_ratio;
	
	Mat GetAffineMatrix(const Mat& img) const;
	bool withinCols(int width, int pos_x) const;
	bool withinRows(int height, int pos_y) const;
	bool isSubjectPixel(const Mat& img, int pos_x, int pos_y) const;
	void PasteImage(Mat& dst, const Mat& paste) const;
	Mat RotateImage(const Mat& img) const;
	Mat ScaleImage(const Mat& img) const;
	Point2d RotatePoint(const Mat& img, const Point2d& point) const;
	void ScalePoint(Point2d& point) const;
};