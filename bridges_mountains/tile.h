#include <unordered_set>
#include <iostream>
//Tile.h - Bill Kerney (WRK) 10/28/19
//A simple Tile class for doing A* or Dijkstra's on a grid
//Can be put into pretty much any standard library container class - hash tables, binary trees, heaps, etc.

//MAX_SIZE needs to be here so our hash function is more efficient
const size_t MAX_SIZE = 1000; //1000x1000 map is the biggest we handle

struct Tile {
	int x,y; //The location of the tile on the grid
	int from_x,from_y; //The location of the place we came from to get here
	double distance; //Our distance from the start, not as the crow files, but by walking
	Tile() = delete; //All tiles need a valid location, so delete default constructor
	Tile(int x, int y, int from_x, int from_y, double distance) {
		this->x = x;
		this->y = y;
		this->from_x = from_x; //-1 if the start tile
		this->from_y = from_y; //-1 if the start tile
		this->distance = distance;
	}
	//Two tiles are the same if they share x and y locations
	// even if they have different distances - this is how we check for duplicate entries
	bool operator==(const Tile &rhs) const { 
		return (x == rhs.x and y == rhs.y);
	}
	bool operator<(Tile &rhs) const { 
		return distance < rhs.distance;
	}
};

//Comparison functor to make a min heap (priority queue) instead of a max heap of tiles
//Make a min heap like this: priority_queue<Tile,vector<Tile>,Tile_Comp> to_process; 
struct Tile_Comp {
	bool operator()(const Tile &lhs, const Tile &rhs) { 
		return lhs.distance > rhs.distance;
	}
};

//Functor to make a hash function for a tile
//Make a hash table like this: unordered_set<Tile> processed;
namespace std {
	template <> struct hash<Tile> {
		size_t operator()(const Tile& t) const {
			return MAX_SIZE * (size_t)t.x + (size_t)t.y;
		}
	};
}

