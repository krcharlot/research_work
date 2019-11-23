#pragma once
#include "ImageData.h"

class EditData {
public:
	EditData();
	~EditData();
	virtual bool Editing() = 0;
	void WriteToJson(picojson::object& json_obj);
	static void ResetDataNum();
protected:
	static int num;
	ImageData* data_dst;
	virtual void Initialize(string id) = 0;
	virtual void Finalize(bool showResult) = 0;
private:
	void ImportRegiontoRegions(picojson::array& regions);
};