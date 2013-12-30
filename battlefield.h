#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "warship.h"
#include <string>

class Battlefield {
	public:

	private:
		int* maparray;
		SDL_Surface* map;
		Warship** warshipmap;
		int height;
		int width;
};

#endif
