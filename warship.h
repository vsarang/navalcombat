/**
 * Broad definition of warships to be used to implement every boat type.
 * @file warship.h
 * @author Vikrant Sarang
 * @date January 2014
 */
#ifndef WARSHIP_H
#define WARSHIP_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <string>

class Warship {
	public:
		/**
		 * Simple structure to hold ammunition data.
		 */
		struct Munition {
			public:
				int type;
				int damage;
		};
		/**
		 * Simple structure to hold weapon data and firing capabilities.
		 */
		struct Weapon {
			public:
				int range;
				int rounds;
				int volleysize;
				Munition munition;
		};

		Warship();
		Warship(const Warship & other);
		virtual Warship & operator=(const Warship & other);
		virtual ~Warship();
		/**
		 * Applies damage to the warship's armor and health based on the type of ammunition fired.
		 * type 0 - Standard round, inflicts damage to armor until armor is reduced to zero, then inflicts damage to health.
		 * type 1 - Armor piercing round, inflicts 50% of damage to armor, and 50% of damage to health.
		 *          If no armor remains, inflicts 75% of damage to health.
		 * Updates the boolean value sunk if the damage was fatal.
		 * @param int type The type of ammunition fired.
		 * @param int damage The total damage to be inflicted.
		 */
		virtual void hurt(int type, int damage);
		/**
		 * Calls target.hurt() with the damage inflicted from one volley of the selected weapon.
		 * Reduces the remaining rounds in the selected weapon.
		 * @param int weaponindex The index of the weapon to be fired.
		 * @param const Warship & target The ship to fire upon.
		 */
		virtual void fireWeapon(int weaponindex, Warship & target);
		virtual int getRange(int weaponindex) const;
		/**
		 * Sets the rotation of the ship. If an invalid parameter is given, the rotation is unchanged.
		 * @param int rot The target rotation between 0 and 3.
		 */
		void setRotation(int rot);
		void setLocation(const SDL_Rect & coords);
		int getWarshipType() const;
		int getRotation() const;
        int getMoverange() const;
		SDL_Rect getLocation() const;

	protected:
		bool sunk;
		/**
		 * The current rotation of the ship from 0 to 3.
		 * 0 North
		 * 1 East
		 * 2 South
		 * 3 West
		 */
		int rotation;
		int maxhealth;
		int health;
		int maxarmor;
		int armor;
		int weaponcount;
		int warshiptype;
        int moverange;
		SDL_Rect location;
		Weapon* weapons;

		virtual void copy(const Warship & other);
		virtual void clear();
};
#endif
