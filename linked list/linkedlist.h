#pragma once
#include <initializer_list>

template<typename T>
class List {
protected:
	// linked list node
	class Node {
	public:
		T value;
		Node *next, *prev;
		Node(const T value = T()) : value(value), next(nullptr), prev(nullptr) {}
	};

public:
	class Iterator;

protected:
	Node head_, tail_;
	unsigned size_;

protected:
	Node * at_(const unsigned index);

public:
	List();
	List(List &list);
	List(const std::initializer_list<T> &list);
	~List();

	unsigned size() const { return size_; }

	// clears entire list
	void clear();

	// inserts element at given position, pushes rest of the list forward
	void insert(const unsigned i, const T & value);

	// removes an element from list
	void remove(const unsigned i);

	// access an element in the list
	T & operator[](const unsigned i);
	// get copy of an element in the list
	T operator[](const unsigned i) const;

	// iterator pointing to first element of list
	Iterator start();
	// iterator pointing to end + 1 of list
	Iterator end();
};


// Iterator for linked lists
template<typename T>
class List<T>::Iterator {
	protected:
		Node *it_;

	public:
		Iterator() : it_(nullptr) {}
		Iterator(Node *ptr) : it_(ptr) {}
		Iterator(const Iterator &it) : it_(it.it_) {}

		// advance iterator
		Iterator next() const { return it_->next; }
		// move iterator back
		Iterator prev() const { return it_->prev; }

		Iterator & operator++();
		Iterator operator++(int);

		Iterator & operator--();
		Iterator operator--(int);
		
		// dereferance iterator
		T & operator*() { return it_->value; }
		T operator*() const { return it_->value; }

		Node * operator=(Node * ptr) { return this->it_ = ptr; }

		bool operator==(const Iterator &it) const { return this->it_ == it.it_; }
		bool operator!=(const Iterator &it) const { return this->it_ != it.it_; }
};




template<typename T>
inline typename List<T>::Iterator & List<T>::Iterator::operator++() {
	it_ = it_->next;
	return *this;
}

template<typename T>
inline typename List<T>::Iterator List<T>::Iterator::operator++(int) {
	Iterator temp = *this;
	it_ = it_->next;
	return temp;
}

template<typename T>
inline typename List<T>::Iterator & List<T>::Iterator::operator--() {
	it_ = it_->prev;
	return *this;
}

template<typename T>
inline typename List<T>::Iterator List<T>::Iterator::operator--(int) {
	Iterator temp = *this;
	it_ = it_->prev;
	return temp;
}






template<typename T>
inline List<T>::List() : size_(0) {
	head_.next = &tail_;
	tail_.prev = &head_;
}

template<typename T>
List<T>::List(List & list) : size_(list.size_) {
	Node *pos = &head_;
	Node *listIt = list.head_.next;

	for (unsigned i = 0; i < size_; ++i) {
		pos->next = new Node(listIt->value);
		pos->next->prev = pos;
		pos = pos->next;
		listIt = listIt->next;
	}
	tail_.prev = pos;
	pos->next = &tail_;
}

template<typename T>
List<T>::List(const std::initializer_list<T> & list) : size_(list.size()) {
	Node *pos = &head_;

	for (unsigned i = 0; i < size_; ++i) {
		pos->next = new Node(*(list.begin() + i));
		pos->next->prev = pos;
		pos = pos->next;
	}
	tail_.prev = pos;
	pos->next = &tail_;
}



template<typename T>
void List<T>::clear(){
	Node *it = head_.next;
	while (it != &tail_) {
		Node *temp = it;
		it = it->next;
		delete temp;
	}
	size_ = 0;
}

template<typename T>
inline List<T>::~List() { clear(); }



template<typename T>
inline typename List<T>::Node * List<T>::at_(const unsigned index) {
	Node *temp = head_.next;
	for (unsigned i = 0; i < index && i < size_; i++)
		temp = temp->next;
	return temp;
}


template<typename T>
void List<T>::insert(const unsigned i, const T & value) {
	Node * insertBefore = at_(i);
	Node * toInsert = new Node(value);

	toInsert->next = insertBefore;
	toInsert->prev = insertBefore->prev;

	toInsert->next->prev = toInsert;
	toInsert->prev->next = toInsert;

	++size_;
}

template<typename T>
void List<T>::remove(const unsigned i) {
	Node * toDel = at_(i);

	toDel->prev->next = toDel->next;
	toDel->next->prev = toDel->prev;

	delete toDel;

	--size_;
}

template<typename T>
inline T & List<T>::operator[](const unsigned i) { return at_(i)->value; }

template<typename T>
inline T List<T>::operator[](const unsigned i) const { return at_(i)->value; }

template<typename T>
inline typename List<T>::Iterator List<T>::start() { return head_.next; }

template<typename T>
inline typename List<T>::Iterator List<T>::end() { return &tail_; }