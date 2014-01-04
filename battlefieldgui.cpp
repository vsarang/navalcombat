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
	scroll_speed = 3;
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
	tile_black = load_image("tiles/black.png");
	if (coast_diagonal_spritesheet == NULL ||
			coast_peninsula_spritesheet == NULL ||
			coast_island == NULL ||
			land_full == NULL ||
			water_full == NULL ||
			tile_black == NULL) {
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
	SDL_FreeSurface(tile_black);
	SDL_Quit();
}

bool BattlefieldGUI::init() {
	// initialize all SDL subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		return false;
	}	
	// set up screen
	screen = SDL_SetVideoMode(screen_width,
					screen_height,
					screen_bpp,
					SDL_SWSURFACE);
	if (screen == NULL) {
		return false;
	}
	// set the window caption
	SDL_WM_SetCaption(frame_title.c_str(), NULL);

	return true;
}

void BattlefieldGUI::drawBorder(int startx, int starty, int endx, int endy) {
	for (int y = starty; y < 0; y++) {
		for (int x = startx; x < 0; x++) {

		}
		for (int x = screen_width/20.0; x < endx; x++) {

		}
	}

	for (int x = startx; x < 0; x++) {
		for (int y = starty; y < 0; y++) {

		}
		//for (int y = 
	}
}

void BattlefieldGUI::drawTile(int x, int y, SDL_Surface* tile) {
	int relx = x*20 - camera.x;
	int rely = y*20 - camera.y;
	apply_surface(relx, rely, tile, screen);
}

void BattlefieldGUI::drawMap(const Battlefield & b) {
	int** map_array = b.getMapArray();
	// find the bounding rectangle of the camera
	int startx = int (camera.x/20)-1;
	int starty = int (camera.y/20)-1;
	int endx = int ((camera.x + screen_width)/20);
	int endy = int ((camera.y + screen_height)/20);

	//std::cout << startx << " | " << camera.x << std::endl;

	int mapstartx = startx;
	int mapstarty = starty;
	// draw black to the left and top of the map
	for (; mapstartx < 0; mapstartx++) {
		for (int y = starty; y <= endy; y++) {
			drawTile(mapstartx, y, tile_black);
		}
	}
	for(; mapstarty < 0; mapstarty++) {
		for (int x = startx; x <= endx; x++) {
			drawTile(x, mapstarty, tile_black);
		}
	}
	// draw the map
	for (int x = mapstartx; x <= endx && x < b.getWidth(); x++) {
		for (int y = mapstarty; y <= endy && y < b.getHeight(); y++) {
			if (map_array[x][y] == 1) {
				drawTile(x, y, land_full);
			} else if (map_array[x][y] == 0) {
				drawTile(x, y, water_full);
			}
		}
	}
	// draw black to the right and below the map
	for (int x = b.getWidth(); x <= endx; x++) {
		for (int y = starty; y <= endy; y++) {
			drawTile(x, y, tile_black);
		}
	}
	for (int y = b.getHeight(); y <= endy; y++) {
		for (int x = startx; x <= endx; x++) {
			drawTile(x, y, tile_black);
		}
	}
}

int BattlefieldGUI::run(const Battlefield & b) {
	//moveCamera(1,1);
	drawMap(b);
	bool quit = false;
	bool scrollRight = false;
	bool scrollLeft = false;
	bool scrollUp = false;
	bool scrollDown = false;
	while (quit == false) {
		// start frame timer
		fps.start();
		// process events
		while(SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
			// process key presses
			switch (event.key.keysym.sym) {
				case SDLK_RIGHT:
					scrollRight = !scrollRight;
					break;
				case SDLK_LEFT:
					scrollLeft = !scrollLeft;
					break;
				case SDLK_UP:
					scrollUp = !scrollUp;
					break;
				case SDLK_DOWN:
					scrollDown = !scrollDown;
					break;
			}
		}
		if (scrollRight) {
			moveCamera(scroll_speed,0);
		}
		if (scrollLeft) {
			moveCamera(-1*scroll_speed,0);
		}
		if (scrollUp) {
			moveCamera(0,-1*scroll_speed);
		}
		if (scrollDown) {
			moveCamera(0,scroll_speed);
		}
		drawMap(b);
		if (SDL_Flip(screen) == -1) {
			return 1;
		}
		// cap framerate
		if (fps.get_ticks() < 1000/FRAMES_PER_SECOND) {
			SDL_Delay(1000/FRAMES_PER_SECOND - fps.get_ticks());
		}
	}
	return 0;
}

void BattlefieldGUI::moveCamera(int x, int y) {
	camera.x += x;
	camera.y += y;
}
