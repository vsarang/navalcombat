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
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string>

class Battlefield {
	public:
		/**
		 * Static variables for the primary eight directions for ease of use.
		 */
		static const int NORTH = 0;
		static const int NORTHEAST = 1;
		static const int EAST = 2;
		static const int SOUTHEAST = 3;
		static const int SOUTH = 4;
		static const int SOUTHWEST = 5;
		static const int WEST = 6;
		static const int NORTHWEST = 7;

		SDL_Surface* map;
		SDL_Surface* board;

		Battlefield(int w, int h);
		Battlefield(const Battlefield & other);
		~Battlefield();
		Battlefield & operator=(const Battlefield & other);
		void clear();
		void copy(const Battlefield & other);
		void generateMap();
		void printMap() const;
		int** getMapArray() const;
		int getWidth() const;
		int getHeight() const;
		int getCell(int x, int y) const;

	private:
		int** maparray;
		Warship*** warshipmap;
		int height;
		int width;

		void generateIsland(int x, int y, int prevX, int prevY, int iterations);
		bool shouldGrow(int iterations) const;
		bool validLocation(int x, int y) const;
};

#endif
