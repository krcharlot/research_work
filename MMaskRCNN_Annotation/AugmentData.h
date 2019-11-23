#pragma once
#include "EditData_IP.h"
#include "KeyboardInput.h"

class AugmentData : public EditData_IP {
public:
	AugmentData(const string& dir_read, const string& dir_save);
	~AugmentData();
	virtual bool Editing() override = 0;

protected:
	void Initialize(string id) override final;
	virtual void ShowMenu() = 0;
};

class AugmentDataInputMode : public AugmentData, public KeyboardInput{
public:
	AugmentDataInputMode(const string& dir_read, const string& dir_save);
	~AugmentDataInputMode();
	bool Editing() override;
private:
	void ShowMenu() override;
};

class AugmentDataCSVMode : public AugmentData, public KeyboardInput {
public:
	AugmentDataCSVMode(const string& dir_read, const string& dir_save, bool correctMode, bool showResult = false);
	~AugmentDataCSVMode();
	void ReadParametersRatioMode(const double pos_x, const double pos_y, const int ang_deg, const double scale);
	void ReadParametersPixelMode(const int pos_x, const int pos_y, const int ang_deg, const double scale);
	bool Editing() override;
private:
	bool isCorrectMode;
	int pos_x;
	int pos_y;
	int ang_deg;
	double scale;
	bool showResult;
	void ShowMenu() override;
};
