#include "warship_corvette.h"

Warship_Corvette::Warship_Corvette() {
	maxhealth = 100;
	health = maxhealth;
	maxarmor = 10;
	armor = maxarmor;
	warshiptype = 0;

    weapons = new Weapon[1];
	weapons[0].range = 3;
	weapons[0].rounds = 300;
	weapons[0].volleysize = 30;
	weapons[0].munition.type = 0;
	weapons[0].munition.damage = 1;
}
