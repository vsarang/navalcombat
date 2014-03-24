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
		Battlefield* battlefield;

		BattlefieldGUI(Battlefield* b);
		~BattlefieldGUI();
		SDL_Surface* load_image(std::string filename);
 		void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* dest);
		int run();
		void moveCamera(int x, int y);
	private:
		static const int FRAMES_PER_SECOND = 120;
        static const int TILE_SIZE = 20;
        static const int SIDEBAR_WIDTH = 150;
        static const Uint32 SIDEBAR_BORDER_COLOR = 0x777777;
        static const Uint32 SIDEBAR_BACKGROUND_COLOR = 0x222222;
        static const Uint32 TEXT_COLOR = 0xFFFFFFFF;

		int screen_width;
		int screen_height;
		int screen_bpp;
		int scroll_speed;
		std::string frame_title;
		Timer fps;
		SDL_Surface* screen;
        TTF_Font* font;
        bool warship_selected;

		SDL_Rect clip[4];
		static const int num_tiles = 5;
		SDL_Surface* tiles[num_tiles];
		static const int WATER_FULL = 0;
		static const int LAND_FULL = 1;
		static const int TILE_BLACK = 2;
		static const int WHITE_BORDER = 3;
        static const int VALID_MOVE = 4;
        SDL_Surface* shipTiles[1];

 		bool load_files();
 		void clean_up();
 		bool init();
		void drawTile(int x, int y, int tile);
		void drawBorder(int startx, int starty, int endx, int endy);
		void leftClick(const SDL_Rect & coords);
		void rightClick(const SDL_Rect & coords);
		void drawMap();
		void drawWarships();
        void drawWarship(const Warship* ship);
        void drawSidebar();
        void drawMoveMask();
		SDL_Rect coordsToGrid(const SDL_Rect & coords);
};

 #endif
