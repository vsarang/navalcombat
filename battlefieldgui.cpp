/**
 * @file battlefieldgui.cpp
 * @author Vikrant Sarang
 * @date January 2014
 */

#include "battlefieldgui.h"

BattlefieldGUI::BattlefieldGUI() {
	screen = NULL;
	init();
	load_files();
}

SDL_Surface * BattlefieldGUI::load_image(std::string filename) {
	// temporary storage for the image that's loaded
	SDL_Surface * loadedImage = NULL;

	// the optimized image that will be used
	SDL_Surface * optimizedImage = NULL;

	// load the image
	loadedImage = IMG_Load(filename.c_str())
;
	// if nothing went wrong
	if (loadedImage != NULL) {

		// create an optimized image
		optimizedImage = SDL_DisplayFormat(loadedImage);

		SDL_FreeSurface(loadedImage);

		if (optimizedImage != NULL) {
			// map the color key
			Uint32 colorkey = SDL_MapRGB(optimizedImage->format, 0, 0xFF, 0xFF);

			SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, colorkey);
		}
	}

	// return the optimized image
	return optimizedImage;
}

void BattlefieldGUI::apply_surface(int x, int y, SDL_Surface * source, SDL_Surface * destination) {
	// make a temporary rectangle to hold the offsets
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;

	// blit the surface
	SDL_BlitSurface(source, NULL, destination, &offset);
}

bool BattlefieldGUI::load_files() {
	return true;
}

void BattlefieldGUI::clean_up() {
	SDL_Quit();
}

bool BattlefieldGUI::init() {
	// initialize all SDL subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		return false;
	}
	// set up screen
	screen = SDL_SetVideoMode(screen_width, screen_height, screen_bpp, SDL_SWSURFACE);
	if (screen == NULL) {
		return false;
	}
	// set the window caption
	SDL_WM_SetCaption(frame_title.c_str(), NULL);

	return true;
}
