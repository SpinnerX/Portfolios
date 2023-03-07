#include "/public/read.h"
#include "/public/colors.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <unordered_map>
#include <cctype>
#include <sstream>
#include <climits>
using namespace std;
// using namespace bridges;

//Each word will have a list of other words they're connected to by this weighted edge.
//For example if APPLE has the following Markovs associated with it: {PEAR,3},{GRAPE,2},{NANA,10} this means that three possible words follow APPLE (PEAR, GRAPE, NANA) and the probability we pick PEAR is 3/15, GRAPE 2/15, NANA 10/15.
//So we'd roll a number from 0 to 14 and if we got a 0-2 we'd pick PEAR, 3-4 we'd pick GRAPE, and the rest NANA
struct Edge {
	string word; //The word we're connected to
	int weight = 0; //How often they follow us
};

//Adjacency List style graph
struct Vertex {
	string word; //What word this vertex corresponds to, like APPLE
	int count = 0; //How often this word came up
	int total_edge_weight = 0; //Total weight of the edges coming out of this vertex
	int start_count = 0; //How often this word starts a sentence
	//End_count can just be computed by count - total_edge_weight
	//int end_count = 0; //How often this word ends a sentence or has a period following it
	int comma_count = 0; //How often the word has a comma following it
	vector<Edge> edges; //Holds the words we're connected to, and how often they follow us, like {PEAR,3}
};
ostream &operator<<(ostream &outs,const Vertex &v) {
	outs << "Vertex " << v.word << " - Count: " << v.count <<
		" TEW: " << v.total_edge_weight <<
		" Start Count: " << v.start_count <<
		" End Count: " << (v.count - v.total_edge_weight) <<
		" Comma Count: " << v.comma_count << 
		" Edges:" << endl;
	for (const Edge &e : v.edges) {
		outs << '\t' << e.word << ": " << e.weight << endl;
	}
	return outs;
}

//Utility functions for changing the case on words
//The graph will hold everything in ALL CAPS

//Turn food -> FOOD
inline void uppercaseify(string &s) {
	for (char &c : s) c = toupper(c);
}
//Turn FOOD -> food
inline void lowercaseify(string &s) {
	for (char &c : s) c = tolower(c);
}
//Turn FOOD -> Food, for starting a sentence
inline void first_cap(string &s) {
	if (!s.size()) return;
	for (char &c : s) c = tolower(c);
	s.at(0) = toupper(s.at(0));
}

//Eliminates all words between square brackets to remove informative lines in lyrics
//Example: "Chorus[GZA]:" becomes "Chorus:"
void strip_brackets(string &s) {
	string retval;
	bool deleting = false;
	for (size_t i = 0; i < s.size(); i++) {
		if (s[i] == '[') deleting = true;
		else if (s[i] == ']') deleting = false;
		else if (!deleting) retval.push_back(s[i]);
	}
	s = retval;
}

int main() {
	/*
#ifndef USING_MAKEFILE
	//static_assert(false,"Use 'make' to build this project.");
#endif
	//Set up BRIDGES
	const int bridges_project_id = 50;
	string bridges_username = "your_name_here";
	string bridges_api_key  = "your_api_key_here";
	Bridges bridges(bridges_project_id, bridges_username, bridges_api_key);
	*/

	cout << "Please enter a text file to open:\n";
	const string filename = read();
	ifstream ins(filename);
	if (!ins) {
		cout << "Bad filename yo\n";
		exit(EXIT_FAILURE);
	}
	
	//Load text from the file - uppercase each word read
	unordered_map<string,size_t> hash; //Holds the index (in vec) of each word we've read
	vector<Vertex> graph; //Holds each word and who it's connected to via adjacency list
	long long total_start_count = 0; //Holds how many sentences we've read

	//PART 1 - BUILD THE GRAPH FROM THE FILE
	//Create one vertex for each word, with a directed edge between them each time word B follows word A
	while (true) {
		string s = readline(ins);
		uppercaseify(s);
		strip_brackets(s); //Remove any words [like this] from the string
		if (!ins) break;
		if (!s.size()) continue;
		stringstream sts(s);
		bool first_word = true;
		size_t last_index = INT_MAX; //Not quite right
		while (true) {
			bool comma_follows = false;
			string word = read(sts); //Read a word from the line
			if (!sts or !word.size()) break;
			while (word.size() and word.back() == ',') { //Drop commas from the end of words
				comma_follows = true;
				word.pop_back();
			}
			while (word.size() and ispunct(word.back())) //Drop other punctuation from the end of the word
				word.pop_back();
			if (!word.size()) continue; //If nothing left continue

			//Look up index in the vector using a hash table
			size_t index = 0;
			// Exists already
			if (hash.count(word)) index = hash[word];
			else { //Make the new vertex
				Vertex v = {word};
				graph.push_back(v);
				index = graph.size()-1;
				hash[word] = index;
			}
			Vertex &v = graph.at(index); //Grab reference to current vertex, this is like a pointer
			v.count++; //We've seen it one more time
			if (comma_follows) v.comma_count++;
			if (first_word) {
				v.start_count++;
				total_start_count++;
				first_word = false;
				last_index = index;
			}
			else {
				Vertex &prev_v = graph.at(last_index);
				prev_v.total_edge_weight++;
				bool found = false;
				//O(N) bleh
				for (Edge &e : prev_v.edges) {
					if (e.word == word) {
						found = true;
						e.weight++;
						break;
					}
				}
				// This is the first time the new word follows the old word
				if (!found) prev_v.edges.push_back({word,1});

				last_index = index;
			}
		}
	}

	cout << "1. Print Graph and Quit\n";
	cout << "2. Generate Random Lyrics\n";
	cout << "3. View on BRIDGES\n";

	int choice = read();
	
	if (choice == 1) {
		for (const Vertex &v : graph) {
			cout << v;
		}
		cout << "Total Start Count: " << total_start_count << endl;
		exit(0);
	}

	//Sanity check that we read in at least one sentence
	if (!total_start_count) {
		cout << "Read no sentences, bailing out now...\n";
		exit(1);
	}

	//PART 2 - GENERATE SOME SENTENCES
	const int sentences = read("How many sentences do you wish to make?\n");
	const int seed = read("Please enter the random seed:\n");

	srand(seed);

    for (int i = 0; i < sentences; i++) {
		int roll = rand() % total_start_count; 
		//O(N) BLEEEEEEHHHHHH find starting vertex
		Vertex cur;
		for (const Vertex &v : graph) {
			roll -= v.start_count;
			if (roll < 0) {
				cur = v;
				break;
			}
		}
		
		string temp = cur.word;
		first_cap(temp);
		cout << temp;
		//Generate sentence
		while (true) {
			//Roll to see if we end the sentence here or pick a random word
			int end_count = cur.count - cur.total_edge_weight;
			roll = rand() % cur.count - end_count;
			//Check for end first
			if (roll < 0) {
				cout << "." << endl;
				break;
			}

			//Make a separate random roll to see if we should emit a comma
			int roll2 = rand() % cur.total_edge_weight - cur.comma_count;
			if (roll2 < 0) cout << ',';

			//If not the end, then randomly pick an edge to follow based on the weight
			//Uses the first random roll to go through the rest of the weight
			for (const Edge &e : cur.edges) {
				roll -= e.weight;
				if (roll < 0) {
					cur = graph.at(hash[e.word]);
					string s = cur.word;
					lowercaseify(s);
					cout << " " << s;
					break;
				}
			}
		}
		cout << endl;
	}

	// PART 3 - SEND THE GRAPH TO BRIDGES
	// YOU
}
