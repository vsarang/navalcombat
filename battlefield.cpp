/**
 * @file battlefield.cpp
 * @author Vikrant Sarang
 * @date January 2014
 */

#include "battlefield.h"

Battlefield::Battlefield(int w, int h) {
	width = w;
	height = h;

	// initialize maparray to all zeroes and warshipmap to all null pointers
	maparray = new int*[w];
	warshipmap = new Warship**[w];
	for (int i = 0; i < w; i++) {
		maparray[i] = new int[h];
		warshipmap[i] = new Warship*[h];
		for (int j = 0; j < h; j++) {
			warshipmap[i][j] = NULL;
			maparray[i][j] = 0;
		}
	}

	map = NULL;
	board = NULL;	
}

Battlefield::Battlefield(const Battlefield & other) {
	clear();
	copy(other);
}

Battlefield::~Battlefield() {
	clear();
}

Battlefield & Battlefield::operator=(const Battlefield & other) {
	clear();
	copy(other);
	return *this;
}

void Battlefield::clear() {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			// free every warship on the warshipmap
			delete warshipmap[i][j];
			warshipmap[i][j] = NULL;
		}
		// free every column in both warshipmap and maparray
		delete [] maparray[i];
		delete [] warshipmap[i];
		maparray[i] = NULL;
		warshipmap[i] = NULL;
	}
	// free both arrays pointers as well
	delete [] maparray;
	delete [] warshipmap;
	maparray = NULL;
	warshipmap = NULL;

	// free both SDL surfaces
	SDL_FreeSurface(map);
	SDL_FreeSurface(board);
}

void Battlefield::copy(const Battlefield & other) {
	// copy primitives
	height = other.height;
	width = other.width;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			// copy every warship on the warship map
			warshipmap[i][j] = new Warship(*(other.warshipmap[i][j]));
			// copy battlefield map cells
			maparray[i][j] = other.maparray[i][j];
		}
	}

	// copy the SDL surfaces
	map = SDL_ConvertSurface(other.map, other.map->format, other.map->flags);
	board = SDL_ConvertSurface(other.board, other.board->format, other.board->flags);
}
