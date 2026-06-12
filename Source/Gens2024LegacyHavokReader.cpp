#include "Gens2024LegacyHavokReader.h"

#include <hklib/hk_packfile.hpp>
#include <spike/io/binreader_stream.hpp>

#include <cstring>
#include <sstream>
#include <string>

namespace gens2024::legacy_havok
{
	namespace
	{
		int IsTagfile(const void* data, unsigned long dataSize)
		{
			if (!data || dataSize < 8)
				return 0;

			const auto bytes = (const unsigned char*)data;
			return bytes[4] == 'T' && bytes[5] == 'A' && bytes[6] == 'G' && bytes[7] == '0';
		}

		int IsLegacyBinaryTagfile(const void* data, unsigned long dataSize)
		{
			if (!data || dataSize < 8)
				return 0;

			const auto bytes = (const unsigned char*)data;
			return
				bytes[0] == 0x1E && bytes[1] == 0x0D && bytes[2] == 0xB0 && bytes[3] == 0xCA &&
				bytes[4] == 0xCE && bytes[5] == 0xFA && bytes[6] == 0x11 && bytes[7] == 0xD0;
		}

		Buffer MakeBuffer(const unsigned char* data, unsigned long dataSize)
		{
			Buffer buffer;
			buffer.data.reset(new unsigned char[dataSize]);
			buffer.size = dataSize;
			std::memcpy(buffer.data.get(), data, dataSize);
			return buffer;
		}

		Buffer ConvertHavok(const void* data, unsigned long dataSize)
		{
			std::string input(
				(const char*)data,
				dataSize
			);

			std::istringstream stream(input);
			BinReaderRef_e reader(stream);
			auto pack = IhkPackFile::Create(reader);
			auto output = pack->ToPackFile(HK2017, 0x8101);

			if (output.empty())
				return {};

			return MakeBuffer(output.data(), (unsigned long)output.size());
		}
	}

	bool Supports(std::string_view type)
	{
		return
			type == "skl.hkx" ||
			type == "anm.hkx" ||
			type == "phy.hkx" ||
			type == "rag.hkx" ||
			type == "skl.hkx.xml" ||
			type == "anm.hkx.xml" ||
			type == "phy.hkx.xml" ||
			type == "rag.hkx.xml";
	}

	int BlocksFallback(std::string_view type, const void* data, unsigned long dataSize)
	{
		return Supports(type) && IsLegacyBinaryTagfile(data, dataSize);
	}

	Buffer Convert(std::string_view type, const void* data, unsigned long dataSize)
	{
		if (!Supports(type) || !data || !dataSize)
			return {};

		if (IsTagfile(data, dataSize))
			return {};

		try
		{
			return ConvertHavok(data, dataSize);
		}
		catch (...)
		{
			return {};
		}
	}
}
