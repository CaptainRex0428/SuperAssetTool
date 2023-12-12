#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "EditorUtilityLibrary.h"
#include "EditorActorFolders.h"

#include "PhysicsEngine/PhysicsAsset.h"

#include "AssetStandard.h"
#include "MessageStandard.h"

#include <string>
#include <regex>
#include <tuple>

#define NULL_FSTRING ""

#define CHECK_RESULT_3State(judge_condition ,undefine,positive,negtive) judge_condition> 1 ? undefine : CheckSTDPrefix(AssetData)? positive : negtive

#include "SPAST_BatchEditorAssetsTool.generated.h"
/*
* 
 */
UCLASS()
class SUPERASSETTOOL_API USPAST_BatchEditorAssetsTool : public UAssetActionUtility
{
	GENERATED_BODY()
	
/*
Public Properties
*/
public:

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "")
	TArray<FAssetData> SelectedAssetsData;

/*
Private Properties
*/
private:


/*
Public Methods
*/
public:

	// Functions Call in Editor
	UFUNCTION(BlueprintCallable,CallInEditor, Category = "SPAST_Debug")
	void DebugTool();

	// check assets
	UFUNCTION(BlueprintCallable, Category = "SPAST_Check")
	void CheckAssetsName(TArray<FAssetData> AssetsData);

	UFUNCTION(BlueprintCallable,Category = "SPAST_Check")
	void CheckAssetsClass(TArray<FAssetData> AssetsData);

	// modify assets
	UFUNCTION(BlueprintCallable, Category = "SPAST_Modify")
	TArray<FAssetData> DuplicateAssets(TArray<FAssetData> AssetsData, FString SubfolderName = "Duplicated Assets",
		bool UseSubfolder = false, bool OverwriteExists = false,
		int DuplicateNum = 1);


/*
Private Methods
*/


private:
	void CheckAssetClass(FAssetData AssetData);

	// Prefix
	const FString GetPrefix(FAssetData& AssetData);
	const FString GetSTDPrefix(FAssetData & AssetData);
	int CheckSTDPrefix(FAssetData & AssetData);
	void FixSTDPrefix(FAssetData & AssetData);

	// Paths
	std::tuple<FString, FString, FString> GetAssetPathInfo(FAssetData AssetData);
};
