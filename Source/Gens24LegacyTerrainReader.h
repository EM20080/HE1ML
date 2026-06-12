#pragma once

#include <memory>
#include <string_view>

namespace gens2024::legacy_terrain
{
	struct Buffer
	{
		std::unique_ptr<unsigned char[]> data;
		unsigned long size{};

		int empty() const
		{
			return !data || !size;
		}
	};

	int GetRootNodeType(const void* data, unsigned long dataSize);
	bool Supports(std::string_view type);
	Buffer Convert(std::string_view type, std::string_view name, const void* data, unsigned long dataSize);
}
