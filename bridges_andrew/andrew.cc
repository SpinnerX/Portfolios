#include "loc.h"

//YOU: Implement Andrew's Monotone Chain Convex Hull Algorithm
//gr is a graph, but we ignore the edges and just make a hull out of the vertices
void andrew(GraphAdjList<string, int, double>& gr) {
	vector<Location> locs;
	vector<Location> upper_hull;
	vector<Location> lower_hull;
	//Copy all of the vertices from the graph to a vector named locs so we can sort it
	for (auto& v : gr.keySet()) {
		locs.emplace_back(v,gr.getVertex(v)->getLocationY(),gr.getVertex(v)->getLocationX());
	}
	if (locs.size() < 3) return;
	//YOU: Sort west to east
	// static_assert(false,"Make an operator< for Location in loc.h for this to compile!");
	sort(locs.begin(),locs.end());
	//Uncomment this to see if the sorting worked
	// for (auto &a : locs) cout << a << endl;

	//Compute upper hull
	upper_hull.push_back(locs[0]);
	upper_hull.push_back(locs[1]);
	for (size_t i = 2; i < locs.size(); i++){
		//YOU: Add loc[i] to the upper_hull
		// <= 0 is right
		// >= 0 is left
		// Another way of implementing it!
		while(upper_hull.size() >= 2 and cross(upper_hull.at(upper_hull.size()-2), upper_hull.at(upper_hull.size()-1), locs.at(i)) >= 0){
			upper_hull.pop_back();
		}
		upper_hull.push_back(locs.at(i));
		//Check to see if there is a left turn from the last three entries in upper hull
		// (For example, cross(p0,p1,p2) will be positive if there is a left turn from p0->p1-p2)
		//If there is a left turn, delete the last point (not this point, but the last one - p1) and repeat 
		// until either there is a right turn or there are only two points left in upper_hull
		
	}

	//For every vertex in the upper hull, print it to cout and then also change its size and color
	cout << "Upper Hull:\n";
	for (size_t i = 0; i < upper_hull.size(); i++) {
		const Location &a = upper_hull.at(i);
		cout << a << '\n';
		//YOU: All vertices in the upper hull should be size 40 and bright red
		//  Access vertices in the graph by using a.name, for example
		gr.getVertex(a.name)->setSize(40.0);
        // gr.getVertex(a.name)->setColor("purple");
		gr.getVertex(a.name)->setColor("red");
		//YOU: Make a large bright red line between the nodes in the upper hull
		//  Use the add_edge function (see example in main.cc) to make the edge easily

		// Adding edges to make the edges red
		if(i < upper_hull.size()-1) add_edge(gr, upper_hull.at(i).name, upper_hull.at(i+1).name, 5, "red");
	}

	//YOU: Do the same but in reverse for the lower hull
	sort(locs.rbegin(), locs.rend());

	lower_hull.push_back(locs[0]);
	lower_hull.push_back(locs[1]);

	//YOU: Set the endpoints of each hull (the first and last elements) to be purple in color
	for(size_t i = 2; i < locs.size(); i++){
		
		while(lower_hull.size() >= 2 and cross(lower_hull.at(lower_hull.size()-2), lower_hull.at(lower_hull.size()-1), locs.at(i)) >= 0){
			lower_hull.pop_back();
		}
		
		lower_hull.push_back(locs.at(i));
	}

	cout << "Lower Hull:\n";
	for(size_t i = 0; i < lower_hull.size(); i++){
		const Location &a = lower_hull.at(i);
		cout << a << endl;

		gr.getVertex(a.name)->setSize(40.0);
		gr.getVertex(a.name)->setColor("blue");
		
		// Adding edges to make the lower part blue-ish
		if(i < lower_hull.size()-1) add_edge(gr, lower_hull.at(i).name, lower_hull.at(i+1).name, 5, "blue");
	}

	// Changes the given connected points into purple
	gr.getVertex(upper_hull.back().name)->setColor("purple");
	gr.getVertex(lower_hull.back().name)->setColor("purple");
	
}
