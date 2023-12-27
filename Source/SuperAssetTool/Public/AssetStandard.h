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

	struct sTextureStandardInfo {
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

	TMap<FString, sTextureStandardInfo> mTextureSubfix_Base{
		{"_D",{2048,TC_Default,true}},
		{"_E",{512,TC_Default,true}},
		{"_N",{1024,TC_Normalmap,false}},
		{"_ARMS",{512,TC_Masks,false}},
		{"_Detail",{512,TC_VectorDisplacementmap,false}},
		{"_Attr",{512,TC_VectorDisplacementmap,false}}
	};

	TMap<FString, sTextureStandardInfo> mTextureSubfix_Hair{
		{"_Attr",{2048,TC_VectorDisplacementmap,true}},
		{"_AO",{2048,TC_Grayscale,true}},
		{"_SDyeM",{512,TC_Grayscale,true}}
	};

	enum eTextureCategory {
		Base,
		Hair
	};

	TMap<eTextureCategory, TMap<FString, sTextureStandardInfo>> mTextureSubfix{
		{Base,mTextureSubfix_Base},
		{Hair,mTextureSubfix_Hair}
	};
}