#include "ReinforceData.h"
#include "Keybord.h"

ReinforceData::ReinforceData(const string& d_read, const string& d_save) {
	num++;
	this->dir_read = d_read;
	this->dir_save = d_save;
}

ReinforceData::~ReinforceData() {
	delete data_ori;
	delete data_dst;
}

void ReinforceData::SetImageData(JsonData& d, size_t itr) {
	//itr:0~num_img-1
	picojson::object obj_img = d.GetObjImg(itr);
	data_ori = new ImageData(obj_img, this->dir_read);
}

bool ReinforceData::Editing() {
	
	Initialize("RE");

	ShowMenu();

	int key = -1;
	p_PR = new PointsReinforcement();

	while (key != KEY_ENTER) {
		switch (key)
		{
		case KEY_UP:
			p_PR->IncreasePitch();
			break;
		case KEY_DOWN:
			p_PR->DecreasePitch();
			break;
		case KEY_DELETE:
			p_PR->ResetPitch();
		default:
			break;
		}
		p_PR->ShowPitch();
		p_PR->VisualizePreview(*data_ori);
		key = waitKey(0);
	}
	p_PR->UpdataRegionsData(*data_dst, *data_ori);
	delete p_PR;
	destroyAllWindows();
	
	Finalize();
	return true;
}

//id:JsonÇ…í«â¡Ç≈ãLèqÇ≥ÇÍÇÈï∂éöóÒ
void ReinforceData::Initialize(string id) {
	data_dst = new ImageData(dir_save, id, id, *data_ori);
	for (size_t i = 0; i < data_dst->regions.size(); i++) {
		delete data_dst->regions[i];
	}
}
void ReinforceData::Finalize(bool showResult) {
	imwrite(dir_save + data_dst->image_file_name, imread(data_ori->image_dir + data_ori->image_file_name, IMREAD_COLOR));
	if (showResult) {
		data_dst->ShowInfo();
		data_dst->ShowRegions();
		Mat img_guide = imread("system_images/finalize_sign.png", IMREAD_COLOR);
		imshow("Guide", img_guide);
		waitKey(0);
		destroyAllWindows();
	}
}

void ReinforceData::ShowMenu() {
	const string reinforce_menu_path = "system_images/ReinforceData_EditData_Menu.png";
	Mat img_menu = imread(reinforce_menu_path, IMREAD_COLOR);
	imshow("Menu", img_menu);
}