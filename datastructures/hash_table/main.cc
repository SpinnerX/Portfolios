#include "/public/read.h"
#include "hash.h"
#include <memory>
#include <unistd.h>
using namespace std;

// 256 <-
// 256 * 2 = 512 <-
// 512 * 2 = 1024 <-

int main(int argc, char **argv) {
	const size_t SIZE = 1000000; //Change to 1M for benchmarking, back to 11 to pass cases
	//Note the slight difference from the enum class in hash.h
	enum MODE { LINEAR, CHAINING, DOUBLE };
	int mode = LINEAR;
	if (argc > 1) mode = atoi(argv[1]); //Read mode from command line parameters
	//If no command line parameter (a.out 0 or a.out 1) is provided, then read from keyboard
	else mode = read("Choose mode:\n0) Linear\n1) Chaining\n");
	if (mode < LINEAR or mode > CHAINING) mode = LINEAR;
	cerr << "Mode == " << mode << endl;

	Hash<int, SIZE> *table = nullptr; //Hash is an abstract class, but by C++ magic we can make it implement one of our two options
	if (mode == LINEAR) table = new LinearProbingHash<int, SIZE>;
	else if (mode == CHAINING) table = new ChainingHash<int, SIZE>; // Edited this line here on 9/1/21
	//YOU: Add double hashing here
	else table = new DoubleHash<int, SIZE>; // Edited this line here on 9/1/21

	while (cin) {
		string s;
		cin >> s;
		if (!cin) break;
		if (s == "CHANGE") {
			int x = 0, y = 0;
			cin >> x >> y;
			if (!cin) break;
			table->change(x, y);

		}
		else {
			int x = read();
			if (!cin) break;
			if (s == "INSERT") table->insert(x);
			
			else if (s == "SEARCH") {
				if (table->search(x)) cout << x << " IN TABLE\n";
				else cout << x << " NOT IN TABLE\n";
			}
			else if (s == "REMOVE") table->remove(x);
			else break;
		}
	}

	delete table;
	return 0;
}
