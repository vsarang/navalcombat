/**
 * @file battlefield.cpp
 * @author Vikrant Sarang
 * @date January 2014
 */

#include "battlefield.h"

Battlefield::Battlefield(int w, int h) {
	width = w;
	height = h;
    selected_warship = NULL;
    std::vector<Warship*> a;
    std::vector<Warship*> b;
    warships.push_back(a);
    warships.push_back(b);

	// initialize maparray to all zeroes and warshipmap to all null pointers
	maparray = new int*[w];
    movearray = new int*[w];
	for (int x = 0; x < w; x++) {
		maparray[x] = new int[h];
        movearray[x] = new int[h];
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
		// free every column in both movearray and maparray
		delete [] maparray[i];
        delete [] movearray[i];
		maparray[i] = NULL;
        movearray[i] = NULL;
	}
    for (int i = 0; i < warships[0].size(); i++) {
        delete warships[0][i];
        warships[0][i] = NULL;
    }
    for (int i = 0; i < warships[1].size(); i++) {
        delete warships[1][i];
        warships[1][i] = NULL;
    }
	// free both arrays pointers as well
	delete [] maparray;
    delete [] movearray;
	maparray = NULL;
    movearray = NULL;
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

int** Battlefield::getMoveMask() const {
    return movearray;
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
    warships[team].push_back(ship);
}

std::vector<Warship*> Battlefield::getWarshipList(size_t team) const {
    return warships[team];
}

void Battlefield::spawnShips() {
}

void Battlefield::spawnShip(SDL_Rect loc, size_t type, size_t team) {
    Warship_Corvette* temp = new Warship_Corvette();
    temp->setLocation(loc);
    addWarship(temp, team);
}

bool Battlefield::select(SDL_Rect loc) {
    size_t team = turn % 2;
    for (int i = 0; i < warships[team].size(); i++) {
        SDL_Rect temp = warships[team][i]->getLocation();
        if (temp.x == loc.x && temp.y == loc.y) {
            selected_warship = warships[team][i];
            markValidMoves();
            return true;
        }
    }
    selected_warship = NULL;
    markValidMoves();
    return false;
}

void Battlefield::markValidMoves() {
    // set up map mask for valid move spaces
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            movearray[i][j] = 0;
        }
    }
    if (selected_warship == NULL) {
        // no selected warship, there are no valid moves
        return;
    }
    // queue for a breadth first search
    std::queue<SDL_Rect> queue;
    std::queue<size_t> distance;
    queue.push(selected_warship->getLocation());
    distance.push(0);
    while (queue.size() > 0) {
        SDL_Rect temp = queue.front();
        size_t dist = distance.front();
        queue.pop();
        distance.pop();
        // make sure space is within move range and a water tile
        if (dist <= selected_warship->getMoverange() && maparray[temp.x][temp.y] == 0) {
            movearray[temp.x][temp.y] = 1;
        } else {
            continue;
        }
        // add surrounding spaces to queue if they haven't yet been searched
        temp.x += 1;
        if (temp.x < width && movearray[temp.x][temp.y] == 0) {
            queue.push(temp);
            distance.push(dist+1);
        }
        temp.x -= 2;
        if (temp.x >= 0 && movearray[temp.x][temp.y] == 0) {
            queue.push(temp);
            distance.push(dist+1);
        }
        temp.x += 1;
        temp.y += 1;
        if (temp.y < height && movearray[temp.x][temp.y] == 0) {
            queue.push(temp);
            distance.push(dist+1);
        }
        temp.y -= 2;
        if (temp.y >= 0 && movearray[temp.x][temp.y] == 0) {
            queue.push(temp);
            distance.push(dist+1);
        }
    }
}
