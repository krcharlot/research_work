#pragma once
#include "JsonData.h"
#include "ImageData.h"

class MMRCNNManager {
public:
	MMRCNNManager();
	~MMRCNNManager();
	void MainProcess();
	string GetSaveDirName();

protected:
	string dir_tar;
	string dir_save;
	picojson::object json_dst;

	virtual void ReadData() = 0;
	virtual void SelectTargetDirectory();
	virtual void InputTargetDirectory();
	virtual string InputSameDirectory() const = 0;
	virtual void EditPlay() = 0;
	bool EditSameImage();
	void ExportJsonFile();

private:
};