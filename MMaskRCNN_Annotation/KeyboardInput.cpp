#include "KeyboardInput.h"
#include <opencv2/opencv.hpp>
using namespace cv;

KeyboardInput::KeyboardInput() {}
KeyboardInput::~KeyboardInput() {}

bool KeyboardInput::InputSmoother(int key) {
	//if (InputSmoother(key = waitKey(10))) continue;により
	//Editing()のループ内のcv::waitKey(ms)での連続入力を可能にする(処理を飛ばしてパラメータ更新のみを行う)
	//画像処理の計算が追い付かないため、FINISH回の間はパラメータのみを更新
	static unsigned int count = 0;
	static const unsigned int START = 0;
	static const unsigned int FINISH = 2;
	if (key != -1 && count < FINISH) return true;
	else {
		count = START;
		return false;
	}
}