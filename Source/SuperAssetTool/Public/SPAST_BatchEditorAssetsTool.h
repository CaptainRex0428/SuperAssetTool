#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"
#include "EditorActorFolders.h"

#include "PhysicsEngine/PhysicsAsset.h"

#include "AssetStandard.h"
#include "MessageStandard.h"
#include "SPAST_Math.h"

#include <string>
#include <regex>
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
	void DebugTool();

	/**
	* @note ----> CallInEditor
	*/
	UFUNCTION(CallInEditor, Category = "SPAST_Check")
	void CheckAssetsClass();

// ------------------------------Public Functions------------------------------//
	
	// check assets
	UFUNCTION(BlueprintCallable, Category = "SPAST_Check")
	void SPAST_CheckAssetsName(TArray<FAssetData> AssetsData);

	// check assets class
	UFUNCTION(BlueprintCallable, Category = "SPAST_Check")
	void SPAST_CheckAssetsClass(TArray<FAssetData> AssetsData);

	// duplicate assets
	UFUNCTION(BlueprintCallable, Category = "SPAST_Modify")
	TArray<FAssetData> SPAST_DuplicateAssets(TArray<FAssetData> AssetsData, FString SubfolderName = "Duplicated Assets",
		bool UseSubfolder = false, bool OverwriteExists = false,
		int DuplicateNum = 1);


//------------------------------Private Functions------------------------------//

private:
	void CheckAssetClass(FAssetData AssetData);

	// Prefix
	const FString GetPrefix(FAssetData& AssetData);
	const FString GetSTDPrefix(FAssetData & AssetData);
	int CheckSTDPrefix(FAssetData & AssetData);
	void FixSTDPrefix(FAssetData & AssetData);

	// Paths
	std::tuple<FString, FString, FString> GetAssetPathInfo(FAssetData AssetData);

	//duplicate
	TArray<FAssetData> DuplicateAsset(FString DestinationDirectory, FString SourceAssetPath, int DuplicateNum, bool overwrite);

	// Subfix
	int MaxNumSubfix(FString DirectoryPath, FString AssetName);



};