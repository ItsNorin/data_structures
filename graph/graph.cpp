#include "graph.h"
#include <algorithm>

Graph::Node::Node() 
{}

Graph::Node::Node(const PointT point)
	: point_(point)
{}

PointT Graph::Node::point() const {
	return point_;
}

const std::vector<std::pair<Graph::Node*, int>>& Graph::Node::links() const {
	return links_;
}


bool Graph::Node::operator==(const Graph::Node &n) const { return point_ == n.point_; }
bool Graph::Node::operator!=(const Graph::Node &n) const { return point_ != n.point_; }


void Graph::Node::linkThis(Node *n, int weight) {
	for (unsigned i = 0; i < links_.size(); i++) {
		if (links_[i].first == n) {
			links_[i].second = weight;
			return;
		}
	}
	links_.push_back(std::make_pair(n, weight));
}

void Graph::Node::unlinkThis(Node *n) {
	for (auto it = links_.begin(); it < links_.end(); it++) 
		if ((*it).first == n) {
			links_.erase(it);
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

bool Graph::remove(const PointT &point) {
	Node *toDel = &map_[point];
	// remove all links to node
	while (toDel->links_.size() > 0) 
		toDel->links_[0].first->unlink(toDel);
		
	map_.erase(toDel->point_);
	return false; 
}

void Graph::link(const PointT &point, const PointT &neighbor, const int weight) {
	map_[point].link(&map_[neighbor], weight);
}

void Graph::unlink(const PointT &point, const PointT &neighbor) {
	map_[point].unlink(&map_[neighbor]);
}

bool Graph::contains(const PointT & point) const {
	return map_.find(point) != map_.end();
}

const std::vector<std::pair<Graph::Node*, int>> Graph::linksOf(const PointT &p) const {
	if (contains(p))
		return map_.at(p).links_;
	else
		return std::vector<std::pair<Graph::Node*, int>>();
}


// TODO: 
// test pathfinding to ensure it doesnt break when:
// - there is no valid path from start to goal
// - start and goal are the same
// - start and/or goal aren't in graph

std::vector<PointT> Graph::pathfindDijkstra(const PointT &start, const PointT &goal) const {
	if (contains(start) && contains(goal)) {
		std::unordered_map<PointT, Node> unexploredNodes = map_;
		std::unordered_map<PointT, int> distanceFromStart; // given any point, what is its distance from start
		std::unordered_map<PointT, const PointT*> pathParents; // previous point for each point in the path

		for (auto it = unexploredNodes.begin(); it != unexploredNodes.end(); it++) {
			distanceFromStart.insert({ (*it).first, INT_MAX }); // make each starting distance as large as possible
			pathParents.insert({ (*it).first, nullptr }); // insert an invalid parent point for each point in map
		}

		distanceFromStart[start] = 0; // distance from start -> start is 0

		while (unexploredNodes.size() > 0) {
			// get point with least distance from start
			const PointT *current = &(*unexploredNodes.begin()).first;
			for (auto it = unexploredNodes.begin(); it != unexploredNodes.end(); it++) {
				if (distanceFromStart[(*it).first] < distanceFromStart[*current]) 
					current = &(*it).first;
			}

			// if path was found, put path into a vector to return
			if (*current == goal) {
				std::vector<PointT> path;
				path.push_back(goal);

				// start from goal, then trace its parents back to current node
				while (path.back() != start)
					path.push_back(*pathParents[path.back()]);

				// reverse path to go from start to goal
				std::reverse(path.begin(), path.end());

				return path;
			}
			else {
				const std::vector<std::pair<Node *, int>> &links = linksOf(*current);

				// for each neighbor of the current node,
				for (auto it = links.begin(); it < links.end(); it++) {
					// update their distanceFromStart if it is less than what it is currently
					PointT neighborPoint = (*it).first->point_;
					int distance = distanceFromStart[*current] + (*it).second;

					if (distance < distanceFromStart[neighborPoint]) {
						distanceFromStart[neighborPoint] = distance;
						pathParents[neighborPoint] = &(*map_.find(*current)).first;
					}
				}
				// remove explored node
				unexploredNodes.erase(*current);
			}
		}
	}

	return std::vector<PointT>();
}
