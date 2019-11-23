#pragma once
#include "ImageData.h"
#include <opencv2/opencv.hpp>
using namespace cv;

class PointsReinforcement {
public:
	PointsReinforcement();
	~PointsReinforcement();
	void IncreasePitch();
	void DecreasePitch();
	void ResetPitch();
	void ShowPitch();
	void VisualizePreview(const ImageData& data);
	void UpdataRegionsData(ImageData& data_dst, const ImageData& data_ori);

private:
	static const double RATE_CHANGE_PITCH;
	static const double PITCH_MAX;
	static const double PITCH_MIN;
	static const double PITCH_RESET;
	static double pitch;

	enum FLUCTUATION {
		X_INCREASE_Y_INCREASE,
		X_INCREASE_Y_DECREASE,
		X_DECREASE_Y_INCREASE,
		X_DECREASE_Y_DECREASE,
	};
	FLUCTUATION CheckPointsBoundFor(const Point2i& p_pre, const Point2i& p_next);
	void InprintMiddlePoints(ImageData& data_dst, const ImageData& data_ori);
};