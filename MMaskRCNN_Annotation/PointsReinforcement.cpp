#include "PointsReinforcement.h"

const double PointsReinforcement::RATE_CHANGE_PITCH(1.0);
const double PointsReinforcement::PITCH_MAX(400.0);
const double PointsReinforcement::PITCH_MIN(2.0);
const double PointsReinforcement::PITCH_RESET(15.0);
double PointsReinforcement::pitch = PointsReinforcement::PITCH_RESET;

PointsReinforcement::PointsReinforcement() {
	cout << endl << "--pitch--" << endl;
}
PointsReinforcement::~PointsReinforcement() {
	cout << "selected pitch:" << pitch << endl << endl;
}

void PointsReinforcement::IncreasePitch() {
	pitch += RATE_CHANGE_PITCH;
	pitch = (pitch < PITCH_MAX) ? pitch : PITCH_MAX;
}
void PointsReinforcement::DecreasePitch() {
	pitch -= RATE_CHANGE_PITCH;
	pitch = (pitch > PITCH_MIN) ? pitch : PITCH_MIN;
}
void PointsReinforcement::ResetPitch() {
	pitch = PITCH_RESET;
}

void PointsReinforcement::ShowPitch() {
	cout << pitch << endl;
}

PointsReinforcement::FLUCTUATION  PointsReinforcement::CheckPointsBoundFor(const Point2i& p_pre, const Point2i& p_next) {
	if (p_next.x - p_pre.x >= 0) {
		return (p_next.y - p_pre.y >= 0) ? X_INCREASE_Y_INCREASE : X_INCREASE_Y_DECREASE;
	}
	else {
		return (p_next.y - p_pre.y >= 0) ? X_DECREASE_Y_INCREASE : X_DECREASE_Y_DECREASE;
	}
}

void PointsReinforcement::VisualizePreview(const ImageData& data) {
	ImageData data_tmp(data.image_dir, data.image_ID, data.image_file_name, data.image_file_size);
	InprintMiddlePoints(data_tmp, data);
	data_tmp.ShowRegions();
}

void PointsReinforcement::UpdataRegionsData(ImageData& data_dst, const ImageData& data_ori) {
	InprintMiddlePoints(data_dst, data_ori);
}

void PointsReinforcement::InprintMiddlePoints(ImageData& data_dst, const ImageData& data_ori) {

	for (size_t itr_r = 0; itr_r < data_ori.regions.size(); itr_r++) {
		RegionData r_data = *data_ori.regions[itr_r];
		size_t num_points = r_data.all_point_x.size();
		vector<int> new_all_points_x;
		vector<int> new_all_points_y;

		for (size_t itr_p = 0; itr_p < num_points; itr_p++) {
			size_t itr_next = (itr_p == num_points - 1) ? 0 : itr_p + 1;
			Point2i point_cur(r_data.all_point_x[itr_p], r_data.all_point_y[itr_p]);
			Point2i point_nex(r_data.all_point_x[itr_next], r_data.all_point_y[itr_next]);
			new_all_points_x.push_back(point_cur.x);
			new_all_points_y.push_back(point_cur.y);
			FLUCTUATION  FLUC = CheckPointsBoundFor(point_cur, point_nex);
			double angle_rad = atan2(static_cast<double>(point_nex.y - point_cur.y), static_cast<double>(point_nex.x - point_cur.x));
			double new_point_x = point_cur.x + pitch * cos(angle_rad);//double‚ÅŒvŽZ‚µ‚È‚¢‚Æ“_‚ª‘å‚«‚­‚¸‚ê‚é
			double new_point_y = point_cur.y + pitch * sin(angle_rad);
			bool between2Points = true;
			while (between2Points) {
				switch (FLUC)
				{
				case X_INCREASE_Y_INCREASE:
					if (point_nex.x - new_point_x >= 0 && point_nex.y - new_point_y >= 0) {
						between2Points = true;
					}
					else {
						between2Points = false;
						continue;
					}
					break;
				case X_INCREASE_Y_DECREASE:
					if (point_nex.x - new_point_x >= 0 && point_nex.y - new_point_y < 0) {
						between2Points = true;
					}
					else {
						between2Points = false;
						continue;
					}
					break;
				case X_DECREASE_Y_INCREASE:
					if (point_nex.x - new_point_x < 0 && point_nex.y - new_point_y >= 0) {
						between2Points = true;
					}
					else {
						between2Points = false;
						continue;
					}
					break;
				case X_DECREASE_Y_DECREASE:
					if (point_nex.x - new_point_x < 0 && point_nex.y - new_point_y < 0) {
						between2Points = true;
					}
					else {
						between2Points = false;
						continue;
					}
					break;
				}
				new_all_points_x.push_back(static_cast<int>(new_point_x));
				new_all_points_y.push_back(static_cast<int>(new_point_y));
				new_point_x += pitch * cos(angle_rad);
				new_point_y += pitch * sin(angle_rad);
			}
		}
		data_dst.RegisterRegion(r_data.group_name, r_data.region_name, new_all_points_x, new_all_points_y);
	}
}