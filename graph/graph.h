#pragma once
#include <vector>
#include <unordered_map>
#include "pointTypes.h"

// TODO: use to turn into templates
// point in nDimensional space, can also contain any associated data
typedef Point2D PointT; 

// graph of any nDimensional space
// supports A* pathfinding
class Graph {
protected:
	// individual node in graph, refering to a point in space of type PointT
	// contains links to other nodes in graph, and weight of each link
	class Node;

protected:
	// map of all points in graph
	std::unordered_map<PointT, Node> map_;

public:
	// inserts a point into graph
	// true if insertion was successful
	bool insert(const PointT &point);

	// removes a point from graph after unlinking it
	// false if point wasn't in graph
	bool remove(const PointT &point);

	// links two points together with a given weight
	// if nodes are already linked, simply updates their weight
	void link(const PointT &point, const PointT &neighbor, const int weight = 1);

	// unlinks two points
	void unlink(const PointT &point, const PointT &neighbor);

	// true if point is in hashmap
	bool contains(const PointT &point) const;

	// links outgoing from node
	// will be empty if node does not exist
	const std::vector<std::pair<Node *, int>> linksOf(const PointT &p) const;

public:
	// finds a short path from start to end
	// returned vector will be empty if no path was found
	std::vector<PointT> pathfindDijkstra(const PointT &start, const PointT &goal) const;
};



// individual node in graph, refering to a point in space of type PointT
// contains links to other nodes in graph, and weight of each link
class Graph::Node {
	friend class Graph;

public:
	Node();
	// create a node for a point in space
	Node(const PointT point);

	PointT point() const;
	const std::vector<std::pair<Node *, int>> & links() const;

	// makes two nodes neighbors
	// each node will insert the other and the given weight into their links vector
	// if node is already neighbors, will only update weight
	void link(Node *n, int weight);

	// unlinks two nodes
	void unlink(Node *n);

	bool operator==(const Graph::Node &n) const;
	bool operator!=(const Graph::Node &n) const;

protected:
	// point this node represents
	PointT point_;

	// any neighbors this node might have and a weight to get to them
	std::vector<std::pair<Node *, int>> links_;

private:
	// make only this node have given graph node as a neighbor
	// if node is already neighbors, will only update weight
	void linkThis(Node *n, int weight);

	// removes only this node's link to another graph node
	void unlinkThis(Node *n);
};