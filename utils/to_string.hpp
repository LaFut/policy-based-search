/*
    Copyright (C) 2012  Jeremy W. Murphy <jeremy.william.murphy@gmail.com>

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

#ifndef TO_STRING
#define TO_STRING

#include <algorithm>
#include <string>

namespace jwm
{
	using std::to_string;
	
	// Universal container element stringifier...?
	template <template <typename T> class Container, typename T>
	std::string to_string_element(Container<T> const &V)
	{
		typedef typename Container<T>::const_reference const_reference;
		
		std::string s;
		
		if(!V.empty())
		{
			std::for_each(std::begin(V), std::end(V) - 1, [&](const_reference E)
			{
				s += to_string(E) + ", ";
			});
			
			s += to_string(V.back());
		}
		
		return s;
	}
	
	
	// Sequence containers.
	template <template <typename T, typename Alloc = std::allocator<T>> class Sequence, typename T>
	std::string to_string(Sequence<T> const &SEQUENCE)
	{
		typedef typename Sequence<T>::const_reference const_reference;
		std::string s("⟨");
		
		if(!SEQUENCE.empty())
		{
			std::for_each(std::begin(SEQUENCE), std::end(SEQUENCE) - 1, [&](const_reference E)
			{
				s += to_string(E) + ", ";
			});
			
			s += to_string(SEQUENCE.back());
		}
		
		s += "⟩";
		
		return s;
	}
	

	// Associative containers.
	template <template <typename Key, typename Compare = std::less<Key>, typename Alloc = std::allocator<Key>> class Set, typename Key>
	std::string to_string(Set<Key> const &SET)
	{
		typedef typename Set<Key>::const_reference const_reference;
		std::string s;
		
		if(!SET.empty())
		{
			auto begin(std::begin(SET));
			s += "{" + to_string(*begin++);
			std::for_each(begin, std::end(SET), [&](const_reference E){ s += ", " + to_string(E); });
			s += "}";
		}
		else
			s = "∅";
		
		return s;
	}


	/* If the element of a container IS a string, then of course we just return it.	 */
	inline std::string to_string(std::string const &S)
	{
		return S;
	}
}

#endif
