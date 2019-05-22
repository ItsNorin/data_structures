#pragma once
#include <list>

#define HASHMAP_BASIC_SIZE 64 // default starting size for hashmap
#define LONGEST_ACCEPTABLE_CHAIN_LENGTH 4 // longest length any chain can be, will re-size table when a chain exceeds this limit
#define GROWTH_RATE 3/2 // new table size will be the number of elements it currently holds * GROWTH_RATE

template<typename KeyT, typename DataT>
class ChainedHashMap {
public:
	// element in hashmap
	struct Entry {
		KeyT key;
		DataT data;
		Entry(KeyT key, DataT data) : key(key), data(data) {}
	};

public:
	// creates a hashmap
	// must be given a hasher function, which can hash the key into an unsigned integer
	// size is the starting width of the hashmap, will change if the hashmap gets a chain longer than LONGEST_ACCEPTABLE_CHAIN_LENGTH
	ChainedHashMap(unsigned(*hasher)(const KeyT &), unsigned size = HASHMAP_BASIC_SIZE);
	~ChainedHashMap();

	// insert an entry into hashmap
	// if the hashmap has become overpopulated, will resize to increase searching efficency
	// true if inserted, false if key already exists
	bool insert(const Entry &e);
	// insert element into hashmap by key and its data
	bool insert(const KeyT key, const DataT data);

	// remove a key from hashmap
	// true if removed, false if not found
	bool remove(KeyT key);

	// search for the data associated with the given key
	// returns pointer to key's associated data if found, if not found, returns nullptr
	DataT * find(KeyT key) const;

	// number of entries hashmap is storing
	unsigned entries() const;


protected:
	// hashmap body
	std::list<Entry> *table_;
	unsigned tableSize_;

	// hashing function, must convert key to an unsigned int
	unsigned(*hasher_)(const KeyT &);

	// number of elements hashmap is storing
	unsigned entryCount_;

	// longest chain, used to resize hashmap
	unsigned longestChainLength_;

protected:
	// resizes hashmap if chains are growing too long
	void resize_();
};



template<typename KeyT, typename DataT>
ChainedHashMap<KeyT, DataT>::ChainedHashMap(unsigned(*hasher)(const KeyT &), unsigned size)
	: hasher_(hasher), table_(new std::list<Entry>[size]), tableSize_(size), entryCount_(0), longestChainLength_(0)
{}


template<typename KeyT, typename DataT>
ChainedHashMap<KeyT, DataT>::~ChainedHashMap() {
	delete[] table_;
}



template<typename KeyT, typename DataT>
inline bool ChainedHashMap<KeyT, DataT>::insert(const Entry &e) {
	std::list<Entry> &chosenList = table_[hasher_(e.key) % tableSize_];
	// ensure key doesnt exist in table
	for (auto it = chosenList.begin(); it != chosenList.end(); ++it) {
		if ((*it).key == e.key)
			return false;
	}
	// insert entry into table
	chosenList.push_back(e);
	
	// clean up table if needed
	longestChainLength_ = (chosenList.size() > longestChainLength_) ? chosenList.size() : longestChainLength_;
	entryCount_++;

	resize_();
	return true;
}


template<typename KeyT, typename DataT>
inline bool ChainedHashMap<KeyT, DataT>::insert(const KeyT key, const DataT data) {
	return insert(Entry(key, data));
}


template<typename KeyT, typename DataT>
inline bool ChainedHashMap<KeyT, DataT>::remove(KeyT key) {
	std::list<Entry> &chosenList = table_[hasher_(key) % tableSize_];

	for (auto it = chosenList.begin(); it != chosenList.end(); ++it) {
		if ((*it).key == key) {
			chosenList.erase(it);
			--entryCount_;
			return true;
		}
	}
	return false;
}


template<typename KeyT, typename DataT>
inline DataT * ChainedHashMap<KeyT, DataT>::find(KeyT key) const {
	std::list<Entry> &chosenList = table_[hasher_(key) % tableSize_];

	for (auto it = chosenList.begin(); it != chosenList.end(); ++it) {
		if ((*it).key == key) 
			return &(*it).data;
	}
	return nullptr;
}


template<typename KeyT, typename DataT>
inline unsigned ChainedHashMap<KeyT, DataT>::entries() const { 
	return entryCount_; 
}


template<typename KeyT, typename DataT>
inline void ChainedHashMap<KeyT, DataT>::resize_() {
	if (longestChainLength_ > LONGEST_ACCEPTABLE_CHAIN_LENGTH) {
		unsigned newTableSize = (unsigned)(entryCount_ * GROWTH_RATE);
		std::list<Entry> *newTable = new std::list<Entry>[newTableSize];

		for (unsigned i = 0; i < tableSize_; i++) {
			std::list<Entry> &originList = table_[i];
			for (auto it = originList.begin(); it != originList.end(); ++it)
				newTable[hasher_((*it).key) % newTableSize].push_back((*it));
		}

		longestChainLength_ = 1;
		delete[] table_;
		table_ = newTable;
		tableSize_ = newTableSize;
	}
}
