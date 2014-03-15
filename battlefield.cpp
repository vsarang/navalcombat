/**
 * @file battlefield.cpp
 * @author Vikrant Sarang
 * @date January 2014
 */

#include "battlefield.h"

Battlefield::Battlefield(int w, int h) {
	width = w;
	height = h;
    selected_warship = -1;

	// initialize maparray to all zeroes and warshipmap to all null pointers
	maparray = new int*[w];
	for (int x = 0; x < w; x++) {
		maparray[x] = new int[h];
		for (int y = 0; y < h; y++) {
			maparray[x][y] = 0;
		}
	}
	srand(time(NULL));
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
		// free every column in both warshipmap and maparray
		delete [] maparray[i];
		maparray[i] = NULL;
	}
    for (int i = 0; i < warships_a.size(); i++) {
        delete warships_a[i];
        warships_a[i] = NULL;
    }
    for (int i = 0; i < warships_b.size(); i++) {
        delete warships_b[i];
        warships_b[i] = NULL;
    }
	// free both arrays pointers as well
	delete [] maparray;
	maparray = NULL;
}

void Battlefield::copy(const Battlefield & other) {
	// copy primitives
	height = other.height;
	width = other.width;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			// copy battlefield map cells
			maparray[i][j] = other.maparray[i][j];
		}
	}
}

void Battlefield::generateMap() {
	int n = rand() % 10 + 3;
	for (int i = 0; i < n; i++) {
		int x = rand() % width;
		int y = rand() % height;
		generateIsland(x, y, x, y, 0);
	}
}

void Battlefield::generateIsland(int x, int y, int prevX, int prevY, int iterations) {
	// up
	if (shouldGrow(iterations)) {
		if (prevY != y-1 && validLocation(x, y-1) && maparray[x][y-1] == 0) {
			maparray[x][y-1] = 1;
			generateIsland(x, y-1, x, y, iterations+1);
		}
	}
	// down
	if (shouldGrow(iterations)) {
		if (prevY != y+1 && validLocation(x, y+1) && maparray[x][y+1] == 0) {
			maparray[x][y+1] = 1;
			generateIsland(x, y+1, x, y, iterations+1);
		}
	}
	// left
	if (shouldGrow(iterations)) {
		if (prevX != x-1 && validLocation(x-1, y) && maparray[x-1][y] == 0) {
			maparray[x-1][y] = 1;
			generateIsland(x-1, y, x, y, iterations+1);
		}
	}
	// right
	if (shouldGrow(iterations)) {
		if (prevX != x+1 && validLocation(x+1, y) && maparray[x+1][y] == 0) {
			maparray[x+1][y] = 1;
			generateIsland(x+1, y, x, y, iterations+1);
		}
	}
}

bool Battlefield::shouldGrow(int iterations) const {
	float scale = 0;
	if (width < height) {
		scale = 200.0/width;
	} else {
		scale = 200.0/height;
	}
	return ((float)(rand() % 100)) > iterations*scale;
}

bool Battlefield::validLocation(int x, int y) const {
	return (x >= 0 && y >= 0 && x < width && y < height);
}

void Battlefield::printMap() const {
	for (int i = 0; i < width; i++) {
		std::cout << "-";
	}
	std::cout << std::endl;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (maparray[x][y] == 1) {
				std::cout << "X";
			} else {
				std::cout << " ";
			}
		}
		std::cout << "|" << std::endl;
	}
	for (int i = 0; i < width; i++) {
		std::cout << "-";
	}
	std::cout << std::endl;
}

int** Battlefield::getMapArray() const {
	return maparray;
}

int Battlefield::getCell(int x, int y) const {
	if (x >= 0 && y >= 0 && x < width && y < height) {
		return maparray[x][y];
	} else {
		return -1;
	}
}

int Battlefield::getWidth() const {
	return width;
}

int Battlefield::getHeight() const {
	return height;
}

void Battlefield::addWarship(Warship* ship, size_t team) {
    if (team == 0)
        warships_a.push_back(ship);
    else
        warships_b.push_back(ship);
}

std::vector<Warship*> Battlefield::getWarshipList(size_t team) const {
	if (team == 0)
        return warships_a;
    else
        return warships_b;

}

void Battlefield::spawnShips() {
}

void Battlefield::spawnShip(SDL_Rect loc, size_t type, size_t team) {
    Warship_Corvette* temp = new Warship_Corvette();
    temp->setLocation(loc);
    addWarship(temp, team);
}

void Battlefield::select(SDL_Rect loc) {
    if (turn % 2 == 0) {
        for (int i = 0; i < warships_a.size(); i++) {
            SDL_Rect temp = warships_a[i]->getLocation();
            if (temp.x == loc.x && temp.y == loc.y) {
                selected_warship = i;
                return;
            }
        }
    } else {
        for (int i = 0; i < warships_b.size(); i++) {
            SDL_Rect temp = warships_b[i]->getLocation();
            if (temp.x == loc.x && temp.y == loc.y) {
                selected_warship = i;
                return;
            }
        }
    }
}
