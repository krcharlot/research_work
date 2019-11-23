#include "AugmentData.h"
#include "Keybord.h"
#include "MainSystem.h"
#include <iomanip>

AugmentData::AugmentData(const string& d_read, const string& d_save) {
	num++;
	this->dir_tar = d_read;
	this->dir_save = d_save;
}

AugmentData::~AugmentData() {
	delete data_tar;
	delete data_dst;
}

void AugmentData::Initialize(string id) {

	img_dst = Mat(Size(img_tar.cols, img_tar.rows), CV_8UC3, Scalar(255, 255, 255));

	stringstream ss;
	ss << setfill('0') << setw(FILE_NUM_DIGIT) << right << to_string(num);
	data_dst = new ImageData(dir_save, ss.str() + id, ss.str() + id, *data_tar);
	for (size_t i = 0; i < data_dst->regions.size(); i++) {
		delete data_dst->regions[i];
	}
}

AugmentDataInputMode::AugmentDataInputMode(const string& d_read, const string& d_save) : AugmentData(d_read, d_save) {}
AugmentDataInputMode::~AugmentDataInputMode() {}

bool AugmentDataInputMode::Editing() {
	Initialize("Ag");
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
		case KEY_DELETE: {
			destroyAllWindows();
			Skip();
			return false;
		}
			break;
		default:
			break;
		}
		if (InputSmoother(key = waitKey(10))) continue;
		p_IP->VisualizePreview(img_dst, img_tar);
		key = waitKey(0);
	}
	p_IP->UpdateImage(img_dst, img_tar);
	p_IP->UpdateRegionsData(*data_dst, *data_tar, img_dst, img_tar);
	destroyAllWindows();

	Finalize();
	return true;
}

void AugmentDataInputMode::ShowMenu() {
	Mat img_menu = imread("system_images/AugmentData_EditData_Menu_Input.png", IMREAD_COLOR);
	imshow("Menu", img_menu);
}

AugmentDataCSVMode::AugmentDataCSVMode(const string& d_read, const string& d_save, bool correctMode, bool sResult) : AugmentData(d_read, d_save), isCorrectMode(correctMode), showResult(sResult) {}
AugmentDataCSVMode::~AugmentDataCSVMode() {}

void AugmentDataCSVMode::ReadParametersRatioMode(const double pos_x_ratio, const double pos_y_ratio, const int ang_deg, const double scale) {

	int pos_x_pixel = static_cast<int>(img_tar.cols * pos_x_ratio);
	int pos_y_pixel = static_cast<int>(img_tar.rows * pos_y_ratio);

	this->pos_x = pos_x_pixel;
	this->pos_y = pos_y_pixel;
	this->ang_deg = ang_deg;
	this->scale = scale;
}

void AugmentDataCSVMode::ReadParametersPixelMode(const int pos_x, const int pos_y, const int ang_deg, const double scale) {
	this->pos_x = pos_x;
	this->pos_y = pos_y;
	this->ang_deg = ang_deg;
	this->scale = scale;
}

bool AugmentDataCSVMode::Editing() {
	Initialize("Ag");
	p_IP = new ImagePasting();
	p_IP->InputValues(pos_x, pos_y, ang_deg, scale);

	if (isCorrectMode) {
		ShowMenu();
		int key = -1;
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
			p_IP->VisualizePreview(img_dst, img_tar);
			key = waitKey(0);
		}
	}

	p_IP->UpdateImage(img_dst, img_tar);
	p_IP->UpdateRegionsData(*data_dst, *data_tar, img_dst, img_tar);
	Finalize(showResult);
	return true;
}

void AugmentDataCSVMode::ShowMenu() {
	Mat img_menu = imread("system_images/AugmentData_EditData_Menu_Input.png", IMREAD_COLOR);
	imshow("Menu", img_menu);
}