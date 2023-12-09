#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "EditorUtilityLibrary.h"
#include "EditorActorFolders.h"

#include "PhysicsEngine/PhysicsAsset.h"

#include "AssetStandard.h"
#include "MessageStandard.h"

#include <iostream>
#include <string>
#include <regex>
#include <format>

#define NULL_FSTRING ""

#define ColumWidth_1 20
#define ColumWidth_2 35

#define ColumnPatternContent "|{0:{1}}|{2:{3}}|"
#define ColumnPatternHeader "|{0:-<{1}}|{2:-^{3}}|"
#define ColumnPatternFooter "|{0:><{1}}|{2:=^{3}}|"
#define Column(colum_pattern,column1_content,column2_content) std::format(colum_pattern, column1_content, ColumWidth_1, column2_content, ColumWidth_2)

#define CHECKPREFIX(undefine,positive,negtive) CheckSTDPrefix(AssetData)> 1 ? undefine : CheckSTDPrefix(AssetData)? positive : negtive

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
	UFUNCTION(BlueprintCallable,CallInEditor, Category = "SPAST_Debug")
	void DebugTool();

	UFUNCTION(BlueprintCallable, Category = "SPAST_Check")
	void CheckAssetsName(TArray<FAssetData> AssetsData);

	UFUNCTION(BlueprintCallable,Category = "SPAST_Check")
	void CheckAssetsClass(TArray<FAssetData> AssetsData);


/*
Private Methods
*/
private:
	void CheckAssetClass(FAssetData AssetData);

	const FString GetPrefix(FAssetData& AssetData);
	const FString GetSTDPrefix(FAssetData & AssetData);
	int CheckSTDPrefix(FAssetData & AssetData);
	void FixSTDPrefix(FAssetData & AssetData);
};
