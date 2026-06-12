//=========================================================================
//	  Copyright (c) 2016 SonicGLvl
//
//    This file is part of SonicGLvl, a community-created free level editor 
//    for the PC version of Sonic Generations.
//
//    SonicGLvl is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    SonicGLvl is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//    
//
//    Read AUTHORS.txt, LICENSE.txt and COPYRIGHT.txt for more details.
//=========================================================================

#pragma once

#define LIBGENS_TERRAIN_ERROR_MESSAGE_NULL_FILE        "Trying to read terrain data from unreferenced file."
#define LIBGENS_TERRAIN_ERROR_MESSAGE_WRITE_NULL_FILE  "Trying to write terrain data to an unreferenced file."
#define LIBGENS_TERRAIN_EXTENSION                      ".terrain"
#define LIBGENS_TERRAIN_GROUP_EXTENSION                ".terrain-group"
#define LIBGENS_TERRAIN_GROUP_FOLDER_EXTENSION         ".ar"
#define LIBGENS_TERRAIN_AUTODRAW_TXT                   "Autodraw.txt"

#define LIBGENS_TERRAIN_ROOT_GENERATIONS              3
#define LIBGENS_TERRAIN_ROOT_GENERATIONS_2024         103

namespace LibGens {
	class UVAnimation;
	class TerrainGroup;
	class TerrainGroupInfo;
	class Model;
	class MaterialLibrary;
	class TerrainInstance;

	class TerrainAutodraw {
		protected:
			string folder;
			vector<string> elements;

			list<UVAnimation *> uv_animations;
		public:
			TerrainAutodraw(string filename);

			void read(File *file);
			bool hasModel(const string& model_name) const;
			string findSkeletonForModel(const string& model_name) const;
			string findAnimationForModel(const string& model_name) const;
			const vector<string>& getElements() const {
				return elements;
			}
			const string& getFolder() const {
				return folder;
			}
	};

	class Terrain {
		protected:
			vector<TerrainGroup *> groups;
			vector<TerrainGroupInfo *> groups_info;
			MaterialLibrary *material_library;
			string gia_folder;
			string resources_folder;
			string stage_folder;

			list<Model *>           models_to_organize;
			list<TerrainInstance *> instances_to_organize;
		public:
			Terrain() {
				material_library = NULL;
			}

			Terrain(string filename, string groups_folder, string resources_folder_p, string terrain_folder="", string gia="", bool load_groups=true);
			void save(string filename, int root_type = LIBGENS_TERRAIN_ROOT_GENERATIONS);

			void read(File *file, string groups_folder, string terrain_folder, bool load_groups);
			void write(File *file);
			void writeGenerations2024(File *file);

			list<TerrainInstance *> getInstances();

			list<Model *> getModelsToOrganize() {
				return models_to_organize;
			}

			list<TerrainInstance *> getInstancesToOrganize() {
				return instances_to_organize;
			}

			void generateGroups(unsigned int cell_size);
			vector<TerrainGroup *> getGroups();

			void addModel(Model *v) {
				models_to_organize.push_back(v);
			}

			void addInstance(TerrainInstance *v) {
				instances_to_organize.push_back(v);
			}

			void addModels(list<Model *> v) {
				models_to_organize.merge(v);
			}

			void addInstances(list<TerrainInstance *> v) {
				instances_to_organize.merge(v);
			}

			void addGroupInfo(TerrainGroupInfo *group_info);
			TerrainGroup *getGroupForInstance(TerrainInstance *instance);

			MaterialLibrary *getMaterialLibrary() {
				return material_library;
			}

			string getStageFolder() {
				return stage_folder;
			}

			string getResourcesFolder() {
				return resources_folder;
			}

			string getGIFolder() {
				return gia_folder;
			}

			void clean();
	};
};
