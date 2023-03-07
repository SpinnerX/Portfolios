#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

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
