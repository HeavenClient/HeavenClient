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
#include "Texture.h"

#include "GraphicsGL.h"

namespace ms
{
	Texture::Texture(nl::node src)
	{
		if (src.data_type() == nl::node::type::bitmap)
		{
			origin = src["origin"];

			std::string source = src["source"];
			std::string _outlink = src["_outlink"];
			std::string _inlink = src["_inlink"];

			auto foundSource = find_child(src, source);
			auto foundOutlink = find_child(src, _outlink);

			auto foundChild = foundSource || foundOutlink;

			if (foundChild)
				src = foundSource ? foundSource : foundOutlink;

			if (!foundChild && !_inlink.empty())
			{
				auto parent_node = src.root();

				for (auto child_node = parent_node.begin(); child_node != parent_node.end(); ++child_node)
				{
					auto found_node = child_node.resolve(_inlink);

					if (found_node.data_type() == nl::node::type::bitmap)
					{
						src = found_node;
						break;
					}
				}
			}

			bitmap = src;
			dimensions = Point<int16_t>(bitmap.width(), bitmap.height());

			GraphicsGL::get().addbitmap(bitmap);
		}
	}

	Texture::Texture()
	{}

	Texture::~Texture()
	{}

	void Texture::draw(const DrawArgument &args) const
	{
		size_t id = bitmap.id();

		if (id == 0)
			return;

		GraphicsGL::get().draw(
				bitmap,
				args.get_rectangle(origin, dimensions),
				args.get_color(),
				args.get_angle()
		);
	}

	void Texture::shift(Point<int16_t> amount)
	{
		origin -= amount;
	}

	bool Texture::is_valid() const
	{
		return bitmap.id() > 0;
	}

	int16_t Texture::width() const
	{
		return dimensions.x();
	}

	int16_t Texture::height() const
	{
		return dimensions.y();
	}

	Point<int16_t> Texture::get_origin() const
	{
		return origin;
	}

	Point<int16_t> Texture::get_dimensions() const
	{
		return dimensions;
	}

	nl::node Texture::find_child(nl::node source, std::string link)
	{
		if (!link.empty())
		{
			nl::node parent_node = source.root();

			return parent_node.resolve(link.substr(link.find('/') + 1));
		}

		return nl::node();
	}
}