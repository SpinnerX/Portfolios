#include "Bridges.h"
#include "ColorGrid.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <vector>
#include "/public/read.h"
#include "tile.h"
#include <set>
#include <queue>
using namespace std;
using namespace bridges;

// This program implements the mountain paths assignment in C++. See the README.md for a 
// detailed description

// CYAN = r=0, g=0, b=100
// Dark blue = r=0, g=10, b=150
// light blue = r=135, g=206, b=235

// light green = r=144, g=238, b=144
// light green(darken) = r=139, g=195, b=74

// grey (light) = r=111, g=116, b=118
// snow = r=254, g=254, b=253

void die(string message = "BAD INPUT!\n"){
	cout << message;
	exit(EXIT_FAILURE);
}

// function prototypes
ColorGrid *getImage (vector<vector<int>>& img_arr, int imgWidth, int imgHeight, int maxVal);
vector<vector<int>> readData (int& imgWidth, int& imgHeight, int& maxVal, string file_name);
void findPath(vector<vector<int>>& img_arr, int width, int height, Tile& start, Tile& end);
bool backtracking(unordered_set<Tile>& processed, vector<vector<int>>&, Tile&, Tile&);

int main(int argc, char **argv) {
	const int bridges_project_id = 1009;
	const string bridges_username = "Aaron_Her";
	const string bridges_api_key  = "672627466850";

	// bridges object initialization
	Bridges bridges(bridges_project_id, bridges_username, bridges_api_key);

	// read input elevation data
	int width, height, maxVal;
	vector<vector<int>> elev_data;

	// parse command line arguments
	// argument 2 is input file
	// string filename = "./colorado1.dat";
	string name = read("Enter filename: ");
	string filename = name + ".dat";
	if (argc >= 2) filename = argv[1]; //This is how you read a command line parameter
	//Elev_data is a pointer to a chunk of memory of size width * height
	elev_data = readData(width, height, maxVal, filename);

	// set title
	//YOU: Change this to "Mountain Paths - Dijkstra's"
	bridges.setTitle("Mountain Paths "s + filename + " - Dijkstra's Algorithms");

	//WRK - Added this section
	//Get starting row and column - the greedy algorithm only needs starting row,
	// but you will use the two locations for doing Dijkstra's
	int startRow = 0, startCol = 0, endRow = 0, endCol = 0;
	cout << "Please enter starting row and col:\n"; //Only starting row is used for greedy mode
	cin >> startRow >> startCol;

	cout << "Please enter ending row and col:\n"; //Neither of these is used in greedy mode
	cin >> endRow >> endCol;

	//YOU: Replace the greedy algorithm with Dijkstra's or A* between the starting and ending points
	//YOU: Look at how findPath does it's think in regards to converting between a 2D location and a 1D array, as well as how it marks the final path by changing the elevation to -1 along the path
	// find path by applying a greedy algorithm from the left side of the screen to the right
	// bool pathFound = findPath (elev_data, width, height, startRow, startCol, endRow, endCol);
	
	Tile start(startCol, startRow, -1, -1, 0); // Starting Tiles
	Tile end(endCol, endRow, 0, 0, -1); // Ending tiles.
	// findPath (elev_data, width, height, startRow, startCol, endRow, endCol);
	
	findPath(elev_data, width, height, start, end); 

	// get the path written into a color grid for visualization
	//YOU: Change the path color to red
	ColorGrid *cg = getImage(elev_data, width, height, maxVal);

	// visualize
	bridges.setDataStructure(cg);
	bridges.visualize();

	delete cg; //WRK Fix memory leak in BRIDGES
}

vector<vector<int>> readData(int &width, int &height, int& maxVal, string file_name) {

	ifstream infile(file_name.c_str());

	int max = 0, val = 0;
	infile >> width >> height;

	vector<vector<int>> elev_data(height, vector<int>(width));

	// read the elevation data
	for (int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++){
			infile >>  val;

			// update the max val
			max = (val > max) ? val : max;
	
			// store  in elevation array
			if(val < 0) elev_data[i][j] = -2;
			else elev_data[i][j] = val;
		}
	}
	// record the max value
	maxVal = max;

	return elev_data;
}

// takes in the processed elevation data and returns a color grid for 
// visualization using BRIDGES
ColorGrid *getImage(vector<vector<int>>& elev_data, int width, int height, int maxVal) {
	// create color grid
	ColorGrid *cg = new ColorGrid(height, width);

	float pixel_val;
	int gray;

	// load the elevation data
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++) {
			pixel_val = (float) elev_data[i][j];

			if (pixel_val == -1.) cg->set(i, j, Color(255, 0, 50)); // this is the path drawn in purple across the image
			else if(pixel_val == -2) cg->set(i, j, Color(135, 206, 235)); // coloring water levels
			else if(pixel_val < 0 or  pixel_val <= 850) cg->set(i, j, Color(139, 195, 74)); // Coloring green, for grass landscape.
			else if(pixel_val < 850 or pixel_val >= 855) cg->set(i, j, Color(245, 254, 253)); // Coloring a gray-ish white, for mountain landscapes.
			else {
				// scale value to be within 0-255, for r,g,b range
				gray = (int) (pixel_val*255./maxVal);
				cg->set(i, j, Color(gray, gray, gray));
			}
		}
	}
	return cg;
}

// determines the least effort path through the mountain starting a point on
// the left edge of the image
// void findPath(vector<vector<int>>& elev_data, int width, int height, int startRow, int startCol, int endRow, int endCol) {
void findPath(vector<vector<int>>& elev_data, int width, int height, Tile& start, Tile& end){	
	if(start.y < 0 || start.y >= height) die("Invalid starting row.\n");
	if(start.x < 0 || start.x >= width) die("Invalid starting col\n");
	if(end.y < 0 || end.y > height) die("Invalid ending row.\n");
	if(end.x < 0 || end.y > width) die("Invalid ending col.\n");

	const double STRAIGHT = 1840.0;
	const double DIAGONAL = 2601.0;
	//Dijkstra's Algoritm starts here -
	priority_queue<Tile, vector<Tile>, Tile_Comp> to_processed; // Auto sorts - via a min heap - all tiles we need to process
	unordered_set<Tile> processed; // Hash table of all tiles (neighbors) that we've finished processing

	processed.reserve(width * height); // We will need at most width*height tiles processed
	
	to_processed.push(start); // We start with just the starting tile in the queue
	
	// While there are still tiles to process, pop the nearest tile off and add its children to the queue
	while(to_processed.size()){
		Tile current = to_processed.top(); // Getting tile closest to the starting tile.
		to_processed.pop(); // Delete from queue.

		// if(processed.count(current)) continue; // Skipping sea level.
	
		// Skip sea level.
		if(!processed.count(current)) processed.insert(current);

		// Find all 8 tiles all around from current path.
		for(int i = -1; i <= 1; i++){
			for(int j = -1; j <= 1; j++){
				int newX = current.x + i;
				int newY = current.y + j;

				if(newX < 0 or newX >= width or newY < 0 or newY >= height) continue;

				double cost = STRAIGHT;

				if(i && j) cost = DIAGONAL;

				int currentElevation = elev_data[current.y][current.x];
				int newElevation = elev_data[newY][newX];

				if(currentElevation < 0) continue; // Skip over the water coordinates

				cost += 20 * std::abs(currentElevation - newElevation);

				Tile newTile(newX, newY, current.x, current.y, current.distance + cost);

				if(processed.count(newTile)) continue;

				to_processed.push(newTile);
			}
		}
		
		if(backtracking(processed, elev_data, current, end)) return;
	}
}

bool backtracking(unordered_set<Tile>& processed, vector<vector<int>>& elev_data, Tile& current, Tile& end){
	
	if(current.x == end.x and current.y == end.y){
		// Backtracking to draw the line.
		// Line will draw if we reach number -1, thorughout the map.
		// -1 = PATH, -2 = WATER
		while(true){

			// cout << "Backtracking - currently at " << current.x << " and " << current.y << endl;
			elev_data[current.y][current.x] = -1; // -1 is the PATH
			if(current.from_x == -1 or current.from_y == -1) break;

			Tile previous_tile(current.from_x, current.from_y, 0, 0, 0);

			auto iter = processed.find(previous_tile);

			current = *iter;
		}
		return true;
	}
	return false;
}
