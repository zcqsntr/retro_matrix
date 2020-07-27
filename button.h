#ifndef B_H
#define B_H
#include "mylib.h"


class Button {
	Point position;
	string label;
	bool selected;
	
	
	public:
		Button(Point p, string l, bool s);
		void run_target();
};


class MenuButton: public Button{
	vector<Button> targets_buttons;
	public:
		MenuButton(Point p, string l, bool s, vector<Button> tbs);
		
};


class ActionButton: public Button {
	
	
	public:
		ActionButton(Point p, string l, bool s, void (*tf()));
		
};

#endif
