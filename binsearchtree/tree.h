#pragma once

// Binary Search Tree
// Written by ItsNorin      https://github.com/ItsNorin/

// an ALV tree optimized for quick searching O(log n) and accessing of elements O(1)

#include <initializer_list>

// #define TREE_PRINTING // uncomment to be able to print tree in console with std::cout

#ifdef TREE_PRINTING
#define LEAF_SPACING 5
#include <iostream>
#endif

// maximum of two numbers
template<typename T> T max(const T &a, const T &b) { return (a > b) ? a : b; }


// type must have ==, >=, >, <, <= operators defined
// if TREE_PRINTING is defined, must also be printable with std::cout <<
template<typename T>
class SearchTree {
protected:
	class Node {
	public:
		T key;

		Node *left, *right,
			 *parent;
		
		int index;  // node's index in the referance array
		int height; // height of node

		Node(const T key = T(), Node *parent = nullptr) 
			: key(key), left(nullptr), right(nullptr), parent(parent), height(0) {
		}
	};

protected:
	Node *head_; // head of tree
	Node **arr_; // array representation of tree
	int size_;   // number of elements in tree
	bool arrValid_;

protected:
	// number of children a node has
	static int numChildren_(const Node *n_ptr);

	// increments count by number of nodes below this node
	static void addChildren_(const Node *n_ptr, int &count);

	// search the node and its children for given element, returns index
	// todo: try to optimize for minimal memory usage
	static int find_(const T &d, const Node *n_ptr);

	// creates an array from node and its children, returns size
	int generateArr_(Node **&arr);

	// adds all children to given array
	static int addToArr_(Node *n_ptr, Node **&arr, int i);
	
	// height of a node
	static int height_(const Node *n_ptr) { 
		return (n_ptr == nullptr) ? -1 : n_ptr->height; 
	}

	// sets a node's height
	static int calcHeight_(Node *n_ptr) { 
		n_ptr->height = max(height_(n_ptr->right), height_(n_ptr->left)) + 1;
		return n_ptr->height;
	}

	// how balanced the subtree is, positive is right heavy, negative left heavy
	static int balanceFactor_(const Node *n_ptr) { 
		return height_(n_ptr->right) - height_(n_ptr->left);  
	}

	// rotates subtree right, returns new parent
	static Node * rotateRight_(Node *root); 

	// rotates subtree left, returns new parent
	static Node * rotateLeft_(Node *root); 

	// balances a subtree
	void balanceSubtree_(Node *n_ptr);

public:
	// initialize a tree
	SearchTree();

	// create a tree from an initializer list
	SearchTree(const std::initializer_list<T> &list);

	// create a tree from an array
	SearchTree(const T arr[], const unsigned size);

	~SearchTree();

	// number of elements in tree
	int size() const;

	// insert element into tree
	// false if key already exists in tree, element was not inserted
	// true if successful
	bool insert(const T key);

	// removes an element from tree
	// false if key not found, true if removed
	bool remove(const T key);

	// delete all elements in tree
	void clear();

	// index of given value if found, -1 if not found
	int find(const T key) const;

	// access a given element in tree
	// order will likely change when an element is inserted or deleted
	T & operator[](const unsigned i) { return arr_[i]->key; }
	T operator[](const unsigned i) const { return arr_[i]->key; }

#ifdef TREE_PRINTING
protected:
	void print_(Node *root, int space);

public:
	void print() { print_(head_, 0); }

#endif // TREE_DEBUG

};



template<typename T>
void SearchTree<T>::addChildren_(const Node *n_ptr, int &count) {
	count++;
	if (n_ptr->left != nullptr)
		addChildren_(n_ptr->left, count);
	if (n_ptr->right != nullptr)
		addChildren_(n_ptr->right, count);
}

template<typename T>
inline int SearchTree<T>::numChildren_(const Node *n_ptr) {
	int c = 0;
	addChildren_(n_ptr, c);
	return c;
}



template<typename T>
int SearchTree<T>::addToArr_(Node *n_ptr, Node **&arr, int i) {
	if (n_ptr == nullptr)
		return i;

	if (n_ptr->left != nullptr)
		i = addToArr_(n_ptr->left, arr, i);

	arr[i] = n_ptr;
	n_ptr->index = i;
	++i;

	if (n_ptr->right != nullptr)
		i = addToArr_(n_ptr->right, arr, i);

	return i;
}



template<typename T>
typename SearchTree<T>::Node *SearchTree<T>::rotateRight_(Node *root) {
	Node *newRoot = root->left;
	newRoot->parent = root->parent;
	root->left = newRoot->right;

	if (root->left != nullptr)
		root->left->parent = root;

	newRoot->right = root;
	root->parent = newRoot;

	if (newRoot->parent != nullptr) {
		if (newRoot->parent->right == root) 
			newRoot->parent->right = newRoot;
		else 
			newRoot->parent->left = newRoot;
	}

	calcHeight_(root);
	calcHeight_(newRoot);

	return newRoot;
}

template<typename T>
typename SearchTree<T>::Node * SearchTree<T>::rotateLeft_(Node *root) {
	Node *newRoot = root->right;
	newRoot->parent = root->parent;
	root->right = newRoot->left;

	if (root->right != nullptr)
		root->right->parent = root;

	newRoot->left = root;
	root->parent = newRoot;

	if (newRoot->parent != nullptr) {
		if (newRoot->parent->right == root) 
			newRoot->parent->right = newRoot;
		else 
			newRoot->parent->left = newRoot;
	}

	calcHeight_(root);
	calcHeight_(newRoot);

	return newRoot;
}

template<typename T>
void SearchTree<T>::balanceSubtree_(Node * n_ptr) {
	calcHeight_(n_ptr);

	int balance = balanceFactor_(n_ptr);

	if (balance < -1) {
		if (height_(n_ptr->left->left) >= height_(n_ptr->left->right)) {
			n_ptr = rotateRight_(n_ptr);
		}
		else {
			n_ptr->left = rotateLeft_(n_ptr->left);
			n_ptr = rotateRight_(n_ptr);
		}
	}
	else if (balance > 1) {
		if (height_(n_ptr->right->right) >= height_(n_ptr->right->left)) {
			n_ptr = rotateLeft_(n_ptr);
		}
		else {
			n_ptr->right = rotateRight_(n_ptr->right);
			n_ptr = rotateLeft_(n_ptr);
		}
	}

	if (n_ptr->parent != nullptr) {
		balanceSubtree_(n_ptr->parent);
	}
	else
		head_ = n_ptr;
}

template<typename T>
inline SearchTree<T>::SearchTree() 
	: size_(0), head_(nullptr), arr_(nullptr), arrValid_(false) {
}

template<typename T>
inline SearchTree<T>::SearchTree(const std::initializer_list<T>& list)
	: size_(0), head_(nullptr), arr_(nullptr), arrValid_(false) {
	for (unsigned i = 0; i < list.size(); i++)
		insert(list.begin()[i]);
}


template<typename T>
inline int SearchTree<T>::generateArr_(Node **&arr) {
	int size = numChildren_(head_);
	if (arr != nullptr)
		delete[] arr;
	arr = new Node*[size];
	addToArr_(head_, arr, 0);
	return size;
}


template<typename T>
inline bool SearchTree<T>::insert(const T key) {
	if (head_ == nullptr)
		head_ = new Node(key);
	else {
		Node *n_ptr = head_, 
			 *n_prev = nullptr;

		bool run = true;
		while (run) {
			if (n_ptr->key == key)
				return false;

			n_prev = n_ptr;

			bool goLeft = n_ptr->key > key;
			n_ptr = goLeft ? n_ptr->left : n_ptr->right;

			if (n_ptr == nullptr) {
				if (goLeft) 
					n_prev->left = new Node(key, n_prev);
				else 
					n_prev->right = new Node(key, n_prev);
				
				balanceSubtree_(n_prev);
				run = false;
			}
		}
	}

	size_ = generateArr_(arr_);

	return true;
}

template<typename T>
bool SearchTree<T>::remove(const T key) {
	if (head_ != nullptr) {
		Node *n_ptr = head_, *parent = head_, *child = head_;
		Node *delNode = nullptr;
			
		while (child != nullptr) {
			parent = n_ptr;
			n_ptr = child;
			child = (key >= n_ptr->key) ? n_ptr->right : n_ptr->left;
			if (key == n_ptr->key)
				delNode = n_ptr;
		}

		if (delNode != nullptr) {
			delNode->key = n_ptr->key;

			child = (n_ptr->left != nullptr) ? n_ptr->left : n_ptr->right;

			if (head_->key == key)
				head_ = child;
			else {
				if (parent->left == n_ptr) 
					parent->left = child;
				else 
					parent->right = child;
		
				balanceSubtree_(parent);
			}

			size_ = generateArr_(arr_);
			return true;
		}
	}
	return false;
}


template<typename T>
inline int SearchTree<T>::find_(const T &key, const Node *n_ptr) {
	if (n_ptr->key == key)
		return n_ptr->index;

	if (key < n_ptr->key) {
		if (n_ptr->left != nullptr)
			return find_(key, n_ptr->left);
	}
	else {
		if (n_ptr->right != nullptr)
			return find_(key, n_ptr->right);
	}
	return -1;
}

template<typename T>
inline void SearchTree<T>::clear() {
	for (int i = 0; i < size_; i++)
		delete arr_[i];
	if (arr_ != nullptr)
		delete[] arr_;
	size_ = 0;
	arr_ = nullptr;
	head_ = nullptr;
}

template<typename T>
inline int SearchTree<T>::find(const T key) const {
	return (head_ == nullptr) ? -1 : find_(key, head_);
}

#ifdef TREE_PRINTING
template<typename T>
inline void SearchTree<T>::print_(Node *root, int space) {
	if (root != nullptr) {
		// Increase distance between levels  
		space += LEAF_SPACING;

		// right child first  
		print_(root->right, space);

		// print current node after space 
		std::cout << '\n';
		for (int i = LEAF_SPACING; i < space; i++)
			std::cout << ' ';
		std::cout << root->key << '\n';

		// left child  
		print_(root->left, space);
	}
}
#endif

template<typename T>
inline SearchTree<T>::SearchTree(const T arr[], const unsigned size)
	: size_(0), head_(nullptr), arr_(nullptr), arrValid_(false) {
	for (unsigned i = 0; i < size; i++)
		insert(arr[i]);
}

template<typename T>
inline SearchTree<T>::~SearchTree() {
	clear(); 
}

template<typename T>
inline int SearchTree<T>::size() const {
	return size_;
}