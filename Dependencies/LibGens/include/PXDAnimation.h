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

#define LIBGENS_PXD_ANIMATION_MAGIC            "NAXP"
#define LIBGENS_PXD_ANIMATION_VERSION          512
#define LIBGENS_PXD_ANIMATION_DATA_START       0x40

#define LIBGENS_PXD_ANIMATION_ERROR_FILE       "Couldn't read PXD animation file."
#define LIBGENS_PXD_ANIMATION_ERROR_MAGIC      "Not a valid PXD animation file."
#define LIBGENS_PXD_ANIMATION_ERROR_VERSION    "Unsupported PXD animation version."

namespace LibGens {
	struct PXDTransform {
		Quaternion rotation;
		Vector3 position;
		float bone_length;
		Vector3 scale;
		float scale_w;
	};

	struct PXDTrackKeyframes {
		vector<unsigned short> loc_frames;
		vector<Vector3> loc_values;
		vector<unsigned short> rot_frames;
		vector<Quaternion> rot_values;
		vector<unsigned short> scale_frames;
		vector<Vector3> scale_values;
	};

	class PXDAnimation {
		protected:
			string name;
			bool is_additive;
			bool is_compressed;
			float duration;
			unsigned int frame_count;
			unsigned int track_count;
			float frame_rate;
			vector<vector<PXDTransform>> compressed_frames;
			vector<vector<PXDTransform>> root_frames;
			vector<PXDTrackKeyframes> uncompressed_tracks;
			vector<PXDTrackKeyframes> uncompressed_root_tracks;
			bool has_root_motion;
		public:
			PXDAnimation();
			PXDAnimation(string filename);
			~PXDAnimation();
			void read(File *file);
			string getName();
			void setName(string v);
			bool getIsAdditive();
			bool getIsCompressed();
			float getDuration();
			unsigned int getFrameCount();
			unsigned int getTrackCount();
			float getFrameRate();
			bool getHasRootMotion();
			vector<vector<PXDTransform>> &getCompressedFrames();
			vector<vector<PXDTransform>> &getRootFrames();
			vector<PXDTrackKeyframes> &getUncompressedTracks();
			vector<PXDTrackKeyframes> &getUncompressedRootTracks();
	};
};
