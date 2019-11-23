#pragma once
#include "EditData.h"
#include "JsonData.h"
#include "ImagePasting.h"
#include <opencv2/opencv.hpp>
using namespace cv;

class EditData_IP : public EditData {
public:
	EditData_IP();
	~EditData_IP();
	virtual void SetTargetImage(JsonData& data, size_t itr);
	virtual bool Editing() override = 0;

protected:
	string dir_tar;
	string dir_save;
	ImageData* data_tar;
	Mat img_tar;
	Mat img_dst;

	ImagePasting* p_IP;

	virtual void Initialize(string id) override = 0;
	void Finalize(bool showResult = true) override final;
	void Skip();

private:
};