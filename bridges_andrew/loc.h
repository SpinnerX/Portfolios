#include <iostream>
#include <fstream>
#include <list>
#include <Bridges.h>
#include <GraphAdjList.h>
using namespace std;
using namespace bridges;

//YOU: Write this in the file andrew.cc
void andrew(GraphAdjList<string, int, double>& gr);

//A struct to hold location data
//You will need to write an operator< so it can be sorted west to east
struct Location {
	string name;   // concatenated city and state
	double lat, longit;

	Location () {};
	Location (string nm, double latitude, double longitude) {
		name = nm; lat = latitude; longit = longitude;
	} 
	//YOU: sort left to right, so using longitude first, then on a tie latitude
	//    Remember, longitude is long, and latitude is like a ladder
	//YOU: write a friend bool operator<(const Location &l1, const Location &l2)
	friend bool operator<(const Location& l1, const Location l2){
		// if(l1.lat == l2.lat) return (l1.longit < l2.longit);
		// else return (l1.lat < l2.lat);
		// if(l2.longit < l2.longit) return true;
		// if (l1.longit == l2.longit && l1.lat < l2.lat) return true;
		// return false;
		return (l1.longit < l2.longit || (l1.longit == l2.longit and l1.lat < l2.lat));
	}

	//This is the 2D cross product, useful to know if something is to the left or right
	//Given two lines from p0 to p1, and from p1 to p2
	//Returns a positive number on a left turn, 0 if co-linear, negaive if right turn
	/*
	Example: this would return a negative number, incidating a right turn from p0 to p1 to p2
		   p1
		  /  \
		 /    \
	   p0     p2
	*/
	friend double cross(const Location &p0, const Location &p1, const Location &p2)
	{
		return (p1.longit - p0.longit) * (p2.lat - p0.lat) - (p1.lat - p0.lat) * (p2.longit - p0.longit);
	}

	//Function to print a Location
	friend ostream &operator<<(ostream &outs, const Location &loc) {
		return outs << loc.name << ": (" << loc.longit << "," << loc.lat << ")";
	}
};

//Distance between two cities
double getDist (double lat1, double long1, double lat2, double long2);

//Build the US Cities graph from the data file
void buildUSCityGraph (GraphAdjList<string, int, double> & gr);

//Adds an edge between v and v2 to the graph gr, using the given thickness and color
inline void add_edge(GraphAdjList<string, int, double> &gr, const string &v, const string &v2, double thickness = 0.3, const string &color = "black") {
	//Figure out the distance between v and v2 in the real world
	double lat1, lat2, longit1, longit2;
	longit1 = gr.getVertex(v)->getLocationX();
	lat1 = gr.getVertex(v)->getLocationY();
	longit2 = gr.getVertex(v2)->getLocationX();
	lat2 = gr.getVertex(v2)->getLocationY();
	double d = getDist(lat1, longit1, lat2, longit2)/1000.;
	gr.addEdge(v, v2, d);
	gr.getLinkVisualizer(v, v2)->setLabel(std::to_string((int)d));
	gr.getLinkVisualizer(v, v2)->setThickness(thickness);
	gr.getLinkVisualizer(v, v2)->setColor(color);
}
