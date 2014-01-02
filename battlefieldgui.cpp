/**
 * @file battlefieldgui.cpp
 * @author Vikrant Sarang
 * @date January 2014
 */

#include "battlefieldgui.h"

BattlefieldGUI::BattlefieldGUI() {
	camera.x = 0;
	camera.y = 0;
	screen = NULL;
	screen_width = 640;
	screen_height = 480;
	screen_bpp = 32;
	frame_title = "title";
	if (!init()) {
		std::cout << "failed to initialize" << std::endl;
	}
	// load images
	if (!load_files()) {
		std::cout << "file error" << std::endl;
	}
}

BattlefieldGUI::~BattlefieldGUI() {
	clean_up();
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
			//Uint32 colorkey = SDL_MapRGB(optimizedImage->format, 0, 0xFF, 0xFF);
			//SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, colorkey);
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
	coast_diagonal_spritesheet = load_image("tiles/coast_diagonal.png");
	coast_peninsula_spritesheet = load_image("tiles/coast_peninsula.png");
	coast_island = load_image("tiles/coast_island.png");
	land_full = load_image("tiles/land_full.png");
	water_full = load_image("tiles/water_full.png");
	if (coast_diagonal_spritesheet == NULL ||
		coast_peninsula_spritesheet == NULL ||
		coast_island == NULL ||
		land_full == NULL ||
		water_full == NULL) {
		return false;
	}
	return true;
}

void BattlefieldGUI::clean_up() {
	SDL_FreeSurface(coast_diagonal_spritesheet);
	SDL_FreeSurface(coast_peninsula_spritesheet);
	SDL_FreeSurface(coast_island);
	SDL_FreeSurface(land_full);
	SDL_FreeSurface(water_full);
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

void BattlefieldGUI::drawMap(const Battlefield & b) {
	int** map_array = b.getMapArray();

	int startx = (int)(camera.x/20.0);
	int starty = (int)(camera.y/20.0);
	int endx = (int)((camera.x + screen_width)/20.0);
	int endy = (int)((camera.y + screen_height)/20.0);
	if (startx < 0) {
		startx = 0;
	}
	if (starty < 0) {
		starty = 0;
	}
	for (int x = startx; x < endx && x < b.getWidth(); x++) {
		for (int y = starty; y < endy && y < b.getHeight(); y++) {
			int relx = x*20 - camera.x;
			int rely = y*20 - camera.y;
			if (map_array[x][y] == 1) {
				apply_surface(relx, rely, land_full, screen);
			} else if (map_array[x][y] == 0) {
				apply_surface(relx, rely, water_full, screen);
			}
		}
	}

	SDL_Flip(screen);
}

void BattlefieldGUI::run(const Battlefield & b) {
	drawMap(b);
	apply_surface(0, 0, land_full, screen);
	bool quit = false;
	while (quit == false) {
		while(SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
		}
	}
}
