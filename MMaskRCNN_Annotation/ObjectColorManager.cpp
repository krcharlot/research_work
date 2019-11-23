#include "ObjectColorManager.h"
#include <iostream>

COLOR& operator++ (COLOR& color) {
	if (color == PURPLE) {
		return color = RED;
	}
	color = static_cast<COLOR>(color + 1);
	return color;
}

ostream& operator<< (ostream& os, COLOR color) {
	string ret;
	switch (color)
	{
	case RED:
		ret = "RED";
		break;
	case ORANGE:
		ret = "ORANGE";
		break;
	case YELLOW:
		ret = "YELLOW";
		break;
	case LIGHTGREEN:
		ret = "LIGHTGREEN";
		break;
	case GREEN:
		ret = "GREEN";
		break;
	case LIGHTBLUE:
		ret = "LIGHTGREEN";
		break;
	case BLUE:
		ret = "BLUE";
		break;
	case PURPLE:
		ret = "PURPLE";
		break;
	default:
		break;
	}
	os << ret;
	return os;
}

ObjectColorManeger::ObjectColorManeger() : color_registered(GREEN) {
	color[RED] = cv::Scalar(0, 0, 255);
	color[ORANGE] = cv::Scalar(0, 125, 255);
	color[YELLOW] = cv::Scalar(0, 255, 255);
	color[LIGHTGREEN] = cv::Scalar(0, 255, 0);
	color[GREEN] = cv::Scalar(125, 255, 0);
	color[LIGHTBLUE] = cv::Scalar(255, 125, 0);
	color[BLUE] = cv::Scalar(255, 0, 0);
	color[PURPLE] = cv::Scalar(255, 0, 155);
}

COLOR ObjectColorManeger::AssignObjColor(const string& obj) {
	auto itr = object2color_table.find(obj);
	if (itr != object2color_table.end()) {
	}
	else {
		object2color_table[obj] = color_registered;
		++color_registered;
	}
	return object2color_table[obj];
}

cv::Scalar ObjectColorManeger::GetObjColor(const string& obj) {
	COLOR tmp_color = AssignObjColor(obj);
	return color[tmp_color];
}

void ObjectColorManeger::ShowTable() const {
	for (auto itr = object2color_table.begin(); itr != object2color_table.end(); itr++) {
		cout << "name:" << itr->first << " color:" << itr->second << endl;
	}
}