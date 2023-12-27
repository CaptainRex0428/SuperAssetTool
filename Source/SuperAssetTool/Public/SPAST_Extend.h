// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"

#include "SPAST_Extend.generated.h"

/**
 * 
 */
UCLASS()
class SUPERASSETTOOL_API USPAST_Extend : public UAssetActionUtility
{
	GENERATED_BODY()
	
public:
	static bool NameContains(FName name, std::string str);
};


