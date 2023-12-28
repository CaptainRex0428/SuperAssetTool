#pragma once

#include "EditorUtilityBlueprint.h"

#include "Engine/Texture2DArray.h"

#include "Engine/Texture.h"

#include "PhysicsEngine/PhysicsAsset.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialInstanceConstant.h"

namespace AssetSTD {

	TMap<UClass*, FString> mAssetPrefix{
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

	enum AssetCategory
	{
		World,
		Character,
		Effect
	};

	struct sTextureInfoManaged {
		int Resolution;
		TextureCompressionSettings CompressionSettings;
		bool iSRGB;
		TextureGroup TxGroup;
	};

	TMap<TextureCompressionSettings, FString> mCompressionSettings2FSrting{
		{TC_Default,"Default (DXT1/5, BC1/3 on DX11)"},
		{TC_Normalmap,"Normalmap (DXT5, BC5 on DX11)"},
		{TC_Masks,"Masks (no sRGB)" },
		{TC_Grayscale,"Grayscale (G8/16, RGB8 sRGB)"},
		{TC_Displacementmap,"Displacementmap (G8/16)"},
		{TC_VectorDisplacementmap,"VectorDisplacementmap (RGBA8)"},
		{TC_HDR,"HDR (RGBA16F, no sRGB)"},
		{TC_EditorIcon,"UserInterface2D (RGBA)"},
		{TC_Alpha,"Alpha (no sRGB, BC4 on DX11)"},
		{TC_DistanceFieldFont,"DistanceFieldFont (G8)"},
		{TC_HDR_Compressed,"HDR Compressed (RGB, BC6H, DX11)"},
		{TC_BC7,"BC7 (DX11, optional A)"},
		{TC_HalfFloat,"Half Float (R16F)"},
		{TC_LQ,"Low Quality (BGR565/BGR555A1)"},
		{TC_EncodedReflectionCapture,"EncodedReflectionCapture"},
		{TC_SingleFloat,"Single Float (R32F)"},
		{TC_HDR_F32,"HDR High Precision (RGBA32F)"},
		{TC_MAX,"MAX"}
	};

	TMap<FString, sTextureInfoManaged> mTextureSubfix_World_Base{
		{"_D",{2048,TC_Default,true,TEXTUREGROUP_World}},
		{"_E",{512,TC_Default,true,TEXTUREGROUP_World}},
		{"_N",{1024,TC_Normalmap,false,TEXTUREGROUP_WorldNormalMap}},
		{"_ARMS",{512,TC_Masks,false,TEXTUREGROUP_World}},
		{"_Detail",{512,TC_VectorDisplacementmap,false,TEXTUREGROUP_World}},
		{"_Attr",{512,TC_VectorDisplacementmap,false,TEXTUREGROUP_World}}
	};

	TMap<FString, sTextureInfoManaged> mTextureSubfix_Effects_Base{
		{"_D",{512,TC_Default,true,TEXTUREGROUP_Effects}},
		{"_E",{512,TC_Default,true,TEXTUREGROUP_Effects}},
		{"_N",{512,TC_Normalmap,false,TEXTUREGROUP_Effects}},
		{"_ARMS",{256,TC_Masks,false,TEXTUREGROUP_Effects}},
		{"_Detail",{256,TC_VectorDisplacementmap,false,TEXTUREGROUP_Effects}},
		{"_Attr",{256,TC_VectorDisplacementmap,false,TEXTUREGROUP_Effects}}
	};

	TMap<FString, sTextureInfoManaged> mTextureSubfix_Char_Base{
		{"_D",{2048,TC_Default,true,TEXTUREGROUP_Character}},
		{"_E",{512,TC_Default,true,TEXTUREGROUP_Character}},
		{"_N",{1024,TC_Normalmap,false,TEXTUREGROUP_CharacterNormalMap}},
		{"_ARMS",{512,TC_Masks,false,TEXTUREGROUP_Character}},
		{"_Detail",{512,TC_VectorDisplacementmap,false,TEXTUREGROUP_Character}},
		{"_Attr",{512,TC_VectorDisplacementmap,false,TEXTUREGROUP_Character}}
	};

	TMap<FString, sTextureInfoManaged> mTextureSubfix_Char_Hair{
		{"_Attr",{2048,TC_VectorDisplacementmap,true,TEXTUREGROUP_Character}},
		{"_AO",{2048,TC_Grayscale,true,TEXTUREGROUP_Character}},
		{"_SDyeM",{512,TC_Grayscale,true,TEXTUREGROUP_Character}}
	};

	enum eTextureCategory {
		Base,
		Hair
	};

	TMap<eTextureCategory, TMap<FString, sTextureInfoManaged>> mTextureSubfix_Char{
		{Base,mTextureSubfix_Char_Base},
		{Hair,mTextureSubfix_Char_Hair}
	};
}