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

#include <array>

namespace ms
{
	template <typename K, typename V, K LENGTH = K::LENGTH>
	// Wraps an array so that it is addressable by enumeration values
	class EnumMap
	{
	public:
		template <typename...Args>
		// Initialize with an initializer list
		EnumMap(Args&& ... args) : m_values{{std::forward<Args>(args)...}}
		{
			static_assert(std::is_enum<K>::value, "Template parameter 'K' for EnumMap must be an enum.");

			for (size_t i = 0; i < LENGTH; i++)
				m_keys[i] = static_cast<K>(i);
		}

		void clear()
		{
			for (size_t i = 0; i < LENGTH; i++)
				m_values[i] = V();
		}

		void erase(K key)
		{
			if (key >= 0 && key < LENGTH)
				m_values[key] = V();
		}

		template <typename...Args>
		void emplace(K key, Args&& ...args)
		{
			m_values[key] = {std::forward<Args>(args)...};
		}

		V& operator[](K key)
		{
			return m_values[key];
		}

		const V& operator[](K key) const
		{
			return m_values[key];
		}

		template <typename T>
		class base_iterator : public std::iterator<std::forward_iterator_tag, V>
		{

		public:
			using index_type = typename std::underlying_type<K>::type;

			base_iterator(T* p, index_type i) : value(p), index(i)
			{}

			struct node
			{
				K first;
				T& second;

				node(K f, T& s) : first(f), second(s)
				{}

				node& operator=(const node&) = delete;

				void set(const T& t)
				{
					second = t;
				}
			};

			node operator*()
			{
				return node{first(), second()};
			}

			explicit operator bool() const
			{
				return index >= 0 && index < LENGTH;
			}

			K first() const
			{
				return static_cast<K>(index);
			}

			T& second()
			{
				if (!this)
					throw std::out_of_range("iterator out of range");
				else
					return *(value + index);
			}

			base_iterator& operator++()
			{
				index++;
				return *this;
			}

			bool operator!=(const base_iterator& other) const
			{
				return index != other.index;
			}

			bool operator==(const base_iterator& other) const
			{
				return index == other.index;
			}

		private:
			T* value;
			index_type index;
		};

		using iterator = base_iterator<V>;
		using const_iterator = base_iterator<const V>;
		using node = typename iterator::node;
		using cnode = typename const_iterator::node;

		iterator find(K key)
		{
			return {m_values.data(), key};
		}

		const_iterator find(K key) const
		{
			return {m_values.data(), key};
		}

		iterator begin()
		{
			return {m_values.data(), 0};
		}

		iterator end()
		{
			return {m_values.data(), LENGTH};
		}

		const_iterator begin() const
		{
			return {m_values.data(), 0};
		}

		const_iterator end() const
		{
			return {m_values.data(), LENGTH};
		}

		const std::array<K, LENGTH>& keys() const
		{
			return m_keys;
		}

		std::array<V, LENGTH>& values()
		{
			return m_values;
		}

		const std::array<V, LENGTH>& values() const
		{
			return m_values;
		}

	private:
		std::array<K, LENGTH> m_keys;
		std::array<V, LENGTH> m_values;
	};
}