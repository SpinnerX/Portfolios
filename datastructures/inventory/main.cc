#include <iostream>
#include <sstream>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>
#include <array>
#include "/public/read.h"
#include <set>
#include <algorithm>
using namespace boost;
using namespace std;

//Turn on or off all debugging couts
#define DEBUG(X) cout << X << endl;

//Upper cases a string
string uppercaseify(string &str) {
	for (char &ch : str) ch = toupper(ch);
	return str;
}

void die() {
	cout << "Invalid Input!\n";
	exit(EXIT_FAILURE);
}

class Inventory {
	// vector<string> cars = {};
	string name = "";
  public:
	vector<string> cars = {};
	Inventory() = default;
	Inventory(string new_name) : name(new_name) { }

	bool is_in(string new_car){
		for(string car : cars){
			if(car == new_car) return true;
		}
		return false;
	}

	void insert(string new_car) {
		if(is_in(new_car)) return;

		cars.push_back(new_car);
	}

	/*
		Intersection
		- grabs two sets and grabs the values from both sets that are the same
	Example:
		Set A: {A, B, B}
		Set B: {J, K, A}
		- Set A & B both have the values of A in those sets
		- This is what intersection is
	*/
	Inventory intersect(const Inventory &other) {
		Inventory set;

		for(string car1 : cars){
			for(string car2 : other.cars){
				if(car1 == car2) set.insert(car1);
			}
		}

		return set; //Stub
	}
	
	/*
		Unionize
		- Grabs both of the two sets and puts them into one whole set
	Example:
		- Set A: {A, B, C}
		- Set B: {J, K, A}
	- Unionize set for A & B  would be {B, C, J, K}
	- Since value A are duplicates, they do not go into this set.
	- This is what unionize is.
	*/
	Inventory unionize(const Inventory &other) {
		Inventory set;

		for(auto i : cars){
			set.insert(i);
		}

		for(auto j : other.cars){
			set.insert(j);
		}
		
		return set; //Stub
	}

	int size() { return cars.size(); }
	string getName() { return name; }

	friend istream& operator>>(istream &ins, Inventory &other);
	friend ostream& operator<<(ostream &outs, Inventory &other);
	
};

class SuperSet {
	public:
	vector<Inventory> set;
	SuperSet() = default;
	
	void create_table(Inventory new_inventory){
		set.emplace_back(new_inventory);
	}

	//It will return the inventory matching name
	//If ignore_fail is set, then it won't die on a lack of a match
	Inventory* search(string name, bool ignore_fail = false) {
		for(Inventory& inventory : set){
			if(inventory.getName() == name) return &inventory;
		}
		ignore_fail = true;
		return nullptr;
	}

	//Prints all inventories in all tables
	void print_all() {
		vector<string> temp;
		
		// Iterating and then saving to the temp vector
		for(size_t i = 0; i < set.size(); i++){
			for(int j = 0; j < set.at(i).size(); j++){
				temp.push_back(set.at(i).cars.at(j));
			}
		}

		sort(temp.begin(),temp.end());

		temp.erase(unique(temp.begin(), temp.end()),temp.end());
		
		// Print the values from the inventory
		for(auto i : set){
			cout << i.getName() << ": " << i << endl;
		}
		cout << temp.size() << endl;
	}
	
	void poset() { //Do the partial ordering of the sets here
		for(auto i : set){
			string current;
				
			for(auto j : i.cars) current.append(j);
			for(auto k : set){
					string current2;
					for(auto l : k.cars) current2.append(l);
					
					if(current2.size() < current.size() and current.find(current2) != current.npos) cout << i.getName() << " " << k.getName();
					else current2.clear();
					// if(is_subset(current_cars2, current_cars)) cout << i.getName() << " " << k.getName();
					// else current_cars2.clear();
			}
		}
	}
} Set;

//You don't need to write this if you don't want.
istream& operator>>(istream &ins, Inventory &other) {
	ins >> other.name;
	return ins;
}

//This should output an invntory to outs
ostream& operator<<(ostream &outs, Inventory &other) {
	// outs << other.name << ": ";
	for(size_t i = 0; i < other.cars.size(); i++){
		if(!(i == other.cars.size()-1)) cout << other.cars[i] << ", ";
		else cout << other.cars[i] << endl;
	}
	return outs;
}

int main(){
	while(!cin.eof()){
		if(!cin) break;

		string input = readline("");

		stringstream i_stream(input);
		string first, second, third, fourth, fifth, sixth, seventh;
		i_stream >> first >> second >> third >> fourth >> fifth >> sixth >> seventh;

		if(first.size() < 1) break;

		string temp = to_upper_copy<string>(third);
		
		// Create a new table for user input
		if(first == "CREATE"){
			if(second != "TABLE") die();
			if(third != temp) die();
			
			Set.create_table(Inventory(third));
		}
		// Inserting
		if(first == "INSERT"){
			// string temp = to_upper_copy<string>(third);
			// Using search() in SuperSet to load in an inventory.
			// if(second != "INTO" || third != temp || fourth != "VALUES") die();

			Inventory* load_table = Set.search(third); // set.search(vec[2]);
			if(load_table == nullptr) die();

			stringstream ss2(fifth);
			while(ss2){
				string commands = readline(ss2, ',');
				// getline(ss2, commands, ',');
					
				if(!ss2) break; // THIS PATH!!!
				
				if(!Set.search(third)) die();

				if(Set.search(third)){
					if(Set.search(commands)) for(auto car : Set.search(commands)->cars) load_table->insert(car);
					else Set.search(third)->insert(commands);
				}
			}
		}
		// Selection
		// SELECT * FROM *
		else if(first == "SELECT" ){

			bool bool3 = (second == "*" and third == "FROM" and fifth == "INNER" and sixth == "JOIN");
			bool bool2 = ((second == "*" and third == "FROM" and fourth == "*") or (bool3));
			bool boolVal = ((second == "FROM" and fourth == "UNION") or (bool2));

			if(!boolVal) die(); 
			
			if(second == "*" and third == "FROM" and fourth == "*") Set.print_all();
			
			Inventory inv_temp1, inv_temp2;
			if(second == "FROM" and fourth == "UNION"){

				if(!Set.search(fourth) and !Set.search(fifth)) die();
				
				// Search values exist
				if(Set.search(third) and Set.search(fifth)){
					inv_temp2 = *Set.search(fifth); // * dereference
					inv_temp1 = Set.search(third)->unionize(inv_temp2);
					cout << inv_temp1;
				}
			}
			if(second == "*" and third == "FROM" and fifth == "INNER" and sixth == "JOIN"){
				// Checking if the two sets already exist in database before unionizing the two sets togetherr.
				if(Set.search(fourth) and Set.search(seventh)){
					inv_temp2 = *Set.search(seventh); // Dereference
					inv_temp1 = Set.search(fourth)->intersect(inv_temp2);
					cout << inv_temp1;
				}
			}
		}
		else if(first == "IS"){
			if(third == "SUBSET"){
				Inventory inv1;
				Inventory inv2;

				if(!Set.search(second) and Set.search(fourth)) die();

				inv2 = *Set.search(fourth);
				inv1 = *Set.search(second);

				sort(inv1.cars.begin(),inv1.cars.end());
				sort(inv2.cars.begin(), inv2.cars.end());
				
				// std::includes() returns a boolean
				// sees is one range contains the same value
				if(includes(inv2.cars.begin(), inv2.cars.end(), inv1.cars.begin(),inv1.cars.end())) cout << "true" << endl;
				else cout << "false" << endl;
			}
		}
		// POSET is a proper subset
		// Meaning: set that is a sub set of another set but entirely different
		/*
		* Example:
		* Set A: {1,3,5}
		* Set B: {1,5}
		* B is a sub set of A because it holds the same values, but is entirely different from set A
		* Set A contains an element that Set B does not contain, which is another meaning of proper sub sets.
		*/
		else if(first == "POSET"){ Set.poset(); }
	}
	return 0;
}
