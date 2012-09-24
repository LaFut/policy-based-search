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
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

using boost::numeric::ublas::matrix;

// typedef unsigned short index;
typedef unsigned short index;
typedef char City;
typedef char Edge;


inline index city_index(City const &C)
{
	if(C < 'A' || C > 'Z')
		throw std::runtime_error("City out of range.");
	return C - 'A';
}


inline index edge_index(Edge const &E)
{
	if(E < 'a' || E > 'z')
		throw std::runtime_error("Edge out of range.");
	return E - 'a';
}


class TSP;
// Problem definition
class TSP
{
public:
	typedef std::vector<index> state;
	typedef index action;
	typedef unsigned int pathcost;
	typedef jsearch::DefaultNode<TSP> node;
};

static size_t const N(4);

template <typename PathCost>
struct EdgeData
{
	PathCost cost;
	std::pair<City, City> city;
};


static matrix<index> minmal_problem()
{
	matrix<index> matrix(N, N);

	matrix(1, 2) = matrix(2, 1) = 1;
	matrix(1, 3) = matrix(3, 1) = 2;
	matrix(1, 4) = matrix(4, 1) = 4;
	matrix(2, 3) = matrix(3, 2) = 7;
	matrix(2, 4) = matrix(4, 2) = 11;
	matrix(3, 4) = matrix(4, 3) = 16;

	return matrix;
}


template <typename T>
std::vector<EdgeData<T>> transpose(matrix<T> const &P)
{
	if(P.size1() != P.size2())
		throw std::runtime_error("Matrix not square.");
	std::vector<EdgeData<T>> result(P.size1());

	for(unsigned i = 0; i < P.size1() - 1; ++i)
	{
		for(unsigned j = i + 1; j < P.size1(); ++j)
			result.push_back({P(i, j), {i, j}});
	}
	
	return result;
}


static boost::numeric::ublas::matrix<index> const PROBLEM(minmal_problem());
auto const foo = transpose(PROBLEM);

typedef std::vector<TSP::pathcost> cost;
std::vector<TSP::action> const EDGES = {0, 1, 2, 3, 4, 5};
cost const COST = {1, 2, 4, 7, 11, 16};	// Edges are implicitly numbered 0 to n-1.


// TSP heuristic: shortest imaginable tour including these edges.
template <typename PathCost, class State>
class MinimalFeasibleTour
{
protected:
	PathCost h(State const &STATE) const
	{
		auto const start = std::begin(COST) + STATE.back();
		return std::accumulate(start, start + N - STATE.size(), 0);
	}
};



template <typename PathCost, typename State, typename Action>
class EdgeCost
{
protected:
	PathCost step_cost(State const &, Action const &ACTION) const
	{
		return COST[ACTION];
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


// GoalTestPolicy
template <typename State>
class ValidTour
{
protected:
	bool goal_test(State const &STATE) const
	{
		// Is it a Hamiltonian cycle?
		// I think it is enough to have n edges that do not break any contraints.
		// If ActionsPolicy does its job, this will be called only once.
		return STATE.size() == N;
	}
};


#endif // TSP_H
