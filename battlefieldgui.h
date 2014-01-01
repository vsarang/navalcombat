/**
 * The header class for everything related to the GUI of naval combat.
 * @file battlefieldgui.h
 * @author Vikrant Sarang
 * @date January 2014
 */

#ifndef BATTLEFIELDGUI_H
#define BATTLEFIELDGUI_H

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "warship.h"
#include "battlefield.h"
#include <string>

class BattlefieldGUI {
	public:
		BattlefieldGUI();
		~BattlefieldGUI();
		SDL_Surface* load_image(std::string filename);
 		void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* dest);
	private:
		int screen_width;
		int screen_height;
		int screen_bpp;
		std::string frame_title;
		SDL_Surface* screen;

 		bool load_files();
 		void clean_up();
 		bool init();
};

#endif
