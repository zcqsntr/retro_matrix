
#include "button.h"
#include "retro_matrix.h"

Button::Button(Point p, string l, bool s){
			position = p;
			label = l;
			selected = s;
}
		



MenuButton::MenuButton(Point p, string l, bool s, vector<Button> tbs) {
			Button(p, l, s);
			targets_buttons = tbs;
}

MenuButton::run_target(){
	RetroMatrix::menu(targets_buttons);
}



ActionButton::ActionButton(Point p, string l, bool s, void (*tf())){
			Button(p, l, s);
			run_target = tf;
}
		



