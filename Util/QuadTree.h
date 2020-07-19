//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <functional>
#include <vector>
#include <unordered_map>

namespace ms
{
	template <typename K, typename V>
	class QuadTree
	{
	public:
		enum Direction
		{
			LEFT, RIGHT, UP, DOWN
		};

		QuadTree(std::function<Direction(const V&, const V&)> c)
		{
			root = 0;
			comparator = c;
		}

		QuadTree() : QuadTree(nullptr)
		{}

		void clear()
		{
			nodes.clear();

			root = 0;
		}

		void add(K key, V value)
		{
			K parent = 0;

			if (root)
			{
				K current = root;

				while (current)
				{
					parent = current;
					current = nodes[parent].addornext(key, value, comparator);
				}
			}
			else
			{
				root = key;
			}

			nodes.emplace(
				std::piecewise_construct,
				std::forward_as_tuple(key),
				std::forward_as_tuple(value, parent, 0, 0, 0, 0)
			);
		}

		void erase(K key)
		{
			if (!nodes.count(key))
				return;

			Node& toerase = nodes[key];

			std::vector<K> leaves;

			for (size_t i = LEFT; i <= DOWN; i++)
			{
				K leafkey = toerase[i];

				if (leafkey)
					leaves.push_back(leafkey);
			}

			K parent = toerase.parent;

			if (root == key)
				root = 0;
			else if (nodes.count(parent))
				nodes[parent].erase(key);

			nodes.erase(key);

			for (auto& leaf : leaves)
				readd(parent, leaf);
		}

		K findnode(const V& value, std::function<bool(const V&, const V&)> predicate)
		{
			if (root)
			{
				K key = findfrom(root, value, predicate);

				return predicate(value, nodes[key].value) ? key : 0;
			}
			else
			{
				return 0;
			}
		}

		V& operator[](K key)
		{
			return nodes[key].value;
		}

		const V& operator[](K key) const
		{
			return nodes.at(key).value;
		}

	private:
		K findfrom(K start, const V& value, std::function<bool(const V&, const V&)> predicate)
		{
			if (!start)
				return 0;

			bool fulfilled = predicate(value, nodes[start].value);
			Direction dir = comparator(value, nodes[start].value);

			if (dir == RIGHT)
			{
				K right = findfrom(nodes[start].right, value, predicate);

				if (right && predicate(value, nodes[right].value))
					return right;
				else
					return start;
			}
			else if (dir == DOWN)
			{
				K bottom = findfrom(nodes[start].bottom, value, predicate);

				if (bottom && predicate(value, nodes[bottom].value))
				{
					return bottom;
				}
				else if (fulfilled)
				{
					return start;
				}
				else
				{
					K right = findfrom(nodes[start].right, value, predicate);

					if (right && predicate(value, nodes[right].value))
						return right;
					else
						return start;
				}
			}
			else if (dir == UP)
			{
				K top = findfrom(nodes[start].top, value, predicate);

				if (top && predicate(value, nodes[top].value))
				{
					return top;
				}
				else if (fulfilled)
				{
					return start;
				}
				else
				{
					K right = findfrom(nodes[start].right, value, predicate);

					if (right && predicate(value, nodes[right].value))
						return right;
					else
						return start;
				}
			}
			else
			{
				K left = findfrom(nodes[start].left, value, predicate);

				if (left && predicate(value, nodes[left].value))
					return left;
				else if (fulfilled)
					return start;

				K bottom = findfrom(nodes[start].bottom, value, predicate);

				if (bottom && predicate(value, nodes[bottom].value))
					return bottom;
				else if (fulfilled)
					return start;

				K top = findfrom(nodes[start].top, value, predicate);

				if (top && predicate(value, nodes[top].value))
					return top;
				else if (fulfilled)
					return start;

				K right = findfrom(nodes[start].right, value, predicate);

				if (right && predicate(value, nodes[right].value))
					return right;
				else
					return start;
			}
		}

		void readd(K start, K key)
		{
			if (start)
			{
				K parent = 0;
				K current = start;

				while (current)
				{
					parent = current;
					current = nodes[parent].addornext(key, nodes[key].value, comparator);
				}

				nodes[key].parent = parent;
			}
			else if (start == root)
			{
				root = key;

				nodes[key].parent = 0;
			}
			else if (root)
			{
				readd(root, key);
			}
		}

		struct Node
		{
			V value;
			K parent;
			K left;
			K right;
			K top;
			K bottom;

			Node(const V& v, K p, K l, K r, K t, K b) : value(v), parent(p), left(l), right(r), top(t), bottom(b)
			{}

			Node() : Node(V(), 0, 0, 0, 0, 0)
			{}

			void erase(K key)
			{
				if (left == key)
					left = 0;
				else if (right == key)
					right = 0;
				else if (top == key)
					top = 0;
				else if (bottom == key)
					bottom = 0;
			}

			K addornext(K key, V val, std::function<Direction(const V&, const V&)> comparator)
			{
				Direction dir = comparator(val, value);
				K dirkey = leaf(dir);

				if (!dirkey)
				{
					switch (dir)
					{
						case LEFT:
							left = key;
							break;
						case RIGHT:
							right = key;
							break;
						case UP:
							top = key;
							break;
						case DOWN:
							bottom = key;
							break;
					}
				}

				return dirkey;
			}

			K leaf(Direction dir)
			{
				switch (dir)
				{
					case LEFT:
						return left;
					case RIGHT:
						return right;
					case UP:
						return top;
					case DOWN:
						return bottom;
					default:
						return 0;
				}
			}

			K operator[](size_t d)
			{
				auto dir = static_cast<Direction>(d);

				return leaf(dir);
			}
		};

		std::function<Direction(const V&, const V&)> comparator;
		std::unordered_map<K, Node> nodes;
		K root;
	};
}