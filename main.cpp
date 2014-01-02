#include <iostream>
#include "battlefield.h"
#include "battlefieldgui.h"
#include "warship.h"

int main (int argc, char* args[]) {
	Battlefield b(100, 100);
	b.generateMap();
	b.printMap();
}
