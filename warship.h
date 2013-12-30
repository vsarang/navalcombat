// abstract class for defining all warships
#ifndef WARSHIP_H
#define WARSHIP_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <string>

class Warship {
	public:
		class Munition {
			public:
				int type;
				int damage;
		};

		class Weapon {
			public:
				int range;
				int rounds;
				int volleysize;
				Munition munition;
		};

		Warship(const Warship & other);
		virtual Warship & operator=(const Warship & other);
		virtual ~Warship();
		virtual void copy(const Warship & other);
		virtual void clear();
		virtual void hurt(int type, int damage);
		virtual void fireWeapon(int weapon, Warship target);
		virtual int getRange(int weapon) const;
		void setRotation(int rot);	

	private:
		int rotation;
		int maxhealth;
		int health;
		int maxarmor;
		int armor;
		int weaponcount;
		SDL_Surface* spritesheet;
		Weapon* weapons;
};
#endif
