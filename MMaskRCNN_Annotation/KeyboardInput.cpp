#include "KeyboardInput.h"
#include <opencv2/opencv.hpp>
using namespace cv;

KeyboardInput::KeyboardInput() {}
KeyboardInput::~KeyboardInput() {}

bool KeyboardInput::InputSmoother(int key) {
	//if (InputSmoother(key = waitKey(10))) continue;�ɂ��
	//Editing()�̃��[�v����cv::waitKey(ms)�ł̘A�����͂��\�ɂ���(�������΂��ăp�����[�^�X�V�݂̂��s��)
	//�摜�����̌v�Z���ǂ��t���Ȃ����߁AFINISH��̊Ԃ̓p�����[�^�݂̂��X�V
	static unsigned int count = 0;
	static const unsigned int START = 0;
	static const unsigned int FINISH = 2;
	if (key != -1 && count < FINISH) return true;
	else {
		count = START;
		return false;
	}
}