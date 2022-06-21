#include "loc.h"
using namespace std;
using namespace bridges;


int main() {
	// Initialize BRIDGES with your credentials
	Bridges bridges(26, "Aaron_Her", "672627466850");
	bridges.setTitle("Andrew's Monotone Chain");

	//Loads from disk and builds the graph from it
	GraphAdjList<string, int, double> gr; //Adjacency List Graph
	buildUSCityGraph (gr);

	//Set overlay to the map of the US
    bridges.setCoordSystemType("albersusa");
    bridges.setMapOverlay(true);
	bridges.setDataStructure(&gr);

	//YOU: Write the code for this in andrew.cc to wrap the graph in a convex hull
	andrew(gr);
	bridges.visualize();
}

void buildUSCityGraph (GraphAdjList<string, int, double> & gr) {

	string filename;
	cout << "Please enter the name of the file to load the graph from (hit enter for default):\n";
	getline(cin,filename);
	if (filename == "") filename = "city_graph_data.txt";
	ifstream infile(filename);

	// create the vertices
	string v, v2; 
	int num_verts, num_edges;
	double lat, longit;
	infile >> v >> num_verts;
	for (int k = 0; k < num_verts; k++) {
		infile >> v >> lat >> longit;
		gr.addVertex (v);
		gr.getVertex(v)->setLocation(longit, lat);
		gr.getVertex(v)->setSize(5.);
		gr.getVertex(v)->setColor(Color(204, 102, 0, 150));
	}
	// create the edges
	infile >> v >> num_edges;
	for (int k = 0; k < num_edges; k++) {
		infile >> v >> v2;
		add_edge(gr,v,v2);
	}
}

// uses the haversine distance formula to compute distance (in meters)
//  between two locations
double getDist (double lat1, double long1, double lat2, double long2) {
	const double R = 6371e3; //Meters
	const double phi1 = lat1 * M_PI/180; //Radians
	const double phi2 = lat2 * M_PI/180;
	const double del_phi = (lat2-lat1) * M_PI/180.;
	const double del_lambda = (long2-long1) * M_PI/180.;

	const double a = sin(del_phi/2.)*sin(del_phi/2) + cos(phi1)*cos(phi2) *
					sin(del_lambda/2.)*sin(del_lambda/2.);
	const double c = 2.*atan2(sqrt(a), sqrt(1.-a));

	return R * c; //in Meters
}


