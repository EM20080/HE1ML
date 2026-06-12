#include "Gens2024LegacyModelReader.h"

#include <LibGens.h>
#include <Model.h>
#include <Material.h>
#include <TerrainInstance.h>
#include <Vertex.h>
#include <cstring>

#ifndef LIBGENS_MODEL_ROOT_DYNAMIC_GENERATIONS_2024
#define LIBGENS_MODEL_ROOT_DYNAMIC_GENERATIONS_2024 54
#endif

#ifndef LIBGENS_MATERIAL_ROOT_GENERATIONS_2024
#define LIBGENS_MATERIAL_ROOT_GENERATIONS_2024 4
#endif

#ifndef LIBGENS_VERTEX_FORMAT_GENERATIONS_2024
#define LIBGENS_VERTEX_FORMAT_GENERATIONS_2024 7
#endif

#ifndef LIBGENS_VERTEX_FORMAT_GENERATIONS_2024_TERRAIN
#define LIBGENS_VERTEX_FORMAT_GENERATIONS_2024_TERRAIN 8
#endif

namespace gens2024::legacy_model
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
			type == "model" ||
			type == "terrain-model" ||
			type == "terrain-instanceinfo" ||
			type == "material";
	}

	Buffer MakeBuffer(const unsigned char* data, unsigned long dataSize)
	{
		Buffer buffer;
		buffer.data.reset(new unsigned char[dataSize]);
		buffer.size = dataSize;
		std::memcpy(buffer.data.get(), data, dataSize);
		return buffer;
	}

	Buffer ConvertModel(std::string_view type, const void* data, unsigned long dataSize)
	{
		LibGens::File in(data, dataSize);
		if (!in.valid())
			return {};

		const auto terrainModel = type == "terrain-model";
		LibGens::Model model(&in, terrainModel);
		model.setTerrainMode(terrainModel);

		LibGens::File out;
		out.prepareHeader(LIBGENS_MODEL_ROOT_DYNAMIC_GENERATIONS);
		out.set64BitAddressMode(true);
		model.changeVertexFormat(terrainModel ? LIBGENS_VERTEX_FORMAT_GENERATIONS_2024_TERRAIN : LIBGENS_VERTEX_FORMAT_GENERATIONS_2024);
		model.write(&out);
		out.writeHeader(true);

		auto output = out.detach();
		return MakeBuffer(output.data(), (unsigned long)output.size());
	}

	Buffer ConvertMaterial(const void* data, unsigned long dataSize)
	{
		LibGens::File in(data, dataSize);
		if (!in.valid())
			return {};

		in.readHeader();

		LibGens::Material material;
		material.read(&in);

		LibGens::File out;
		out.prepareHeader(LIBGENS_MATERIAL_ROOT_GENERATIONS);
		out.set64BitAddressMode(true);
		material.write(&out);
		out.writeHeader(true);

		auto output = out.detach();
		return MakeBuffer(output.data(), (unsigned long)output.size());
	}

	Buffer ConvertTerrainInstanceInfo(const void* data, unsigned long dataSize)
	{
		LibGens::File in(data, dataSize);
		if (!in.valid())
			return {};

		LibGens::TerrainInstance instance(&in, nullptr);

		LibGens::File out;
		out.prepareHeader(LIBGENS_MODEL_ROOT_DYNAMIC_GENERATIONS);
		out.set64BitAddressMode(true);
		instance.write(&out);
		out.writeHeader();

		auto output = out.detach();
		return MakeBuffer(output.data(), (unsigned long)output.size());
	}

	Buffer Convert(std::string_view type, const void* data, unsigned long dataSize)
	{
		if (!Supports(type) || !data || !dataSize)
			return {};

		LibGens::File probe(data, dataSize);
		if (!probe.valid())
			return {};

		probe.readHeader();
		if (probe.getRootNodeType() == 0 || probe.get64BitAddressMode())
			return {};

		if (type == "model" || type == "terrain-model")
			return ConvertModel(type, data, dataSize);

		if (type == "terrain-instanceinfo")
			return ConvertTerrainInstanceInfo(data, dataSize);

		if (type == "material")
			return ConvertMaterial(data, dataSize);

		return {};
	}
}
