#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"
#include "EditorActorFolders.h"

#include "PhysicsEngine/PhysicsAsset.h"

#include "AssetStandard.h"
#include "MessageStandard.h"
#include "DebugMessage.h"
#include "SPAST_Math.h"

#include <string>
#include <tuple>

#define NULL_FSTRING ""

#define CHECK_RESULT_3State(judge_condition ,undefine,positive,negtive) judge_condition> 1 ? undefine : CheckSTDPrefix(AssetData)? positive : negtive

#include "SPAST_BatchEditorAssetsTool.generated.h"

/*
* @brief Batch Rename, Batch Check, Batch Modify
 */
UCLASS()
class SUPERASSETTOOL_API USPAST_BatchEditorAssetsTool : public UAssetActionUtility
{
	GENERATED_BODY()
	
// ------------------------------Public Properties------------------------------//

public:

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "")
	TArray<FAssetData> SelectedAssetsData;

// ------------------------------Private Properties------------------------------//

private:


public:

// ------------------------------Public Functions------------------------------//
// ------------------------------(Call In Editor)------------------------------//

	/**
	* @note ----> CallInEditor
	*/
	UFUNCTION(CallInEditor, Category = "SPAST_Debug")
	void dbTool();

	/**
	* @note ----> CallInEditor
	*/
	UFUNCTION(CallInEditor, Category = "SPAST_Check")
	void cClass();

	UFUNCTION(CallInEditor, Category = "SPAST_Check")
	void cTexture();

	/**
	* @note ----> CallInEditor
	*/
	UFUNCTION(CallInEditor, Category = "SPAST_Modify")
	void mdDuplicate(FString SubfolderName,
		bool UseSubfolder = false, bool OverwriteExists = false,
		int DuplicateNum = 1);


// ------------------------------Public Functions------------------------------//
	
	// check assets
	UFUNCTION(BlueprintCallable, Category = "SPAST_Check")
	void SPAST_CheckAssetsName(TArray<FAssetData> AssetsData);

	// check assets class
	UFUNCTION(BlueprintCallable, Category = "SPAST_Check")
	void SPAST_CheckAssetsClass(TArray<FAssetData> AssetsData);

	UFUNCTION(BlueprintCallable, Category = "SPAST_Check")
	void SPAST_CheckAssetsTexture(TArray<FAssetData> AssetsData);

	// duplicate assets
	UFUNCTION(BlueprintCallable, Category = "SPAST_Modify")
	TArray<FAssetData> SPAST_DuplicateAssets(TArray<FAssetData> AssetsData,
		FString SubfolderName,
		bool UseSubfolder, bool OverwriteExists,
		int DuplicateNum);


//------------------------------Private Functions------------------------------//

private:
	void CheckAssetClass(FAssetData AssetData);

	// Prefix
	const FString GetPrefix(FAssetData& AssetData);
	const FString GetSTDPrefix(FAssetData & AssetData);
	int CheckSTDPrefix(FAssetData & AssetData);
	void FixSTDPrefix(FAssetData & AssetData);

	//subfix
	const FString GetSubfix(FAssetData& AssetData);
	
	const AssetSTD::sTextureInfoManaged * GetSTDTextureInfo(FString& subfix, TMap<FString, AssetSTD::sTextureInfoManaged> & textureSTDCategory);
	TMap<FString, AssetSTD::sTextureInfoManaged> * GetSTDTextureCategoty(FAssetData assetData, AssetSTD::AssetCategory AssetCategory);

	const AssetSTD::sTextureInfoManaged* GetTexture2DInfo(FAssetData assetData);

	FAssetData setTexture2DInfo(UTexture2D * TextureObject,AssetSTD::sTextureInfoManaged textureInfo);

	// Paths
	std::tuple<FString, FString, FString> GetAssetPathInfo(FAssetData AssetData);

	//duplicate
	TArray<FAssetData> DuplicateAsset(FString DestinationDirectory, FString SourceAssetPath, int DuplicateNum, bool overwrite);

	// Subfix
	int MaxNumSubfix(FString DirectoryPath, FString AssetName);

};