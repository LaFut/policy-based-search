/*
    TSP.h: Travelling Salesman Problem cast as an implicit graph search.
    Copyright (C) 2012  Jeremy Murphy <jeremy.william.murphy@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TSP_H
#define TSP_H

#include "problem.h"

#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <array>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/edge_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/depth_first_search.hpp>

using boost::adjacency_matrix;


/*
inline Index city_Index(City const &C)
{
	if(C < 'A' || C > 'Z')
		throw std::runtime_error("City out of range.");
	return C - 'A';
}


inline Index edge_Index(Edge const &E)
{
	if(E < 'a' || E > 'z')
		throw std::runtime_error("Edge out of range.");
	return E - 'a';
}


inline City city(Index const &I)
{
	return I + 'A';
}


inline Edge edge(Index const &I)
{
	return I + 'a';
}
*/



//Bundled properties.
struct VertexProps
{
	VertexProps() {}
	VertexProps(std::string const &NAME) : name(NAME) {}
	
	std::string name;
};


struct EdgeProps
{
	EdgeProps() {}
	EdgeProps(char const &NAME, unsigned int const &W) : name(NAME), cost(W) {}

	char name;
	unsigned int cost;

	bool operator<(EdgeProps const &OTHER) const
	{
		return cost < OTHER.cost;
	}
};


typedef adjacency_matrix<boost::undirectedS, VertexProps, EdgeProps> Graph;

// typedef std::pair<int, int> E;

typedef typename boost::graph_traits<Graph>::vertex_iterator vertex_iter;
typedef typename boost::graph_traits<Graph>::edge_iterator edge_iter;
typedef typename boost::graph_traits<Graph>::vertices_size_type vertices_size_type;
typedef typename boost::graph_traits<Graph>::edges_size_type edges_size_type;
typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_desc;
typedef typename boost::graph_traits<Graph>::edge_descriptor edge_desc;

vertices_size_type n; // Size of the TSP instance (number of cities).
edges_size_type N; // Size of the TSP instance (number of edges).


typedef unsigned short Index;
typedef char City;
typedef char Edge;

class TSP;
// Problem definition
class TSP
{
public:
	typedef std::vector<Index> state;
	typedef Index action;
	typedef unsigned int pathcost;
	typedef jsearch::DefaultNode<TSP> node;
	static bool const combinatorial = true;
};

Graph const *g = nullptr;
// Could these two be combined into a std::map<TSP::action, EdgeProps> without sacrificing complexity?
std::vector<TSP::action> EDGES;
std::vector<EdgeProps> COST;
////////////////////////////////////////////////////////////////////////


// NOTE: Could this data somehow be stored on and accessed from Problem?
// std::vector<EdgeData<TSP::pathcost>> COST;
// std::vector<TSP::action> EDGES;
// size_t n; // I'm using a convention that n == number of cities, and N == number of edges.
// So n is the size of the TSP in cities, but N is the size of the TSP in edges.
// Curiously, I just realized that the real difficulty of a TSP is the difference between n and N: neither by themselves tells you enough.

// TSP heuristic: shortest imaginable tour including these edges.
template <typename PathCost, class State>
class MinimalImaginableTour
{
protected:
	PathCost h(State const &STATE) const
	{
		// Expects edge costs to be ordered.
		auto const START = std::begin(COST) + (STATE.empty() ? 0 : STATE.back() + 1); // NOTE: Test this.
		PathCost const result = std::accumulate(START, START + n - STATE.size(), 0, [](PathCost A, EdgeProps const &B)
		{
			return A + B.cost;
		});
		
		return result;
	}
};


template <typename PathCost, typename State, typename Action>
class EdgeCost
{
protected:
	PathCost step_cost(State const &, Action const &ACTION) const
	{
		return COST[ACTION].cost;
	}
};


// This actually has to do the bulk of the work in the algorithm.
template <typename State, typename Action>
class HigherCostValidEdges
{
protected:
	std::set<Action> actions(State const &STATE) const
	{
		std::set<Action> const candidates(std::begin(EDGES) + (STATE.empty() ? 0 : STATE.back() + 1), std::end(EDGES));
		// TODO: Now check them!
		if(STATE.size() > 1)
		{
			std::map<Index, unsigned short> checker;
		}

		return candidates;
	}
};


// Trivial function that adds action to the set of edges.
template <typename State, typename Action>
class AppendEdge	// ho ho ho
{
protected:
	State result(State const &PARENT, Action const &ACTION) const
	{
		State child(PARENT);
		child.push_back(ACTION);
		return child;
	}
};


// GoalTestPolicy: Is it a Hamiltonian cycle?
// I think it is enough to have n edges that do not break any contraints.
template <typename State>
class ValidTour
{
protected:
	bool goal_test(State const &STATE) const
	{
		return STATE.size() == n;
	}
};

#endif // TSP_H
