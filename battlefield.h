/**
 * Class defintion for the naval combat game board.
 * @file battlefield.h
 * @author Vikrant Sarang
 * @date January 2014
 */

#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "warship.h"
#include <string>

class Battlefield {
	public:
		/**
		 * Variables the primary eight directions for ease of use.
		 */
		static const int NORTH = 0;
		static const int NORTHEAST = 1;
		static const int EAST = 2;
		static const int SOUTHEAST = 3;
		static const int SOUTH = 4;
		static const int SOUTHWEST = 5;
		static const int WEST = 6;
		static const int NORTHWEST = 7;

		Battlefield(int w, int h);
		Battlefield(const Battlefield & other);
		~Battlefield();
		Battlefield & operator=(const Battlefield & other);
		void clear();
		void copy(const Battlefield & other);

	private:
		int** maparray;
		//SDL_Surface* map;
		//SDL_Surface* board;
		Warship*** warshipmap;
		int height;
		int width;
};

#endif
