#include <iostream>
#include "battlefield.h"
#include "battlefieldgui.h"
#include "warship.h"

int main (int argc, char* args[]) {
	Battlefield b(80, 35);
	b.generateMap();
	b.printMap();
}
