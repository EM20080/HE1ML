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

#include <list>
#include <string>

#ifndef _M_X64
#include "Havok.h"
#endif

#define LIBGENS_HAVOK_PHYSICS_EXTENSION     ".phy.hkx"

namespace LibGens {
	class HavokPhysicsCache;
	class HavokSkeletonCache;
	class HavokAnimationCache;

	class HavokEnviroment {
		protected:
			std::list<HavokPhysicsCache *> physics_cache;
			std::list<HavokSkeletonCache *> skeleton_cache;
			std::list<HavokAnimationCache *> animation_cache;
			std::list<std::string> search_paths;
		public:
			HavokEnviroment(int bufferSize);
			void loadHavokFile(std::string filename, std::string animation_name="");
#ifndef _M_X64
		void dumpPhysics(hkpPhysicsData*);
		void dumpPhysicsSystem(hkpPhysicsSystem&);
		void dumpRigidbody(hkpRigidBody&);
#endif
			void addFolder(std::string folder);
			HavokPhysicsCache *getPhysics(std::string physics_name);
			HavokSkeletonCache *getSkeleton(std::string skeleton_name);
			HavokAnimationCache *getAnimation(std::string animation_name);
			bool deletePhysicsEntry(std::string physics_name);
	};
};