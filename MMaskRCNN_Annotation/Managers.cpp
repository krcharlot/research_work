#include "Managers.h"

enum EditMode{
	eNONE,
	Reinforce,
	Augment,
	Annotate,
};

enum InputMode {
	iNONE,
	KEY_INPUT,
	CSV,
	CORRECT,
};

MMRCNNManager* ModeSelect() {
	cout << "-----Select Mode-----" << endl;
	cout << "1:Reinforce 2:Augment 3:Annotate" << endl;
	while (true) {
		cout << "Input:";
		int emode_num;
		cin >> emode_num;

		EditMode edit_mode = static_cast<EditMode>(emode_num);
		switch (edit_mode)
		{
		case Reinforce:
			return new ReinforceManager();
		case Augment:
		{
			InputModeMenu();
			while (true) {
				cout << "Input:";
				int imode_num;
				cin >> imode_num;
				InputMode input_mode = static_cast<InputMode>(imode_num);
				switch (input_mode)
				{
				case KEY_INPUT:
					return new AugmentManagerInputMode();
				case CSV:
					return new AugmentManagerCSVMode(false);
				case CORRECT:
					return new AugmentManagerCSVMode(true);
				default:
					InputModeMsg();
					break;
				}
			}
		}
		case Annotate:
		{
			InputModeMenu();
			while (true) {
				cout << "Input:";
				int imode_num;
				cin >> imode_num;
				InputMode input_mode = static_cast<InputMode>(imode_num);
				switch (input_mode)
				{
				case KEY_INPUT:
					return new AnnotateManagerInputMode();
				case CSV:
					return new AnnotateManagerCSVMode(false);
				case CORRECT:
					return new AnnotateManagerCSVMode(true);
				default:
					InputModeMsg();
					break;
				}
			}
		}
		default:
			cout << "Please input 1:Reinforce or 2:Augment or 3:Annotate." << endl << endl;
			break;
		}
	}
}

void InputModeMenu() {
	cout << "1:KeyInput 2:CSV 3:Correct" << endl;
}
void InputModeMsg() {
	cout << "Please input 1:KeyInput or 2:CSV or 3: Correct." << endl << endl;
}