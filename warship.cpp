// implementation of the abstract class warship
#include "warship.h"

Warship::Warship(const Warship & other) {
	copy(other);
}

Warship & Warship::operator=(const Warship & other) {
	clear();
	copy(other);
	return *this;
}

Warship::~Warship() {
	clear();
}

void Warship::clear() {
	SDL_FreeSurface(spritesheet);
	delete [] weapons;
}

void Warship::copy(const Warship & other) {
	rotation = other.rotation;
	maxhealth = other.maxhealth;
	health = other.health;
	maxarmor = other.maxarmor;
	armor = other.armor;
	spritesheet = SDL_ConvertSurface(other.spritesheet, other.spritesheet->format, other.spritesheet->flags);
	for (int i = 0; i < weaponcount; i++) {
		weapons[i] = other.weapons[i];
	}
}

void Warship::hurt(int type, int damage) {
	switch(type) {
		case 0:
			health -= damage;
			break;
		case 1:
			health -= damage/2;
			armor -= damage/2;
			break;
	}
	if (health <= 0) {
		sunk = true;
	}
}

void Warship::fireWeapon(int weaponindex, Warship target) {
	Weapon weapon = weapons[weaponindex];
	// if there are enough rounds to fire, hurt the target and update rounds
	if (weapon.rounds >= weapon.volleysize) {
		weapon.rounds -= weapon.volleysize;
		target.hurt(weapon.munition.type, weapon.munition.damage*weapon.volleysize);
	}
	// otherwise, fire the remaining rounds and set rounds to zero 
	else {
		target.hurt(weapon.munition.type, weapon.munition.damage*weapon.rounds);
		weapon.rounds = 0;
	}
}

int Warship::getRange(int weaponindex) const {
	return weapons[weaponindex].range;
}

void Warship::setRotation(int rot) {
	rotation = rot;
}
