#pragma once
#include "EditData_IP.h"
#include "KeyboardInput.h"

class AnnotateData : public EditData_IP {
public:
	AnnotateData(const string& dir_tar, const vector<string>& v_dir_add, const string& dir_save);
	~AnnotateData();
	void SetAddedImages(vector<JsonData>& v_data_add);
	virtual bool Editing() override = 0;

protected:
	unsigned int num_added_imgs;
	vector<string> v_dir_added;
	vector<ImageData> v_data_added;
	vector<Mat> v_img_added;

	void Initialize(string id) override final;
	virtual void ShowMenu() = 0;
};

class AnnotateDataInputMode : public AnnotateData, public KeyboardInput {
public:
	AnnotateDataInputMode(const string& dir_tar, const vector<string>& v_dir_add, const string& dir_save);
	~AnnotateDataInputMode();
	bool Editing() override;
private:
	int SelectAddedImage();
	void ShowMenu() override;
};
class AnnotateDataCSVMode : public AnnotateData, public KeyboardInput {
public:
	AnnotateDataCSVMode(const string& dir_tar, const vector<string>& v_dir_add, const string& dir_save, bool correctMode , bool showResult = false);
	~AnnotateDataCSVMode();
	void ReadParametersRatioMode(const int add_img_itr, const double pos_x, const double pos_y, const int ang_deg, const double scale);
	void ReadParametersPixelMode(const int add_img_itr, const int pos_x, const int pos_y, const int ang_deg, const double scale);
	
	bool Editing() override;
private:
	bool isCorrectMode;
	vector<int> v_add_img_num;
	vector<int> v_pos_x;
	vector<int> v_pos_y;
	vector<int> v_ang_deg;
	vector<double> v_scale;
	bool showResult;
	void ShowMenu() override;
};