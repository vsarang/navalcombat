#include "warship_corvette.h"

Warship_Corvette::Warship_Corvette() {
    rotation = 0;
	maxhealth = 100;
	health = maxhealth;
	maxarmor = 10;
	armor = maxarmor;
	warshiptype = 0;
    location.x = 0;
    location.y = 0;
    sunk = false;

    weaponcount = 1;
    weapons = new Weapon[weaponcount];
	weapons[0].range = 3;
	weapons[0].rounds = 300;
	weapons[0].volleysize = 30;
	weapons[0].munition.type = 0;
	weapons[0].munition.damage = 1;
}
