/**
 * @file warship.cpp
 * @author Vikrant Sarang
 * @date January 2014
 */
#include "warship.h"

Warship::Warship() {
	location.x = 0;
	location.y = 0;
	rotation = 0;
}

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
	delete [] weapons;
}

void Warship::copy(const Warship & other) {
	rotation = other.rotation;
	maxhealth = other.maxhealth;
	health = other.health;
	maxarmor = other.maxarmor;
	armor = other.armor;
	location = other.location;
	warshiptype = other.warshiptype;
	for (int i = 0; i < weaponcount; i++) {
		weapons[i] = other.weapons[i];
	}
}

void Warship::hurt(int type, int damage) {
	switch(type) {
		case 0:
			if (armor > 0) {
				armor -= damage;
			} else {
				health -= damage;
			}
			break;
		case 1:
			if (armor > 0) {
				health -= (int)(damage * 0.5);
				armor -= (int)(damage * 0.5);
			} else {
				health -= (int)(damage * 0.75);
			}
			break;
	}
	if (armor < 0) {
		armor = 0;
	}
	if (health < 0) {
		health = 0;
	}
	if (health <= 0) {
		sunk = true;
	}
}

void Warship::fireWeapon(int weaponindex, Warship & target) {
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
	if (rot >= 0 && rot <= 7) {
		rotation = rot;
	}
}

void Warship::setLocation(const SDL_Rect & coords) {
	location = coords;
}

SDL_Rect Warship::getLocation() const {
	return location;
}

int Warship::getRotation() const {
	return rotation;
}

int Warship::getWarshipType() const {
	return warshiptype;
}
