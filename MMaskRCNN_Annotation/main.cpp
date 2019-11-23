#include "MainSystem.h"
#include "Managers.h"

bool isDebug = true;
bool isDebugClassManegement = false;
const int FILE_NUM_DIGIT = 4;
std::string start_up_time = GetTimeInfomation();

bool MainLoop() {

	cout << "Do you want to continue?" << endl;

	char flag;
	for (int i = 0; i < 1; i++) {
		cout << "(y/n):";
		cin >> flag;
		switch (flag)
		{
		case 'y':
			return true;
		case 'n':
			return false;
		default:
			i--;
			break;
		}
	}
}

int main() {	
	
	bool loop = true;

	while (loop) {
		
		MMRCNNManager* mmrcnn = ModeSelect();
		mmrcnn->MainProcess();
		delete mmrcnn;

		loop = MainLoop();
	}
	return 0;
}