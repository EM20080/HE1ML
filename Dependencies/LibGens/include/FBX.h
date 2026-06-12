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

#include <fbxsdk.h>
#include <fbxsdk/fileio/fbxiosettings.h>

#define LIBGENS_FBX_DEFAULT_SCENE_NAME_IMPORT    "FBXImport"
#define LIBGENS_FBX_DEFAULT_SCENE_NAME_EXPORT    "FBXExport"

#ifdef _M_X64
struct hkaSkeleton;
struct hkaAnimationBinding;
struct hkaAnimation;
#else
class hkaSkeleton;
struct hkGeometry;
class hkaAnimationBinding;
class hkaAnimation;
class hkaAnimatedSkeleton;
#endif

namespace LibGens {
	class Bone;
	class Submesh;
	class Model;
	class TerrainInstance;
	class Material;
	class MaterialLibrary;
	class HavokSkeletonCache;
	class HavokAnimationCache;
	class PXDSkeleton;
	class PXDAnimation;

	class FBX {
		protected:
			list<Model *> models;
			list<TerrainInstance *> instances;
			map<Material *, FbxSurfacePhong *> material_map;
			MaterialLibrary *material_library;
			FbxScene *scene;
			FbxPose* bind_pose;
#ifndef _M_X64
			hkaAnimatedSkeleton *animated_skeleton;
#endif
		public:
			FBX(FbxManager *sdk_manager, string scene_name=LIBGENS_FBX_DEFAULT_SCENE_NAME_EXPORT);
			FBX(FbxScene* lScene);
			~FBX();
			FbxSurfacePhong *addMaterial(Material *material);
		FbxMesh* addNode(Model* model, HavokSkeletonCache* skeleton = NULL, vector<HavokAnimationCache*> animations = {}, Matrix4 transform_matrix = Matrix4(), bool skin = true);
		FbxMesh* addNamedNode(string name, Model* model, HavokSkeletonCache* skeleton = NULL, vector<HavokAnimationCache*> animations = {}, Matrix4 transform_matrix = Matrix4(), bool skin = true);
#ifdef _M_X64
		FbxMesh* addNamedNode(string name, Model* model, Matrix4 transform_matrix, bool skin = true);
#endif
			FbxMesh* addTerrainInstance(TerrainInstance* instance);
			void skinModelToSkeleton(Model *model, FbxMesh *model_mesh, vector<FbxNode *> &skeleton_bones, FbxAMatrix lSkinMatrix);
		void addMorphModels(Model *model, vector<FbxNode *> &skeleton_bones, FbxAMatrix lSkinMatrix);

		FbxNode *addHavokSkeleton(vector<FbxNode *> &skeleton_bones, hkaSkeleton *skeleton);
		FbxNode *addHavokBone(FbxNode *parent_node, unsigned int parent_index, vector<FbxNode *> &skeleton_bones, hkaSkeleton *skeleton);
		void addHavokAnimation(vector<FbxNode *> &skeleton_bones, hkaSkeleton *skeleton, hkaAnimationBinding *animation_binding, hkaAnimation *animation, const string& animation_name);
#ifndef _M_X64
		FbxMesh *addHavokCollision(string name, hkGeometry *geometry, Matrix4 transform);
#endif
			void addSkeleton(vector<FbxNode *>& skeleton_bones, Model *model);
			void addBone(FbxNode *parent_node, unsigned int parent_index, vector<FbxNode *>& skeleton_bones, vector<Bone *>& bones);

		FbxNode *addPXDSkeleton(vector<FbxNode *> &skeleton_bones, PXDSkeleton *skeleton);
		FbxNode *addPXDBone(FbxNode *parent_node, unsigned int parent_index, vector<FbxNode *> &skeleton_bones, PXDSkeleton *skeleton);
		void addPXDAnimation(vector<FbxNode *> &skeleton_bones, PXDSkeleton *skeleton, PXDAnimation *animation);
		FbxMesh *addPXDNode(Model *model, PXDSkeleton *skeleton, vector<PXDAnimation *> animations, Matrix4 transform_matrix = Matrix4(), bool skin = true);

			list<Model *> getModels();
			list<TerrainInstance *> getInstances();
			void setMaterialLibrary(MaterialLibrary *v);
			MaterialLibrary *getMaterialLibrary();
			FbxScene *getScene();

			Material *buildMaterialFromFBX(FbxSurfaceMaterial *lMaterial);
			Submesh *buildSubmeshFromFBX(FbxMesh *lMesh, int material_index, bool single_material=false);
			Model *buildModelFromFBX(FbxMesh *lMesh, int material_count, vector<string> material_names);
			TerrainInstance *buildTerrainInstanceFromFBX(FbxNode *lNode);
	};
};
