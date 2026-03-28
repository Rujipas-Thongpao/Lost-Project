#pragma once

#include "MeshData.h"
#include "MaterialData.h"
#include "BoneData.h"

class ModelData {
public:
	MeshData mc;
	MaterialData mat;
	BoneData b;
};
