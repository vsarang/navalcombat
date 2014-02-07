#include "warship_corvette.h"

Warship_Corvette::Warship_Corvette() {
	maxhealth = 100;
	health = maxhealth;
	maxarmor = 10;
	armor = maxarmor;
	warshiptype = 0;

	weapons = new Weapon[1];
	weapons[1].range = 3;
	weapons[1].rounds = 300;
	weapons[1].volleysize = 30;
	weapons[1].munition.type = 0;
	weapons[1].munition.damage = 1;
}
