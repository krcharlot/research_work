#pragma once
#include "EditData.h"
#include "JsonData.h"
#include "PointsReinforcement.h"

class ReinforceData : public EditData {
public:
	ReinforceData(const string& dir_read, const string& dir_save);
	~ReinforceData();
	void SetImageData(JsonData& data, size_t itr);
	bool Editing() override;

private:
	string dir_read;
	string dir_save;
	ImageData* data_ori;

	PointsReinforcement* p_PR;
	
	void Initialize(string id) override;
	void Finalize(bool showResult = true) override;
	void ShowMenu();
};