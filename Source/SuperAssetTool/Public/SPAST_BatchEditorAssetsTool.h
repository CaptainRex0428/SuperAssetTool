// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define NULL_FSTRING ""

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "EditorUtilityLibrary.h"
#include "EditorActorFolders.h"

#include "PhysicsEngine/PhysicsAsset.h"

#include "AssetStandard.h"
#include "MessageStandard.h"

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
	UFUNCTION(BlueprintCallable, Category = "SPAST_Debug")
	void DebugTool();

	UFUNCTION(BlueprintCallable, Category = "SPAST_Debug")
	void DebugAssetsName();

	UFUNCTION(BlueprintCallable,Category = "SPAST_Debug")
	void DebugAssetsClass(TArray<FAssetData> AssetsData);


/*
Private Methods
*/
private:
	void DebugAssetClass(FAssetData AssetData);

	const FString GetPrefix(FAssetData& AssetData);
	const FString GetSTDPrefix(FAssetData & AssetData);
	bool CheckSTDPrefix(FAssetData & AssetData);
	void FixSTDPrefix(FAssetData & AssetData);
};
