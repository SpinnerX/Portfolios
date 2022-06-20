#include "tile.h"
#include "Bridges.h"
#include "ColorGrid.h"
#include <deque>
#include <cmath>
#include <unordered_set>
#include <algorithm>
#include <functional>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <climits>
#include <queue>
using namespace std;
using namespace bridges;

//This is the C++17 way of doing a #define DEBUG
constexpr bool DEBUG = false; //Set to true to get informative print outs, asserts, and bounds-checking
const int PATH = -1; //Elevation of -1 means it is on the shortest route path
const int WATER = -2; //Elevation of -2 means it is water and hence blocking terrain

// function prototypes
bool findPath(vector<vector<int>> &elev_data, const int width, const int height, const int startRow, const int startCol, const int endRow, const int endCol);
vector<vector<int>> readData(int &width, int &height, int &maxVal, int &minVal, const string &file_name);
ColorGrid *getImage(const vector<vector<int>> &elev_data, const int width, const int height, const int maxVal, const int minVal);

// This program implements the mountain paths assignment in C++.
// WRK Now using Dijkstra's algorithm to minimize horizontal + 20*vertical distance
int main(int argc, char **argv) {
	int bridges_project_id = 1009;
	string bridges_username = "Aaron_Her";
	string bridges_api_key  = "672627466850";

	// bridges object initialization
	Bridges bridges(bridges_project_id, bridges_username, bridges_api_key);

	// read input elevation data
	int width = 0, height = 0, maxVal = 0, minVal = 0;

	// parse command line arguments
	// argument 2 is input file
	// string filename = "./colorado1.dat";
	string filename = "./lake_victoria_maybe.dat";
	if (argc >= 2) filename = argv[1];
	vector<vector<int>> elev_data = readData(width, height, maxVal, minVal, filename);

	bridges.setTitle("Mountain Paths "s + filename + " - Dijkstra's Version");

	//WRK - Prompt the user for starting and ending locations
	int startRow = 0, startCol = 0, endRow = 0, endCol = 0;
	cout << "MaxRow = " << height - 1 << " MaxCol = " << width - 1 << endl;
	cout << "Please enter starting row and col:\n"; //Only starting row is used for greedy mode
	cin >> startRow >> startCol;
	cout << "Please enter ending row and col:\n"; //Neither of these is used in greedy mode
	cin >> endRow >> endCol;

	// Will Dijkstra from the starting points to the end
	bool path_exists = findPath(elev_data, width, height, startRow, startCol, endRow, endCol);
	if (!path_exists) {
		cout << "Sorry, no path exists!\n"; //This won't happen without blocking terrain
		exit(EXIT_FAILURE);
	}

	// get the path written into a color grid for visualization
	ColorGrid *cg = getImage(elev_data, width, height, maxVal, minVal);

	// visualize
	bridges.setDataStructure(cg);
	bridges.visualize();

	delete cg; //WRK Fix memory leak
}

vector<vector<int>> readData(int &width, int &height, int &maxVal, int &minVal, const string &file_name) {
	ifstream infile(file_name);
	if (!infile) { //WRK
		cout << "Error opening file: " << file_name << endl;
		exit(EXIT_FAILURE);
	}

	// read header
	int max = 0, val;
	infile >> width >> height;
	if (!infile or width < 1 or height < 1) { //WRK
		cout << "Error reading from file: " << file_name << endl;
		cout << "The format is: number of cols, number of rows, followed by rows*cols ints of height data.\n";
		cout << "Cols and rows must both be >= 1.\n";
		cout << "If you just downloaded the file from the NOAA, you'll have to edit the file a bit to match.\n";
		exit(EXIT_FAILURE);
	}
	if (width > MAX_SIZE or height > MAX_SIZE) { //WRK
		cout << "Error reading from file: " << file_name << endl;
		cout << "MAX_SIZE for any dimension is " << MAX_SIZE << endl;
		exit(EXIT_FAILURE);
	}

	//Read height data
	vector<vector<int>> vec(height, vector<int>(width)); //WRK Replaced 1D array with 2D vector
	maxVal = INT_MIN;
	minVal = INT_MAX; //WRK added in minVal so that the greyscale will scale properly at high elevations
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int temp = 0;
			infile >> temp;
			if (!infile) {
				cout << "Error reading from file: " << file_name << endl;
				cout << "Unexpected end of file occured, maybe you don't have rows*cols data in there?\n";
				exit(EXIT_FAILURE);
			}
			if (temp < 0) temp = WATER; //WRK - All below sea level points are now blocking terrain / water
			vec.at(i).at(j) = temp;
			if (maxVal < temp) maxVal = temp;
			if (minVal > temp) minVal = temp;
		}
	}
	assert(maxVal >= minVal);
	return vec;
}

// takes in the processed elevation data and returns a color grid for
// visualization using BRIDGES
ColorGrid *getImage(const vector<vector<int>> &elev_data, const int width, const int height, const int maxVal, const int minVal) {
	// create color grid
	ColorGrid *cg = new ColorGrid(height, width);

	float pixel_val = 0;
	assert(maxVal >= minVal);
	float range = maxVal - minVal;
	if (range < 1) range = 1; //Stop division by 0 on a flat map
	int n = 0, gray = 0;

	// load the elevation data
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) {
			if constexpr(DEBUG) pixel_val = (float) elev_data.at(i).at(j);
			else pixel_val = (float) elev_data[i][j];
			if (pixel_val == PATH) {
				cg->set(i, j, Color(200, 0, 0)); //Draw a colored line at each point with -1 elevation set by FindPath
			} else if (pixel_val == WATER) { //Below sea level
				cg->set(i, j, Color(15, 20, 40)); //Make it blue
			} else {
				// scale value to be within 0-255, for r,g,b range
				//gray = (int) (pixel_val)*255/maxVal;
				gray = 255 * ((pixel_val - minVal) / range); //WRK - clamp values to 0..255, and add in minVal
				const int GREEN_MIX = 30;
				const int BROWN_MIX = 150;
				const float BROWN_R = 1.3;
				const float BROWN_G = 1.15;
				if (gray < GREEN_MIX) //WRK Mix in some green to low level elevations to make it look pretty
					cg->set(i, j, Color(gray, 1.1 * gray + 5, gray)); //WRK - Careful, it throws if values aren't in range
				else if (gray < BROWN_MIX) { //WRK Mix in some brown
					cg->set(i, j, Color(BROWN_R * gray, BROWN_G * gray, gray));
				} else { //And then fade to white on the mountain tops
					int r = (BROWN_R * BROWN_MIX + gray) / 2;
					int g = (BROWN_G * BROWN_MIX + gray) / 2;
					int b = gray;
					cg->set(i, j, Color(r, g, b));
				}
			}
		}
	return cg;
}

// determines the least effort path through the mountain starting a point on
// the left edge of the image
bool findPath(vector<vector<int>> &elev_data, const int width, const int height, const int startRow, const int startCol, const int endRow, const int endCol) {

	if (startRow < 0 || startRow >= height) {
		cout << "Invalid starting row.\n"; //WRK Let them know
		exit(EXIT_FAILURE);
	}
	if (startCol < 0 || startCol >= width) {
		cout << "Invalid starting column.\n";
		exit(EXIT_FAILURE);
	}
	if (endRow < 0 || endRow >= height) {
		cout << "Invalid ending row.\n";
		exit(EXIT_FAILURE);
	}
	if (endCol < 0 || endCol >= width) {
		cout << "Invalid ending column.\n";
		exit(EXIT_FAILURE);
	}
	if (elev_data.at(startRow).at(startCol) < 0) {
		cout << "Can't swim, sorry. You picked a point under water.\n";
		exit(EXIT_FAILURE);
	}

	//Dijkstra's Algoritm starts here -
	priority_queue<Tile, vector<Tile>, Tile_Comp> to_process; //Auto sorts - via a min heap - all tiles we need to process
	unordered_set<Tile> processed; //Hash table of all tiles that we've finished processing
	processed.reserve(width * height); //We will need at most width*height tiles processed
	Tile start(startCol, startRow, -1, -1, 0); //-1 is a sentinel that indicates they have no parent tile
	to_process.push(start); //We start with just the starting tile in the queue

	//While there are still tiles to process, pop the nearest tile off and add its children to the queue
	while (to_process.size()) {
		Tile current = to_process.top(); //Get the tile closest (not as the crow flies) to the start
		to_process.pop(); //Delete it from the to_process list
		if constexpr(DEBUG) cout << "Processing tile: " << current.x << "," << current.y << " Elevation: " << elev_data.at(current.y).at(current.x) << " Distance from start = " << current.distance << endl;
		if (processed.count(current)) {
			if constexpr(DEBUG) cout << "Already seen it, skipping...\n";
			continue;
		}
		processed.insert(current); //Add current to the processed list

		//YOU: If end is found, then handle that - backtrack and set x and y to the next move the user should make
		if (current.x == endCol and current.y == endRow) {
			if constexpr(DEBUG) cout << "Found end!\n";
			while (true) {
				if constexpr(DEBUG) cout << "Backtracking - currently at " << current.x << "," << current.y << endl;
				//We normally would add it to a vector of tiles to output the path, but it's unnecessary in this assignment
				elev_data.at(current.y).at(current.x) = PATH; //Mark it on the route
				//if (current.x == startCol && y == startRow) break; //Redundant
				if (current.from_x == -1 or current.from_y == -1) break;
				Tile temp(current.from_x, current.from_y, 0, 0, 0); //Dummy variable used to look up real data for parent in hash table
				auto it = processed.find(temp);
				if constexpr(DEBUG) assert(it != processed.end());  //This should never happen
				current = *it;
			}
			return true;
		}

		//Add all 8 kids of current that are not in processed, including diagonals
		for (int i = -1; i <= 1; i++) { //+-1 in the x (horizontal) direction
			for (int j = -1; j <= 1; j++) { //+-1 in the y (vertical) direction
				if (i == 0 and j == 0) continue; //Skip current tile
				int new_x = current.x + i;
				int new_y = current.y + j;
				if (new_x < 0 or new_x >= width or new_y < 0 or new_y >= height) continue; //Bounds check

				//Heart of the program - compute move distance
				//For this assignment the formula is horizontal distance + 20*vertical distance
				//With one arc-minute maps, this works out to a horizontal cost of:
				//  1840m for straight movement or 2601m for diagonals
				const double STRAIGHT = 1840.0; //TODO: Allow different map resolutions
				const double DIAGONAL = 2601.0;
				double move_cost = STRAIGHT;
				if (i && j) move_cost = DIAGONAL;
				int my_elevation, new_elevation;
				if constexpr(DEBUG) { //Do bounds checking when debug mode is on
					my_elevation = elev_data.at(current.y).at(current.x);
					new_elevation = elev_data.at(new_y).at(new_x);
				} else { //Disable bounds check when DEBUG mode is off
					my_elevation = elev_data[current.y][current.x];
					new_elevation = elev_data[new_y][new_x];
				}
				if (my_elevation < 0) continue; //Don't travel over water
				move_cost += 20 * abs(my_elevation - new_elevation); //Add in vertical cost
				if constexpr(DEBUG) {
					cout << "\tTile: " << new_x << "," << new_y << " elevation: " << new_elevation << endl;
					cout << "\tCost to move there: " << move_cost << endl;
				}

				Tile child(new_x, new_y, current.x, current.y, current.distance + move_cost);
				if (processed.count(child)) {
					if constexpr(DEBUG) cout << "Skipping duplicate...\n";
					continue;
				}
				to_process.push(child);
			}
		}
	}
	return false; //No route found
}
