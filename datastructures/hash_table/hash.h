#include <vector>
#include <functional>
#include <set>
#include <boost/multiprecision/miller_rabin.hpp> //prime testing 
using namespace std;

//using namespace boost::random;
using namespace boost::multiprecision;

//Base class that provides an interface the two subclasses will inherit
//This is an abstract class that does nothing
//But it allows us to avoid copy/pasting code in main because the interface is the same for both hash tables

int lowestPrime(int x) {
	int nextPrime = x - 1;
	while (!miller_rabin_test(nextPrime, 23)) {
		nextPrime--;
	}
	return nextPrime;
}
int f(int x, int y) {
	return 1 + (x % y);
}

template <class T, size_t SIZE>
class Hash {
  public:
	virtual ~Hash() {}
	virtual void insert(T new_data) = 0;
	virtual void remove(T old_data) = 0;
	virtual bool search(T test_data) const = 0;
	virtual void change(T old_data, T new_data) = 0;
};

//YOU: Write three hash tables, one for linear probing, one for chaining, one for double hashing
template <class T, size_t SIZE>
class LinearProbingHash : public Hash<T, SIZE> { 
	//This bit of magic makes three constants: STATUS::OPEN, STATUS::FILLED, and STATUS::DELETED
	//OPEN means the bucket has never held a value in it
	//FILLED means it currently holds a valid value in it
	//DELETED means the value in it has been deleted and not replaced yet
	enum class STATUS : char {OPEN, FILLED, DELETED};
	std::vector<T> data;
	std::vector<STATUS> status; //Each element can be STATUS::OPEN, STATUS::FILLED or STATUS::DELETED
  public:
	LinearProbingHash() {
		data.resize(SIZE); //Set the vector to be SIZE big - SIZE is a template parameter
		status.resize(SIZE, STATUS::OPEN); //Set the state of every bucket to STATUS::OPEN
	}
	void insert(T new_data) {
		//Step 1 - Compute the initial bucket to try
		size_t address = std::hash<T> {}(new_data) % SIZE;
		//Step 2 - We will start at the bucket at "address" and then move to the right,
		//  looping around to the start if necessary, to find first available bucket
		//  Either an OPEN bucket or a DELETED bucket will do.
		//TODO: Make this not infinite loop on a full hash table  -----DONE-----
		//while (status.at(address) == STATUS::FILLED)

		unsigned int counter = 0;
			
		if (search(new_data)) {
			return;
		}
		while (true) {
			if (status.at(address) == STATUS::OPEN) {
				data.at(address) = new_data;
				status.at(address) = STATUS::FILLED;
				return;
			}
			address = (address + 1) % SIZE; //Move one bucket to the right, looping around
			counter++;

			//if (status.at(address) == STATUS::FILLED) break;
			if (counter == SIZE) {
				return;
			}
		}
	}
	//Ok, we've found an open spot, write the data in and mark it as filled
	//data.at(address) = new_data;
	//status.at(address) = STATUS::FILLED;

	//YOU:
	void remove(T old_data) {
		size_t address = std::hash<T> {}(old_data) % SIZE;

		unsigned int counter = 0;
		while (true) {
			if (status.at(address) == STATUS::FILLED) {
				if (data.at(address) == old_data) {
					status.at(address) = STATUS::DELETED;
					//status.at(address) == STATUS::OPEN;
					//return here?
					return;
				}
			}
			address = (address + 1) % SIZE; //Move one bucket to the right, looping around
			counter++;

			//if (status.at(address) == STATUS::FILLED) break;
			if (counter == SIZE) return;

		}
	}
	//YOU:
	bool search(T test_data) const {
		//if (data.at(address) == )
		//cout << "Found: " << data.at(address) << endl;
		size_t address = std::hash<T> {}(test_data) % SIZE;

		unsigned int counter = 0;
		while (true) {
			if (status.at(address) == STATUS::FILLED) {
				if (data.at(address) == test_data) {
					return true;
				}
			}
			address = (address + 1) % SIZE; //Move one bucket to the right, looping around
			counter++;

			//if (status.at(address) == STATUS::FILLED) break;
			if (counter == SIZE) {
				break;
			}

		}
		return false;
		
	}

	//YOU:
	void change(T old_data, T new_data) {
		//Remember, only change it if it actually is in the hash table
		if (search(old_data)) {
			remove(old_data);
			insert(new_data);
		}
	}

}; 

//YOU: Do the same as the above, with with hashing with chaining instead of linear probing
//You can do chaining either with LLs or with BSTs
template <class T, size_t SIZE>
class ChainingHash : public Hash<T, SIZE> {
	//YOU:
	vector<set<T>> table;
  public:
	//YOU:
	ChainingHash() {
		table.resize(SIZE);
	}
	//YOU:
	void insert(T new_data) override {
		size_t address = std::hash<T> {}(new_data) % SIZE;
		table.at(address).insert(new_data);
	}
	//YOU:
	void remove(T old_data) override {
		size_t address = std::hash<T> {}(old_data) % SIZE;
		table.at(address).erase(old_data);
	}
	//YOU:
	bool search(T test_data) const override {
		size_t address = std::hash<T> {}(test_data) % SIZE;
		return table.at(address).count(test_data);
		//return table.at(address).at(count);
	}
	//YOU:
	void change(T old_data, T new_data) override {
		if (search(old_data)) {
			remove(old_data);
			insert(new_data);
		}
	}
};

//YOU: Write a double hashing class here, following the pattern above

/* template <class T, size_t SIZE>
   class Hash {
   public:
   virtual ~Hash() {}
   virtual void insert(T new_data) = 0;
   virtual void remove(T old_data) = 0;
   virtual bool search(T test_data) const = 0;
   virtual void change(T old_data, T new_data) = 0;
   */
//YOU: Write three hash tables, one for linear probing, one for chaining, one for double hashing
//class LinearProbingHash : public Hash<T,SIZE> {
template <class T, size_t SIZE>
class DoubleHash : public Hash<T, SIZE> { 
	//This bit of magic makes three constants: STATUS::OPEN, STATUS::FILLED, and STATUS::DELETED
	//OPEN means the bucket has never held a value in it
	//FILLED means it currently holds a valid value in it
	//DELETED means the value in it has been deleted and not replaced yet
	enum class STATUS : char {OPEN, FILLED, DELETED};
	std::vector<T> data;
	std::vector<STATUS> status; //Each element can be STATUS::OPEN, STATUS::FILLED or STATUS::DELETED
	int prime = lowestPrime(SIZE);
  public:
	DoubleHash() {
		data.resize(SIZE); //Set the vector to be SIZE big - SIZE is a template parameter
		status.resize(SIZE, STATUS::OPEN); //Set the state of every bucket to STATUS::OPEN
	}
	void insert(T new_data) override {
		//Step 1 - Compute the initial bucket to try
		size_t address = std::hash<T> {}(new_data) % SIZE;
		//Step 2 - We will start at the bucket at "address" and then move to the right,
		//  looping around to the start if necessary, to find first available bucket
		//  Either an OPEN bucket or a DELETED bucket will do.
		//TODO: Make this not infinite loop on a full hash table  -----DONE-----
		//while (status.at(address) == STATUS::FILLED)
		size_t step = f(new_data, prime);
		unsigned int counter = 0;
		if (search(new_data)) return;

		while (true) {

			if (status.at(address) == STATUS::OPEN) {
				data.at(address) = new_data;
				status.at(address) = STATUS::FILLED;
				counter++; 
				return;
			}
			if (status.at(address) == STATUS::FILLED) {
				address = (address + step) % SIZE; //Move one bucket to the right, looping around
				counter++;
			}
			//if (status.at(address) == STATUS::FILLED) break;
			if (counter == SIZE) return;
		}
		//Ok, we've found an open spot, write the data in and mark it as filled
		//data.at(address) = new_data;
		//status.at(address) = STATUS::FILLED;
	}
	//YOU:
	void remove(T old_data) override {
		size_t address = std::hash<T> {}(old_data) % SIZE;
		size_t step = f(old_data, prime);

		unsigned int counter = 0;
		while (true) {
			if (status.at(address) == STATUS::FILLED) {
				if (data.at(address) == old_data) status.at(address) = STATUS::DELETED;
				return;
			}
			address = (address + step) % SIZE; //Move one bucket to the right, looping around
			counter++;

			//if (status.at(address) == STATUS::FILLED) break;
			if (counter == SIZE) return;
		}
	} 
	//YOU:
	bool search(T test_data) const override {
		//if (data.at(address) == )
		//cout << "Found: " << data.at(address) << endl;
		size_t address = std::hash<T> {}(test_data) % SIZE;
		size_t step = f(test_data, prime);

		unsigned int counter = 0;
		while (true) {
			if (status.at(address) == STATUS::FILLED) {
				if (data.at(address) == test_data) {
					return true;
				}
			}
			address = (address + step) % SIZE; //Move one bucket to the right, looping around
			counter++;

			//if (status.at(address) == STATUS::FILLED) break;
			if (counter == SIZE) {
				break;
			}
		}
			return false;
	}

		//YOU:
		void change(T old_data, T new_data) override {
			//Remember, only change it if it actually is in the hash table
			if (search(old_data)) {
				remove(old_data);
				insert(new_data);
			}
		}
};
