#pragma once
#include "TargetDataManager.h"
#include "ReinforceData.h"

class ReinforceManager : public TargetDataManager {
public:
	ReinforceManager();
	~ReinforceManager();
private:
	ReinforceData* reinforce_data;
	string InputSameDirectory() const override;
	void EditPlay() override;
};