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

#define LIBGENS_MILLER_PAC_SPLIT_BYTES_LIMIT 0x1E00000
#define LIBGENS_MILLER_PAC_ALIGNMENT 16
#define LIBGENS_MILLER_PAC_LZ4_CHUNK_SIZE 65536

namespace LibGens {
	class MillerPacExtension {
		public:
			const char *extension;
			const char *type;
			int split;
	};

	class MillerPacFile {
		public:
			string filename;
			string name;
			string extension;
			string type;
			vector<unsigned char> data;
			unsigned short split_index;
			int split;
	};

	class MillerPac {
		protected:
			vector<MillerPacFile *> files;
			vector<string> parent_paths;
			string source_filename;
			const MillerPacExtension *extensions;
			size_t extension_count;
			int version;
			int internal_version;
			int compressed;

			void addFile(string filename, string pac_name);
			void addFolder(string folder, string relative_folder);
			void splitFiles();
			const MillerPacExtension *findExtension(string extension);
		public:
			MillerPac();
			MillerPac(string filename);
			~MillerPac();

			void readFile(string filename);
			void addFolder(string folder);
			void save(string filename);
			void extract(string folder);
	};
};
