#include "Gens24LegacyTerrainReader.h"

#include <LibGens.h>
#include <AR.h>
#include <Terrain.h>
#include <TerrainBlock.h>
#include <TerrainGroup.h>
#include <GITextureGroup.h>
#include <Light.h>
#include <LightField.h>
#include <cstring>

#ifndef LIBGENS_TERRAIN_ROOT_GENERATIONS
#define LIBGENS_TERRAIN_ROOT_GENERATIONS 3
#endif

#ifndef LIBGENS_TERRAIN_BLOCK_ROOT_GENERATIONS
#define LIBGENS_TERRAIN_BLOCK_ROOT_GENERATIONS 0
#endif

#ifndef LIBGENS_TERRAIN_GROUP_ROOT_GENERATIONS
#define LIBGENS_TERRAIN_GROUP_ROOT_GENERATIONS 1
#endif

#ifndef LIBGENS_GI_TEXTURE_GROUP_ROOT_TYPE
#define LIBGENS_GI_TEXTURE_GROUP_ROOT_TYPE 2
#endif

#ifndef LIBGENS_LIGHT_ROOT_GENERATIONS
#define LIBGENS_LIGHT_ROOT_GENERATIONS 1
#endif

#ifndef LIBGENS_LIGHT_LIST_ROOT_GENERATIONS
#define LIBGENS_LIGHT_LIST_ROOT_GENERATIONS 0
#endif

namespace gens2024::legacy_terrain
{
	int GetRootNodeType(const void* data, unsigned long dataSize)
	{
		if (!data || !dataSize)
			return 0;

		LibGens::File file(data, dataSize);
		if (!file.valid())
			return 0;

		file.readHeader();
		return file.getRootNodeType();
	}

	bool Supports(std::string_view type)
	{
		return
			type == "pfi" ||
			type == "terrain" ||
			type == "tbst" ||
			type == "terrain-group" ||
			type == "gi-texture-group-info" ||
			type == "light" ||
			type == "light-list" ||
			type == "lft";
	}

	Buffer MakeBuffer(const unsigned char* data, unsigned long dataSize)
	{
		Buffer buffer;
		buffer.data.reset(new unsigned char[dataSize]);
		buffer.size = dataSize;
		memcpy(buffer.data.get(), data, dataSize);
		return buffer;
	}

	unsigned int ReadBE32(const unsigned char* data)
	{
		return (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
	}

	void WriteBE32(vector<unsigned char>& data, unsigned int value)
	{
		data.push_back((value >> 24) & 0xFF);
		data.push_back((value >> 16) & 0xFF);
		data.push_back((value >> 8) & 0xFF);
		data.push_back(value & 0xFF);
	}

	void SetBE32(vector<unsigned char>& data, size_t offset, unsigned int value)
	{
		data[offset] = (value >> 24) & 0xFF;
		data[offset + 1] = (value >> 16) & 0xFF;
		data[offset + 2] = (value >> 8) & 0xFF;
		data[offset + 3] = value & 0xFF;
	}

	void WriteBE64Address(vector<unsigned char>& data, unsigned int value)
	{
		WriteBE32(data, 0);
		WriteBE32(data, value);
	}

	void SetBE64Address(vector<unsigned char>& data, size_t offset, unsigned int value)
	{
		SetBE32(data, offset, 0);
		SetBE32(data, offset + 4, value);
	}

	void Pad(vector<unsigned char>& data, size_t alignment)
	{
		while (data.size() % alignment)
			data.push_back(0);
	}

	vector<unsigned char> ConvertTerrain(const void* data, unsigned long dataSize)
	{
		LibGens::File in(data, dataSize);
		if (!in.valid())
			return {};

		in.readHeader();

		LibGens::Terrain terrain;
		terrain.read(&in, "", "", false);

		LibGens::File out;
		out.prepareHeader(LIBGENS_TERRAIN_ROOT_GENERATIONS);
		out.set64BitAddressMode(true);
		terrain.writeGenerations2024(&out);
		out.writeHeader();

		return out.detach();
	}

	vector<unsigned char> ConvertTerrainBlock(const void* data, unsigned long dataSize)
	{
		LibGens::File in(data, dataSize);
		if (!in.valid())
			return {};

		in.readHeader();

		LibGens::TerrainBlock block;
		block.read(&in);

		LibGens::File out;
		out.prepareHeader(LIBGENS_TERRAIN_BLOCK_ROOT_GENERATIONS);
		out.set64BitAddressMode(true);
		block.write(&out);
		out.writeHeader();

		return out.detach();
	}

	vector<unsigned char> ConvertTerrainGroup(const void* data, unsigned long dataSize)
	{
		LibGens::File in(data, dataSize);
		if (!in.valid())
			return {};

		in.readHeader();

		LibGens::TerrainGroup group("", "", "");
		group.read(&in, "");

		LibGens::File out;
		out.prepareHeader(LIBGENS_TERRAIN_GROUP_ROOT_GENERATIONS_2024);
		out.set64BitAddressMode(true);
		group.write(&out);
		out.writeHeader();

		return out.detach();
	}

	vector<unsigned char> ConvertGITextureGroupInfo(const void* data, unsigned long dataSize)
	{
		LibGens::File in(data, dataSize);
		if (!in.valid())
			return {};

		in.readHeader();

		LibGens::GITextureGroupInfo info;
		info.read(&in, "");

		LibGens::File out;
		out.prepareHeader(LIBGENS_GI_TEXTURE_GROUP_ROOT_TYPE);
		out.set64BitAddressMode(true);
		info.writeGenerations2024(&out);
		out.writeHeader();

		return out.detach();
	}

	vector<unsigned char> ConvertLight(const void* data, unsigned long dataSize)
	{
		LibGens::File in(data, dataSize);
		if (!in.valid())
			return {};

		in.readHeader();

		LibGens::Light light;
		light.read(&in);

		LibGens::File out;
		out.prepareHeader(LIBGENS_LIGHT_ROOT_GENERATIONS);
		out.set64BitAddressMode(true);
		light.write(&out);
		out.writeHeader(true);

		return out.detach();
	}

	vector<unsigned char> ConvertLightList(const void* data, unsigned long dataSize)
	{
		LibGens::File in(data, dataSize);
		if (!in.valid())
			return {};

		in.readHeader();

		LibGens::LightList lightList;
		lightList.readNames(&in);

		LibGens::File out;
		out.prepareHeader(LIBGENS_LIGHT_LIST_ROOT_GENERATIONS);
		out.set64BitAddressMode(true);
		lightList.write(&out);
		out.writeHeader();

		return out.detach();
	}

	vector<unsigned char> ConvertLightField(const void* data, unsigned long dataSize)
	{
		if (!data || dataSize < 72)
			return {};

		const unsigned char* bytes = (const unsigned char*)data;
		vector<unsigned char> input(bytes, bytes + dataSize);

		if (ReadBE32(&input[4]) != LIBGENS_LIGHTFIELD_FILE_ROOT_TYPE)
			return {};

		unsigned int rootAddress = ReadBE32(&input[12]);
		unsigned int tableAddress = ReadBE32(&input[16]);

		if (rootAddress >= input.size() || tableAddress <= rootAddress || tableAddress > input.size())
			return {};

		unsigned int cubeAddress = ReadBE32(&input[rootAddress + 28]);
		unsigned int colorAddress = ReadBE32(&input[rootAddress + 36]);
		unsigned int indexAddress = ReadBE32(&input[rootAddress + 44]);
		unsigned int rootSize = tableAddress - rootAddress;

		if (rootAddress + rootSize > input.size())
			return {};

		if (cubeAddress > colorAddress || colorAddress > indexAddress || indexAddress > rootSize)
			return {};

		vector<unsigned char> root;

		root.insert(root.end(), input.begin() + rootAddress, input.begin() + rootAddress + 28);
		Pad(root, 8);

		size_t cubeSlot = root.size();
		WriteBE64Address(root, 0);

		root.insert(root.end(), input.begin() + rootAddress + 32, input.begin() + rootAddress + 36);
		Pad(root, 8);

		size_t colorSlot = root.size();
		WriteBE64Address(root, 0);

		root.insert(root.end(), input.begin() + rootAddress + 40, input.begin() + rootAddress + 44);
		Pad(root, 8);

		size_t indexSlot = root.size();
		WriteBE64Address(root, 0);

		unsigned int newCubeAddress = (unsigned int)root.size();
		root.insert(root.end(), input.begin() + rootAddress + cubeAddress, input.begin() + rootAddress + colorAddress);

		unsigned int newColorAddress = (unsigned int)root.size();
		root.insert(root.end(), input.begin() + rootAddress + colorAddress, input.begin() + rootAddress + indexAddress);

		Pad(root, 4);

		unsigned int newIndexAddress = (unsigned int)root.size();
		root.insert(root.end(), input.begin() + rootAddress + indexAddress, input.begin() + rootAddress + rootSize);

		Pad(root, 8);

		SetBE64Address(root, cubeSlot, newCubeAddress);
		SetBE64Address(root, colorSlot, newColorAddress);
		SetBE64Address(root, indexSlot, newIndexAddress);

		vector<unsigned char> out(24, 0);
		out.insert(out.end(), root.begin(), root.end());

		unsigned int newTableAddress = (unsigned int)out.size();

		WriteBE32(out, 3);
		WriteBE32(out, (unsigned int)cubeSlot);
		WriteBE32(out, (unsigned int)colorSlot);
		WriteBE32(out, (unsigned int)indexSlot);

		unsigned int footAddress = (unsigned int)out.size();
		out.insert(out.end(), 16, 0);

		SetBE32(out, 0, (unsigned int)out.size());
		SetBE32(out, 4, LIBGENS_LIGHTFIELD_FILE_ROOT_TYPE);
		SetBE32(out, 8, newTableAddress - 24);
		SetBE32(out, 12, 24);
		SetBE32(out, 16, newTableAddress);
		SetBE32(out, 20, footAddress);

		return out;
	}

	vector<unsigned char> ConvertPFI(const void* data, unsigned long dataSize)
	{
		LibGens::File in(data, dataSize);
		if (!in.valid())
			return {};

		in.readHeader();

		unsigned int count = 0;
		size_t tableAddress = 0;

		in.readInt32BE(&count);
		in.readInt32BEA(&tableAddress);

		struct Entry
		{
			string name;
			unsigned int address;
			unsigned int size;
		};

		vector<Entry> entries;

		for (size_t i = 0; i < count; i++)
		{
			size_t entryAddress = 0;
			size_t nameAddress = 0;
			Entry entry;

			in.goToAddress(tableAddress + i * in.getAddressSize());
			in.readInt32BEA(&entryAddress);

			in.goToAddress(entryAddress);
			in.readInt32BEA(&nameAddress);
			in.readInt32BE(&entry.address);
			in.readInt32BE(&entry.size);

			in.goToAddress(nameAddress);
			in.readString(&entry.name);

			entries.push_back(entry);
		}

		LibGens::File out;
		out.prepareHeader(0);
		out.set64BitAddressMode(true);

		unsigned int outCount = (unsigned int)entries.size();
		size_t outTableAddress = 40;

		out.writeInt32BE(&outCount);
		out.writeInt32BEA(&outTableAddress);
		out.writeNull(entries.size() * out.getAddressSize());

		vector<size_t> entryAddresses;

		for (size_t i = 0; i < entries.size(); i++)
		{
			out.fixPadding(8);

			size_t entryAddress = out.getCurrentAddress();
			entryAddresses.push_back(entryAddress);

			size_t nameSlot = out.writeNullAddress();

			out.writeInt32BE(&entries[i].address);
			out.writeInt32BE(&entries[i].size);

			size_t nameAddress = out.getCurrentAddress();
			out.writeString(&entries[i].name);
			out.fixPadding();

			out.goToAddress(nameSlot);
			out.writeInt32BEA(&nameAddress);
			out.goToEnd();
		}

		for (size_t i = 0; i < entryAddresses.size(); i++)
		{
			out.goToAddress(outTableAddress + i * out.getAddressSize());
			out.writeInt32BEA(&entryAddresses[i]);
		}

		out.goToEnd();
		out.writeHeader();

		return out.detach();
	}

	Buffer Convert(std::string_view type, std::string_view name, const void* data, unsigned long dataSize)
	{
		if (!Supports(type) || !data || !dataSize)
			return {};

		LibGens::File probe(data, dataSize);
		if (!probe.valid())
			return {};

		probe.readHeader();

		if (probe.get64BitAddressMode())
			return {};

		if (probe.getRootNodeType() == 0 &&
			type != "pfi" &&
			type != "tbst" &&
			type != "lft" &&
			type != "light-list")
			return {};

		vector<unsigned char> output;

		if (type == "terrain")
			output = ConvertTerrain(data, dataSize);
		else if (type == "tbst")
			output = ConvertTerrainBlock(data, dataSize);
		else if (type == "terrain-group")
			output = ConvertTerrainGroup(data, dataSize);
		else if (type == "gi-texture-group-info")
			output = ConvertGITextureGroupInfo(data, dataSize);
		else if (type == "light")
			output = ConvertLight(data, dataSize);
		else if (type == "light-list")
			output = ConvertLightList(data, dataSize);
		else if (type == "lft")
			output = ConvertLightField(data, dataSize);
		else if (type == "pfi")
			output = ConvertPFI(data, dataSize);

		if (!output.empty())
			return MakeBuffer(output.data(), (unsigned long)output.size());

		return {};
	}
}
