/*  Havok Format Library
    Copyright(C) 2016-2022 Lukas Cone

    This program is free software : you can redistribute it and / or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once
#include "hk_base.hpp"

struct hkaMeshBinding : IhkVirtualClass {
  DECLARE_HKCLASS(hkaMeshBinding)

  virtual const char *OriginalSkeletonName() const { return ""; }
  virtual const char *Name() const { return ""; }
  virtual const IhkVirtualClass *Mesh() const { return nullptr; }
  virtual const hkaSkeleton *Skeleton() const { return nullptr; }
  virtual size_t GetNumBoneFromSkinMeshTransforms() const { return 0; }
  virtual const float *GetBoneFromSkinMeshTransform(size_t) const {
    return nullptr;
  }
};
