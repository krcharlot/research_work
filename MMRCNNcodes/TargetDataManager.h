#pragma once
#include "MMRCNNManager.h"

class TargetDataManager : public MMRCNNManager {
public:
	TargetDataManager();
	~TargetDataManager();

protected:
	JsonData json_data_input;

	virtual string InputSameDirectory() const override = 0;
	virtual void EditPlay() override = 0;

private:
	void ReadData() override final;
	void ReadTargetData(const string& path);
};