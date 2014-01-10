/**
 * @file battlefieldgui.cpp
 * @author Vikrant Sarang
 * @date January 2014
 */

#include "battlefieldgui.h"

BattlefieldGUI::BattlefieldGUI(Battlefield* b) {
	battlefield = b;
	camera.x = 0;
	camera.y = 0;
	screen = NULL;
	screen_width = 640;
	screen_height = 480;
	screen_bpp = 32;
	frame_title = "title";
	scroll_speed = 3;
	selected_warship = NULL;
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
	tiles[COAST_DIAGONAL_SPRITESHEET] = load_image("img/tiles/coast_diagonal.png");
	tiles[COAST_PENINSULA_SPRITESHEET] = load_image("img/tiles/coast_peninsula.png");
	tiles[LAND_ISLAND] = load_image("img/tiles/coast_island.png");
	tiles[LAND_FULL] = load_image("img/tiles/land_full.png");
	tiles[WATER_FULL] = load_image("img/tiles/water_full.png");
	tiles[TILE_BLACK] = load_image("img/tiles/black.png");
	tiles[WHITE_BORDER] = load_image("img/tiles/white_border.png");
	for (int i = 0; i < num_tiles; i++) {
		if (tiles[i] == NULL) {
			return false;
		}
	}
	return true;
}

void BattlefieldGUI::clean_up() {
	for (int i = 0; i < num_tiles; i++) {
		SDL_FreeSurface(tiles[i]);
		tiles[i] = NULL;
	}
	SDL_FreeSurface(screen);
	screen = NULL;
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

void BattlefieldGUI::drawTile(int grid_x, int grid_y, int tile) {
	int relx = grid_x*20 - camera.x;
	int rely = grid_y*20 - camera.y;
	apply_surface(relx, rely, tiles[tile], screen);
}

void BattlefieldGUI::drawMap() {
	// find the bounding rectangle of the camera
	int startx = (camera.x/20)-1;
	int starty = (camera.y/20)-1;
	int endx = (camera.x + screen_width)/20;
	int endy = (camera.y + screen_height)/20;

	//std::cout << startx << " | " << camera.x << std::endl;

	int mapstartx = startx;
	int mapstarty = starty;
	// draw black to the left and top of the map
	for (; mapstartx < 0; mapstartx++) {
		for (int y = starty; y <= endy; y++) {
			drawTile(mapstartx, y, TILE_BLACK);
		}
	}
	for(; mapstarty < 0; mapstarty++) {
		for (int x = startx; x <= endx; x++) {
			drawTile(x, mapstarty, TILE_BLACK);
		}
	}
	// draw the map
	for (int x = mapstartx; x <= endx && x < battlefield->getWidth(); x++) {
		for (int y = mapstarty; y <= endy && y < battlefield->getHeight(); y++) {
			if (battlefield->getCell(x, y) == 1) {
				drawTile(x, y, LAND_FULL);
			} else if (battlefield->getCell(x, y) == 0) {
				drawTile(x, y, WATER_FULL);
			}
		}
	}
	// draw black to the right and below the map
	for (int x = battlefield->getWidth(); x <= endx; x++) {
		for (int y = starty; y <= endy; y++) {
			drawTile(x, y, TILE_BLACK);
		}
	}
	for (int y = battlefield->getHeight(); y <= endy; y++) {
		for (int x = startx; x <= endx; x++) {
			drawTile(x, y, TILE_BLACK);
		}
	}
}

int BattlefieldGUI::run() {
	//moveCamera(1,1);
	drawMap();
	bool quit = false;
	bool scrollRight = false;
	bool scrollLeft = false;
	bool scrollUp = false;
	bool scrollDown = false;
	SDL_Rect mouse_grid;
	while (quit == false) {
		// start frame timer
		fps.start();
		// process events
		while(SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
			// process key presses
			if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
				switch (event.key.keysym.sym) {
					case SDLK_RIGHT:
						scrollRight = (event.type == SDL_KEYDOWN);
						break;
					case SDLK_LEFT:
						scrollLeft = (event.type == SDL_KEYDOWN);
						break;
					case SDLK_UP:
						scrollUp = (event.type == SDL_KEYDOWN);
						break;
					case SDLK_DOWN:
						scrollDown = (event.type == SDL_KEYDOWN);
						break;
				}
			}
			// process mouse clicks
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				SDL_Rect click_coords;
				click_coords.x = event.button.x + camera.x;
				click_coords.y = event.button.y + camera.y;
				if (event.button.button == SDL_BUTTON_LEFT) {
					leftClick(click_coords);
				}
				if (event.button.button == SDL_BUTTON_RIGHT) {
					rightClick(click_coords);
				}
			}
			// process mouse movement
			if (event.type == SDL_MOUSEMOTION) {
				SDL_Rect coords;
				SDL_Rect grid;
				coords.x = event.motion.x + camera.x;
				coords.y = event.motion.y + camera.y;
				mouse_grid = coordsToGrid(coords);
			}
		}

		// move the camera as needed
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
		drawMap();
		drawTile(mouse_grid.x, mouse_grid.y, WHITE_BORDER);
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

void BattlefieldGUI::leftClick(const SDL_Rect & coords) {
	SDL_Rect grid_pos;
	grid_pos.x = coords.x/20;
	grid_pos.y = coords.y/20;
	if (grid_pos.x >= 0 && grid_pos.y >= 0 && grid_pos.x < battlefield->getWidth() && grid_pos.y < battlefield->getHeight()) {
	}
}

void BattlefieldGUI::rightClick(const SDL_Rect & coords) {
	SDL_Rect grid_pos;
	grid_pos.x = coords.x/20;
	grid_pos.y = coords.y/20;
}

SDL_Rect BattlefieldGUI::coordsToGrid(const SDL_Rect & coords) {
	SDL_Rect grid;
	grid.x = coords.x/20;
	grid.y = coords.y/20;
	return grid;
}
