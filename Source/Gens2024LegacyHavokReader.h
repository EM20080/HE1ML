#pragma once

#include <memory>
#include <string_view>

namespace gens2024::legacy_havok
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

	bool Supports(std::string_view type);
	int BlocksFallback(std::string_view type, const void* data, unsigned long dataSize);
	Buffer Convert(std::string_view type, const void* data, unsigned long dataSize);
}
