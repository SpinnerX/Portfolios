#include "/public/read.h"
#include <initializer_list>
// #include "list.h" // NOTE: Just called my header list.h to simplify naming.
using namespace std;

//YOU: You will be doing something here after you finish Part 1 called List
template<typename _Tp>
struct Node {
	_Tp data; //Data Payload
	Node<_Tp>* next = nullptr; //Next node in the list
	Node<_Tp>* prev = nullptr; //Previous node in the list
};

template<typename _Tp>
class List {
public:
	List() {}
	List(std::initializer_list<_Tp> list){
		for(auto iter = list.begin(); iter != list.end(); iter++) this->push_back(*iter);
	}

	~List() {
		Node<_Tp> *temp = head;
		while (temp != nullptr) {
			Node<_Tp> *copy = temp;
			temp = temp->next;
			delete copy;
		}
		_size = 0;
	}

	size_t size() const { return _size; }
	_Tp front() const {
		if (!head) return {};
		return head->data;
	}

	_Tp back() const {
		if (!tail) return {};
		return tail->data;
	}

	bool find(_Tp val) {
		for (Node<_Tp> *current = head; current; current = current->next){
			if (current->data == val) return true;
		}

		return false;
	}

	void push_back(_Tp val) {
		if (!head or !tail) head = tail = new Node<_Tp>{val};
		else {
			Node<_Tp> *temp = new Node<_Tp>{val, nullptr, tail};
			tail->next = temp;
			tail = temp; // Acts as if we're doing, tail = tail->next;
		}

		_size++;
	}
	
	// Pop Front: Delete the front of LL and setting previous ptr to nullptr, and next value as the new head.
	// Pop Back: Delete values in back of LL, and setting the new tail to previous ptr and the current tail to nullptr.
	void pop_back() {
		if (head == tail) {
			delete head;
			head = tail = nullptr;
		}
		else {
			tail = tail->prev;
			delete tail->next;
			tail->next = nullptr;
		}
		_size--;
	}
	
	// Adds to the beginning of LL, and shifting the next ptrs.
	void push_front(_Tp val) {
		if (!head or !tail) {
			head = tail = new Node<_Tp>{val};
		}
		else {
			Node<_Tp> *new_dude = new Node<_Tp>{val, head, nullptr};
			head->prev = new_dude;
			head = new_dude;
		}
		_size++;
	}

	void pop_front() {
		if (head == tail) {
			delete head;
			head = tail = nullptr;
		}
		else {
			head = head->next;
			delete head->prev;
			head->prev = nullptr;
		}
		_size--;
	}

	friend std::ostream& operator<<(std::ostream& outs, const List<_Tp>& l){
		for(Node<_Tp>* current = l.head; current; current = current->next){
			outs << current->data << '\n';
		}
		return outs;
	}
 private:
	Node<_Tp>* head = nullptr;
	Node<_Tp>* tail = nullptr;
	size_t _size = 0;
};


//2.5 points
//YOU: Make a Queue class that implements:
//1) Front() - returns the first value in the queue
//2) Push() - adds a value to the end of the queue
//3) Pop() - deletes the value at the front of the queue
//4) Size() - returns how many elements are in the queue
//Use the std::list class to implement it
//Errors: If they call front or pop on an empty queue, die()

void die() {
	cout << "BAD INPUT!\n";
	exit(1);
}

class Queue {
	List<int> my_data;
public:
	size_t size() { //YOU:
		return my_data.size();
	}

	int front() {  //YOU:
		if(size() == 0) return 0;
		return my_data.front();
	}
	void pop() { //YOU:
		if(size() == 0){
			std::cout << "BAD INPUT!\n";
			return;
		}
		my_data.pop_front();
	}
	void push(int new_data) { //YOU:
		my_data.push_back(new_data);
	}
};

//2.5 points
//YOU: Make a Stack class that implements:
//1) Front() - returns the first value in the stack
//2) Push() - adds a value to the front of the stack
//3) Pop() - deletes the value at the front of the stack
//4) Size() - returns how many elements are in the stack
//Use the std::list class to implement it
//Errors: If they call front or pop on an empty stack, die()

class Stack {
	List<int> my_data;
public:
	size_t size() { //YOU:
		return my_data.size();
	}
	int top() {  //YOU:
		if(size() == 0) return 0;
		return my_data.front();
	}
	void pop() { //YOU:
		if(size() == 0){
			std::cout << "BAD INPUT!\n";
			return;
		}
		my_data.pop_front();
	}
	void push(int new_data) { //YOU:
		my_data.push_front(new_data);
	}
};

//Stop here and make sure you are passing all test cases before proceeding

//5 points
//YOU: Now make a Queue2 and Stack2 class that is the same as the above
// but using a linked list class that you write!
//The List class needs:
// push_front
// pop_front
// push_back
// front
// size
//When you are done, switch the std::list in Queue and Stack below with 
// your own List class.
// (Your List class will need to be above Queue and Stack so that they can see it.)
// If you did it right, you will still pass all test cases!

int main() {
	int choice = read("1. Queue\n2. Stack\n");
	if (choice == 1) {
		Queue my_queue; 
		while (true) {
			int choice = read("1. Push\n2. Pop\n3. Top\n4. Size\n");
			if (choice == 1) {
				int num = read("Enter number to push:\n");
				my_queue.push(num);
			}
			else if (choice == 2) {
				if(my_queue.size() == 0) die();
				my_queue.pop();
			}
			else if (choice == 3) {
				if(my_queue.size() == 0) die();
				cout << my_queue.front() << endl;
			}
			else if (choice == 4) {
				cout << my_queue.size() << endl;
			}
			else die();
		}
	}
	else if (choice == 2) {
		Stack my_stack; 
		while (true) {
			int choice = read("1. Push\n2. Pop\n3. Top\n4. Size\n");
			if (choice == 1) {
				int num = read("Enter number to push:\n");
				my_stack.push(num);
			}
			else if (choice == 2) {
				if(my_stack.size() == 0) die();
				my_stack.pop();
			}
			else if (choice == 3) {
				if(my_stack.size() == 0) die();
				cout << my_stack.top() << endl;
			}
			else if (choice == 4) {
				cout << my_stack.size() << endl;
			}
			else die();
		}
	}
	else die(); //Bad choice
}
