#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <map>

using namespace std;

enum COLOR {
	RED,
	ORANGE,
	YELLOW,
	LIGHTGREEN,
	GREEN,
	LIGHTBLUE,
	BLUE,
	PURPLE,
};
COLOR& operator ++ (COLOR& color);
ostream& operator<<(ostream& os, COLOR color);

class ObjectColorManeger {
private:
	static const int NUM_COLOR = 8;
	cv::Scalar color[NUM_COLOR];
	COLOR color_registered;
	map<string, COLOR> object2color_table;
	ObjectColorManeger();
	ObjectColorManeger(const ObjectColorManeger& obj) {}
	ObjectColorManeger& operator=(const ObjectColorManeger& obj) {}
protected:
	COLOR AssignObjColor(const string& obj);
public:
	static ObjectColorManeger* GetInstance() {
		static ObjectColorManeger instance;
		return &instance;
	}
	cv::Scalar GetObjColor(const string& obj);
	void ShowTable() const;
};