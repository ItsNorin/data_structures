#include "graph.h"

Graph::Node::Node() 
{}

Graph::Node::Node(const PointT point)
	: point(point)
{}


bool Graph::Node::operator==(const Graph::Node &n) const { return point == n.point; }
bool Graph::Node::operator!=(const Graph::Node &n) const { return point != n.point; }


void Graph::Node::linkThis(Node *n, int weight) {
	for (unsigned i = 0; i < links.size(); i++) {
		if (links[i].first == n) {
			links[i].second = weight;
			return;
		}
	}
	links.push_back(std::make_pair(n, weight));
}

void Graph::Node::unlinkThis(Node *n) {
	for (auto it = links.begin(); it < links.end(); it++) 
		if ((*it).first == n) {
			links.erase(it);
			return;
		}
}


void Graph::Node::link(Node *n, int weight) {
	this->linkThis(n, weight);
	n->linkThis(this, weight);
}

void Graph::Node::unlink(Node *n) {
	n->unlinkThis(this);
	this->unlinkThis(n);
}


/*
 * GRAPH METHODS
*/

bool Graph::insert(const PointT &point) {
	return map_.insert(std::make_pair(point, Graph::Node(point))).second;
}

void Graph::link(const PointT &point, const PointT &neighbor, const int weight) {
	map_[point].link(&map_[neighbor], weight);
}

void Graph::unlink(const PointT & point, const PointT & neighbor) {
	map_[point].unlink(&map_[neighbor]);
}
