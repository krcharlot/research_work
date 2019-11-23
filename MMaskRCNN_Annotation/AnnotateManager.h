#pragma once
#include "MMRCNNManager.h"
#include "AnnotateData.h"
#include "CSVInput.h"

class AnnotateManager : public MMRCNNManager {
public:
	AnnotateManager();
	~AnnotateManager();

protected:
	JsonData json_data_input_target;
	vector <JsonData> v_json_data_input_added;
	vector<string> v_dir_added;

	void ReadData() override final;
	void ReadTargetData(const string& path);
	void ReadAddedData(const string& path);
	string InputSameDirectory() const override;
};

class AnnotateManagerInputMode : public AnnotateManager {
public:
	AnnotateManagerInputMode();
	~AnnotateManagerInputMode();
private:
	AnnotateDataInputMode* annotate_data;
	void EditPlay() override;
};

class AnnotateManagerCSVMode : public AnnotateManager, public CSVInput {
public:
	AnnotateManagerCSVMode(bool corectMode);
	~AnnotateManagerCSVMode();
private:
	bool isCorrectMode;
	string s_add_img_num;
	AnnotateDataCSVMode* annotate_data;
	void EditPlay() override;
	void ShowValues() const override;
};