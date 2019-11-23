#include "AnnotateData.h"
#include "Keybord.h"
#include "MainSystem.h"
#include <iomanip>

AnnotateData::AnnotateData(const string& d_tar, const vector<string>& v_d_add, const string& d_save) {
	num++;
	this->num_added_imgs = 0;
	this->dir_tar = d_tar;
	this->v_dir_added = v_d_add;
	this->dir_save = d_save;
}

AnnotateData::~AnnotateData() {
	delete data_tar;
	delete data_dst;
}

void AnnotateData::SetAddedImages(vector<JsonData>& v_data_add) {
	for (size_t dir_itr = 0; dir_itr < v_data_add.size(); dir_itr++) {
		size_t num_img = v_data_add[dir_itr].GetNumObjects();
		for (size_t img_itr = 0; img_itr < num_img; img_itr++) {
			picojson::object obj_img = v_data_add[dir_itr].GetObjImg(img_itr);
			ImageData data_tmp(obj_img, v_dir_added[dir_itr]);
			v_data_added.push_back(data_tmp);
			v_img_added.push_back(imread(data_tmp.image_dir + data_tmp.image_file_name, IMREAD_COLOR));
			num_added_imgs++;
		}
	}
}

void AnnotateData::Initialize(string id) {
	
	img_dst = img_tar.clone();

	stringstream ss;
	ss << setfill('0') << setw(FILE_NUM_DIGIT) << right << to_string(num);
	data_dst = new ImageData(dir_save, ss.str() + id, ss.str() + id, *data_tar);
	for (size_t itr = 0; itr < data_tar->regions.size(); itr++) {
		data_dst->RegisterRegion(data_tar->regions[itr]);
	}
}

AnnotateDataInputMode::AnnotateDataInputMode(const string& d_tar, const vector<string>& v_d_add, const string& d_save) : AnnotateData(d_tar, v_d_add, d_save) {}
AnnotateDataInputMode::~AnnotateDataInputMode() {}

bool AnnotateDataInputMode::Editing() {

	Initialize("_AN");

	while (true) {
		int itr_select_img = SelectAddedImage();//-1でその画像の編集終了

		//ENTERキー
		if (itr_select_img == -1) {
			destroyAllWindows();
			Finalize();
			return true;
		}
		//DELETEキー
		else if (itr_select_img == -2) {
			destroyAllWindows();
			Skip();
			return false;
		}
		ImageData data_added = v_data_added[itr_select_img];
		Mat img_added = v_img_added[itr_select_img].clone();

		ShowMenu();

		int key = -1;
		p_IP = new ImagePasting();

		while (key != KEY_ENTER) {

			switch (key)
			{
			case KEY_UP:
				p_IP->MoveUP();
				break;
			case KEY_DOWN:
				p_IP->MoveDown();
				break;
			case KEY_LEFT:
				p_IP->MoveLeft();
				break;
			case KEY_RIGHT:
				p_IP->MoveRight();
				break;
			case KEY_A:
				p_IP->ScaleUP();
				break;
			case KEY_S:
				p_IP->ScaleDown();
				break;
			case KEY_D:
				p_IP->TurnCounterClockwise();
				break;
			case KEY_F:
				p_IP->TurnClockwise();
				break;
			case KEY_I:
				p_IP->ResetValues();
				break;
			case KEY_R:
				delete data_dst;
				Initialize("_AN");
				goto IP_SKIP;
				break;
			case KEY_BACKSPACE:
				goto IP_SKIP;
				break;
			default:
				break;
			}
			if (InputSmoother(key = waitKey(10))) continue;
			p_IP->VisualizePreview(img_dst, img_added);
			key = waitKey(0);
		}
		p_IP->UpdateImage(img_dst, img_added);
		p_IP->UpdateRegionsData(*data_dst, data_added, img_dst, img_added);

	IP_SKIP:
		destroyAllWindows();
	}
}

int AnnotateDataInputMode::SelectAddedImage() {
	Mat img_menu = imread("system_images/SelectAddedImage_Menu.png", IMREAD_COLOR);
	imshow("Menu", img_menu);

	Mat img_dst_view;
	resize(img_dst, img_dst_view, Size(), 0.5, 0.5);
	imshow("img_dst", img_dst_view);

	Mat img;
	int select_num = 0;
	int key = -1;
	bool isEnd = false;
	while (!isEnd) {
		resize(v_img_added[select_num], img, Size(), 0.5, 0.5);
		imshow("selected img", img);
		key = waitKey(0);
		switch (key)
		{
		case KEY_ENTER:
			select_num = -1;
			isEnd = true;
			break;
		case KEY_SPACE:
			isEnd = true;
			break;
		case KEY_DELETE:
			select_num = -2;
			isEnd = true;
			break;
		case KEY_RIGHT:
			if (select_num < static_cast<int>(num_added_imgs - 1)) {
				select_num++;
			}
			else {
				select_num = 0;
			}
			break;
		case KEY_LEFT:
			if (select_num == 0) {
				select_num = num_added_imgs - 1;
			}
			else {
				select_num--;
			}
		default:
			break;
		}
	}
	destroyWindow("Menu");
	return select_num;
}

void AnnotateDataInputMode::ShowMenu() {
	Mat img_menu = imread("system_images/AnnotateData_EditData.png", IMREAD_COLOR);
	imshow("Menu", img_menu);
}

AnnotateDataCSVMode::AnnotateDataCSVMode(const string& d_tar, const vector<string>& v_d_add, const string& d_save, bool correctMode, bool sResult) : AnnotateData(d_tar, v_d_add, d_save) ,isCorrectMode(correctMode), showResult(sResult) {}
AnnotateDataCSVMode::~AnnotateDataCSVMode() {}

void AnnotateDataCSVMode::ReadParametersRatioMode(const int add_img_itr, const double pos_x_ratio, const double pos_y_ratio, const int ang_deg, const double scale) {
	
	int pos_x_pixel = static_cast<int>(img_tar.cols * pos_x_ratio);
	int pos_y_pixel = static_cast<int>(img_tar.rows * pos_y_ratio);

	v_add_img_num.push_back(add_img_itr);
	v_pos_x.push_back(pos_x_pixel);
	v_pos_y.push_back(pos_y_pixel);
	v_ang_deg.push_back(ang_deg);
	v_scale.push_back(scale);
}

void AnnotateDataCSVMode::ReadParametersPixelMode(const int add_img_itr, const int pos_x, const int pos_y, const int ang_deg, const double scale) {
	v_add_img_num.push_back(add_img_itr);
	v_pos_x.push_back(pos_x);
	v_pos_y.push_back(pos_y);
	v_ang_deg.push_back(ang_deg);
	v_scale.push_back(scale);
}

bool AnnotateDataCSVMode::Editing() {

	Initialize("_AN");

	for (size_t itr = 0; itr < v_add_img_num.size(); itr++) {
		ImageData data_added = v_data_added[v_add_img_num[itr]];
		Mat img_added = v_img_added[v_add_img_num[itr]].clone();

		p_IP = new ImagePasting();
		p_IP->InputValues(v_pos_x[itr], v_pos_y[itr], v_ang_deg[itr], v_scale[itr]);

		if (isCorrectMode) {
			ShowMenu();

			int key = -1;
			//p_IP = new ImagePasting();

			while (key != KEY_ENTER) {

				switch (key)
				{
				case KEY_UP:
					p_IP->MoveUP();
					break;
				case KEY_DOWN:
					p_IP->MoveDown();
					break;
				case KEY_LEFT:
					p_IP->MoveLeft();
					break;
				case KEY_RIGHT:
					p_IP->MoveRight();
					break;
				case KEY_A:
					p_IP->ScaleUP();
					break;
				case KEY_S:
					p_IP->ScaleDown();
					break;
				case KEY_D:
					p_IP->TurnCounterClockwise();
					break;
				case KEY_F:
					p_IP->TurnClockwise();
					break;
				case KEY_I:
					p_IP->ResetValues();
					break;
				default:
					break;
				}
				if (InputSmoother(key = waitKey(10))) continue;
				p_IP->VisualizePreview(img_dst, img_added);
				key = waitKey(0);
			}
		}
		p_IP->UpdateImage(img_dst, img_added);
		p_IP->UpdateRegionsData(*data_dst, data_added, img_dst, img_added);
	}
	Finalize(showResult);
	return true;
}

void AnnotateDataCSVMode::ShowMenu() {
	Mat img_menu = imread("system_images/AnnotateData_EditData.png", IMREAD_COLOR);
	imshow("Menu", img_menu);
}