#pragma once
#include <vector>
#include <unordered_map>
#include "pointTypes.h"

// TODO: use to turn into templates
// point in nDimensional space, can also contain any associated data
typedef Point2D PointT; 

// graph of any nDimensional space
// supports Dijkstra and A* pathfinding
class Graph {
protected:
	// individual node in graph, refering to a point in space of type PointT
	// contains links to other nodes in graph, and weight of each link
	class Node;

protected:
	// map of all points in graph
	std::unordered_map<PointT, Node> map_;

public:
	bool insert(const PointT &point);
	void link(const PointT &point, const PointT &neighbor, const int weight);
	void unlink(const PointT &point, const PointT &neighbor);
};



// individual node in graph, refering to a point in space of type PointT
// contains links to other nodes in graph, and weight of each link
class Graph::Node {
public:
	Node();
	// create a node for a point in space
	Node(const PointT point);

	// makes two nodes neighbors
	// each node will insert the other and the given weight into their links vector
	// if node is already neighbors, will only update weight
	void link(Node *n, int weight = 1);

	// unlinks two nodes
	void unlink(Node *n);

	bool operator==(const Graph::Node &n) const;
	bool operator!=(const Graph::Node &n) const;
public:
	// point this node represents
	PointT point;

	// any neighbors this node might have and a weight to get to them
	std::vector<std::pair<Node *, int>> links;

private:
	// make only this node have given graph node as a neighbor
	// if node is already neighbors, will only update weight
	void linkThis(Node *n, int weight);

	// removes only this node's link to another graph node
	void unlinkThis(Node *n);
};