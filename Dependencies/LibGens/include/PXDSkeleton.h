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

#define LIBGENS_PXD_SKELETON_MAGIC            "KSXP"
#define LIBGENS_PXD_SKELETON_VERSION          512
#define LIBGENS_PXD_SKELETON_HEADER_SIZE      104
#define LIBGENS_PXD_SKELETON_DATA_START       0x40

#define LIBGENS_PXD_SKELETON_ERROR_FILE       "Couldn't read PXD skeleton file."
#define LIBGENS_PXD_SKELETON_ERROR_MAGIC      "Not a valid PXD skeleton file."
#define LIBGENS_PXD_SKELETON_ERROR_VERSION    "Wrong PXD skeleton version."

namespace LibGens {
	struct PXDBone {
		string name;
		short parent_index;
		Vector3 position;
		Quaternion rotation;
		Vector3 scale;
	};

	class PXDSkeleton {
		protected:
			vector<PXDBone> bones;
			string name;
		public:
			PXDSkeleton();
			PXDSkeleton(string filename);
			~PXDSkeleton();
			void read(File *file);
			vector<PXDBone> &getBones();
			unsigned int getBoneCount();
			string getName();
			void setName(string v);
			short getBoneParentIndex(unsigned int index);
			string getBoneName(unsigned int index);
			Vector3 getBonePosition(unsigned int index);
			Quaternion getBoneRotation(unsigned int index);
			Vector3 getBoneScale(unsigned int index);
	};
};
