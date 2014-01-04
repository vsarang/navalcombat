/**
 * The header class for everything related to the GUI of naval combat.
 * @file battlefieldgui.h
 * @author Vikrant Sarang
 * @date January 2014
 */

#ifndef BATTLEFIELDGUI_H
#define BATTLEFIELDGUI_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "warship.h"
#include "battlefield.h"
#include "timer.h"
#include <iostream>
#include <string>

class BattlefieldGUI {
	public:
		SDL_Rect camera;
		SDL_Event event;

		BattlefieldGUI();
		~BattlefieldGUI();
		SDL_Surface* load_image(std::string filename);
 		void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* dest);
		void drawMap(const Battlefield & b);
		int run(const Battlefield & b);
		void moveCamera(int x, int y);
	private:
		static const int FRAMES_PER_SECOND = 30;

		int screen_width;
		int screen_height;
		int screen_bpp;
		std::string frame_title;
		Timer fps;
		int scroll_speed;
		SDL_Surface* screen;
		SDL_Surface* map;

		SDL_Surface* coast_diagonal_spritesheet;
		SDL_Surface* coast_peninsula_spritesheet;
		SDL_Surface* land_full;
		SDL_Surface* water_full;
		SDL_Surface* coast_island;
		SDL_Surface* tile_black;

 		bool load_files();
 		void clean_up();
 		bool init();
		void drawTile(int x, int y, SDL_Surface* tile);
		void drawBorder(int startx, int starty, int endx, int endy);
};

#endif
