#pragma once


#include "EditorUtilityBlueprint.h"

#include "Engine/Texture2DArray.h"

#include "PhysicsEngine/PhysicsAsset.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialInstanceConstant.h"

namespace AssetSTD {

	TMap<UClass*, FString> EditorPrefix{
		{UBlueprint::StaticClass(), "BP_"},
		{UEditorUtilityBlueprint::StaticClass(),"EUBP_"},
		
		{UTexture::StaticClass(),"T_"},
		{UTexture2D::StaticClass(),"T_"},
		{UTexture2DArray::StaticClass(),"TArr_"},

		{UMaterial::StaticClass(),"M_"},
		{UMaterialInstance::StaticClass(),"MI_"},
		{UMaterialInstanceConstant::StaticClass(),"MI_"},
		{UMaterialParameterCollection::StaticClass(),"MPC_"},
		
		{UStaticMesh::StaticClass(),"SM_"},
		{USkeletalMesh::StaticClass(),"SK_"},
		{USkeleton::StaticClass(),"SKEL_"},
		{UPhysicsAsset::StaticClass(),"PHYS_"}
	};

}