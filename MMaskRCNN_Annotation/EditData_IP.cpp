#include "EditData_IP.h"
#include "MainSystem.h"
#include <iomanip>

EditData_IP::EditData_IP() {}
EditData_IP::~EditData_IP() {}

void EditData_IP::SetTargetImage(JsonData& data, size_t itr) {
	//itr:0~num_img-1
	picojson::object obj_img = data.GetObjImg(itr);
	data_tar = new ImageData(obj_img, this->dir_tar);
	img_tar = imread(data_tar->image_dir + data_tar->image_file_name, IMREAD_COLOR);
}

void EditData_IP::Finalize(bool showResult) {
	imwrite(dir_save + data_dst->image_file_name, img_dst);
	if (showResult) {
		data_dst->ShowInfo();
		data_dst->ShowRegions();
		Mat img_guide = imread("system_images/finalize_sign.png", IMREAD_COLOR);
		imshow("Guide", img_guide);
		waitKey(0);
		destroyAllWindows();
	}
}

void EditData_IP::Skip() {
	Mat img_guide = imread("system_images/EditData_Skip.png", IMREAD_COLOR);
	imshow("Guide", img_guide);
	waitKey(0);
	destroyAllWindows();
}