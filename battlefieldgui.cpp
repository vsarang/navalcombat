/**
 * @file battlefieldgui.cpp
 * @author Vikrant Sarang
 * @date January 2014
 */

#include "battlefieldgui.h"

BattlefieldGUI::BattlefieldGUI(Battlefield* b) {
	battlefield = b;
	camera.x = -180;
	camera.y = 0;
	screen = NULL;
    font = NULL;
	screen_width = 1280;
	screen_height = 700;
	screen_bpp = 32;
	frame_title = "title";
	scroll_speed = 3;
    warship_selected = false;
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
	loadedImage = IMG_Load(filename.c_str());
	// if nothing went wrong
	if (loadedImage != NULL) {
		// create an optimized image
		optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
		SDL_FreeSurface(loadedImage);
		if (optimizedImage != NULL) {
			// map the color key
			//Uint32 colorkey = SDL_MapRGB(optimizedImage->format, 0xFE, 0xFE, 0xFE);
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
	tiles[LAND_FULL] = load_image("img/tiles/land_full.png");
	tiles[WATER_FULL] = load_image("img/tiles/water_full.png");
	tiles[TILE_BLACK] = load_image("img/tiles/black.png");
	tiles[WHITE_BORDER] = load_image("img/tiles/white_border.png");
    tiles[VALID_MOVE] = load_image("img/tiles/valid_move.png");
	for (int i = 0; i < num_tiles; i++) {
		if (tiles[i] == NULL) {
			return false;
		}
	}
    shipTiles[0] = load_image("img/warships/corvette.png");
    font = TTF_OpenFont("fonts/hooge0553.ttf", 20);

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
    if (TTF_Init() == -1) {
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
	int relx = grid_x*TILE_SIZE - camera.x;
	int rely = grid_y*TILE_SIZE - camera.y;
	apply_surface(relx, rely, tiles[tile], screen);
}

void BattlefieldGUI::drawMap() {
	// find the bounding rectangle of the camera
	int startx = (camera.x/TILE_SIZE)-1;
	int starty = (camera.y/TILE_SIZE)-1;
	int endx = (camera.x + screen_width)/TILE_SIZE;
	int endy = (camera.y + screen_height)/TILE_SIZE;

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

void BattlefieldGUI::drawSidebar() {
    SDL_Rect outline = {0, 0, SIDEBAR_WIDTH+2, screen_height};
    SDL_Rect bound = {0, 0, SIDEBAR_WIDTH, screen_height};
    SDL_FillRect(screen, &outline, SIDEBAR_BORDER_COLOR);
    SDL_FillRect(screen, &bound, SIDEBAR_BACKGROUND_COLOR);
    // draw menu options if there is a ship selected
    if (warship_selected) {
        SDL_Color textColor = {(Uint8)(TEXT_COLOR >> 24), (Uint8)(TEXT_COLOR >> 16), (Uint8)(TEXT_COLOR >> 8)};
        SDL_Surface* message = TTF_RenderText_Solid(font, "SIDEBAR", textColor);
        apply_surface(10, 100, message, screen);
    }
}

void BattlefieldGUI::drawWarships() {
    std::vector<Warship*> warships = battlefield->getWarshipList(0);
    for (int i = 0; i < warships.size(); i++) {
        drawWarship(warships[i]);
    }
    warships = battlefield->getWarshipList(1);
    for (int i = 0; i < warships.size(); i++) {
        drawWarship(warships[i]);
    }
}

void BattlefieldGUI::drawWarship(const Warship* ship) {
	int relx = ship->getLocation().x*TILE_SIZE - camera.x;
	int rely = ship->getLocation().y*TILE_SIZE - camera.y;
    apply_surface(relx, rely, shipTiles[ship->getWarshipType()], screen);
}

void BattlefieldGUI::drawMoveMask() {
    int** moveMask = battlefield->getMoveMask();
    for (int x = 0; x < battlefield->getWidth(); x++) {
        for (int y = 0; y < battlefield->getHeight(); y++) {
            if (moveMask[x][y] == 1) {
                drawTile(x, y, VALID_MOVE);
            }
        }
    }
}

int BattlefieldGUI::run() {
	drawMap();
	bool quit = false;
	bool scrollRight = false;
	bool scrollLeft = false;
	bool scrollUp = false;
	bool scrollDown = false;
    SDL_Rect mouse_relative;
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
                // get mouse position
                SDL_Rect grid_pos;
                int temp_x;
                int temp_y;
                SDL_GetMouseState(&temp_x, &temp_y);
                grid_pos.x = (temp_x + camera.x)/TILE_SIZE;
                grid_pos.y = (temp_y + camera.y)/TILE_SIZE;
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
                    case SDLK_s:
                        // spawn ship on team a
	                    if (grid_pos.x >= 0 && grid_pos.y >= 0 && grid_pos.x < battlefield->getWidth() && grid_pos.y < battlefield->getHeight()) {
                            battlefield->spawnShip(grid_pos, 0, 0);
                        }
                        break;
                    case SDLK_d:
                        // spawn ship on team b
	                    if (grid_pos.x >= 0 && grid_pos.y >= 0 && grid_pos.x < battlefield->getWidth() && grid_pos.y < battlefield->getHeight()) {
                            battlefield->spawnShip(grid_pos, 0, 1);
                        }
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
				//mouse_relative.x = event.motion.x + camera.x;
				//mouse_relative.y = event.motion.y + camera.y;
				//mouse_grid = coordsToGrid(mouse_relative);
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
        drawMoveMask();
		drawWarships();
		drawTile(mouse_grid.x, mouse_grid.y, WHITE_BORDER); // draw border for selected grid cell
        drawSidebar();
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
	grid_pos.x = coords.x/TILE_SIZE;
	grid_pos.y = coords.y/TILE_SIZE;
	if (grid_pos.x >= 0 && grid_pos.y >= 0 && grid_pos.x < battlefield->getWidth() && grid_pos.y < battlefield->getHeight()) {
        warship_selected = battlefield->select(grid_pos);
	}
}

void BattlefieldGUI::rightClick(const SDL_Rect & coords) {
}

SDL_Rect BattlefieldGUI::coordsToGrid(const SDL_Rect & coords) {
	SDL_Rect grid;
	grid.x = coords.x/TILE_SIZE;
	grid.y = coords.y/TILE_SIZE;
	return grid;
}
