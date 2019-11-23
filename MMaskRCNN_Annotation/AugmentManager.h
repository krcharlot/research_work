#pragma once
#include "TargetDataManager.h"
#include "AugmentData.h"
#include "CSVInput.h"

class AugmentManager : public TargetDataManager {
public:
	AugmentManager();
	~AugmentManager();
protected:
	string InputSameDirectory() const override;
};

class AugmentManagerInputMode : public AugmentManager {
public:
	AugmentManagerInputMode();
	~AugmentManagerInputMode();
private:
	AugmentDataInputMode* augment_data;
	void EditPlay() override;
};

class AugmentManagerCSVMode : public AugmentManager , public CSVInput {
public:
	AugmentManagerCSVMode(bool correctMode);
	~AugmentManagerCSVMode();
private:
	bool isCorrectMode;
	AugmentDataCSVMode* augment_data;
	void EditPlay() override;
};