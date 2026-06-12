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

#define LIBGENS_POINT_CLOUD_ERROR_MESSAGE_NULL_FILE        "Trying to read point cloud data from unreferenced file."
#define LIBGENS_POINT_CLOUD_ERROR_MESSAGE_WRITE_NULL_FILE  "Trying to write point cloud data to an unreferenced file."
#define LIBGENS_POINT_CLOUD_ERROR_MESSAGE_INVALID_FILE     "Point cloud file is invalid."

#define LIBGENS_POINT_CLOUD_FORMAT_VERSION_1               1
#define LIBGENS_POINT_CLOUD_FORMAT_VERSION_2               2

#define LIBGENS_POINT_CLOUD_EXTENSION_MODEL                ".pcmodel"
#define LIBGENS_POINT_CLOUD_EXTENSION_COLLISION            ".pccol"
#define LIBGENS_POINT_CLOUD_EXTENSION_RAYTRACING           ".pcrt"

namespace LibGens {
	class PointCloudInstance {
		protected:
			string name;
			string resource_name;
			Vector3 position;
			Vector3 rotation;
			Vector3 scale;
			int field_28;

		public:
			PointCloudInstance();

			void setName(string v);
			void setResourceName(string v);
			void setPosition(Vector3 v);
			void setRotation(Vector3 v);
			void setScale(Vector3 v);
			void setField28(int v);

			string getName();
			string getResourceName();
			Vector3 getPosition();
			Vector3 getRotation();
			Vector3 getScale();
			int getField28();
	};

	class PointCloud {
		protected:
			string name;
			unsigned int format_version;
			vector<PointCloudInstance *> instances;

		public:
			PointCloud();
			PointCloud(string filename);
			~PointCloud();

			void read(File *file);
			void save(string filename);
			void write(File *file);

			void addInstance(PointCloudInstance *instance);
			vector<PointCloudInstance *> getInstances();

			void setName(string v);
			string getName();
			void setFormatVersion(unsigned int v);
			unsigned int getFormatVersion();
	};
};
