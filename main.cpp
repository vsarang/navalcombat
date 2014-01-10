#include <iostream>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "battlefield.h"
#include "battlefieldgui.h"
#include "warship.h"

int main (int argc, char* args[]) {
	Battlefield b(50, 50);
	b.generateMap();
	//b.printMap();
	BattlefieldGUI bGUI = BattlefieldGUI(&b);
	bGUI.run();
}
