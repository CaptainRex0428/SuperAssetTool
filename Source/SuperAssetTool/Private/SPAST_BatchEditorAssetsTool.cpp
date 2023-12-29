#include "SPAST_BatchEditorAssetsTool.h"

// ------------------------------Public Functions------------------------------//
// ------------------------------(Call In Editor)------------------------------//

void USPAST_BatchEditorAssetsTool::dbTool(FName paramterName) {

	for (auto asset : UEditorUtilityLibrary::GetSelectedAssetsOfClass(UMaterialInstance::StaticClass())) {
		UMaterialInstance* instance = (UMaterialInstance*)asset;
		EnableMaterialInstanceEidtorOnlyParamter(instance, paramterName, EMaterialParameterType::StaticSwitch);
	}
	
}

void USPAST_BatchEditorAssetsTool::cClass() {
	SPAST_CheckAssetsClass(UEditorUtilityLibrary::GetSelectedAssetData());
}

void USPAST_BatchEditorAssetsTool::cTexture() {
	SPAST_CheckAssetsTexture(UEditorUtilityLibrary::GetSelectedAssetData());
}

void USPAST_BatchEditorAssetsTool::mdDuplicate(FString SubfolderName,
	bool UseSubfolder, bool OverwriteExists,
	int DuplicateNum) {
	SPAST_DuplicateAssets(UEditorUtilityLibrary::GetSelectedAssetData(), SubfolderName, UseSubfolder, OverwriteExists, DuplicateNum);
}


// ------------------------------Public Functions------------------------------//

void USPAST_BatchEditorAssetsTool::SPAST_CheckAssetsName(TArray<FAssetData> AssetsData) {
	SPAST_Print("DebugAssetsName is under development.",MSGTARGET_SCREEN_OUTPUTLOG);
}

void USPAST_BatchEditorAssetsTool::SPAST_CheckAssetsClass(TArray<FAssetData> AssetsData) {

	int count = 0;

	for (auto AssetData : AssetsData) {
		count++;

		// output log header
		FString Column_Header = TABLESTRING_2Column(TablePattern_2Column_Header, "Index", count).c_str();
		SPAST_PrintLog_OUTPUTLOG(Column_Header,MsgSTD::SPAST_MSG_Tips);

		CheckAssetClass(AssetData);
	}

	// output log footer
	FString Column_Footer = TABLESTRING_2Column(TablePattern_2Column_Footer, "Assets Count", count).c_str();
	SPAST_PrintLog_OUTPUTLOG(Column_Footer, MsgSTD::SPAST_MSG_Tips);
	
	// output screen count
	SPAST_PrintLog_SCREEN(FString::FromInt(count),MsgSTD::SPAST_MSG_Tips);

}

void USPAST_BatchEditorAssetsTool::SPAST_CheckAssetsTexture(TArray<FAssetData> AssetsData) {
	for (auto asset : AssetsData) {
		auto info = GetTexture2DInfo(asset);

		if (info) {
			SPAST_Print(TABLESTRING_2Column(TablePattern_2Column_Content, "Resolution", info->Resolution).c_str());
			SPAST_Print(TABLESTRING_2Column(TablePattern_2Column_Content, "sRGB", info->iSRGB ? "sRGB" : "No sRGB").c_str());
			SPAST_Print(TABLESTRING_2Column(TablePattern_2Column_Content, "Compression Settings", TCHAR_TO_UTF8(**AssetSTD::mCompressionSettings2FSrting.Find(info->CompressionSettings))).c_str());
			SPAST_Print(TABLESTRING_2Column(TablePattern_2Column_Content, "Texture Group", TCHAR_TO_UTF8(UTexture::GetTextureGroupString(info->TxGroup))).c_str());
		}
	}
};

/**
* @brief Duplicate a series assets. Overwrite is not recommended for material type assets.
*
* @param <AssetsData>		[TArray<FAssetData>] UEditorUtilityLibrary::GetSelectedAssetData()
* @param <SubfolderName>	[FString] The sub folder name to duplicate the new assets.
* @param <UseSubfolder>		[bool] Whether use sub folder to duplicate the new assets.
* @param <OverwriteExists>	[bool]	  Whether overwrite the existed asset. index will start with 1. This is not recommended for material type assets.
* @param <DuplicateNum>		[int]	  How many asset to duplicate.
* 
* @return [TArray<FAssetData>] An array of duplicated assets.
*/
TArray<FAssetData> USPAST_BatchEditorAssetsTool::SPAST_DuplicateAssets(TArray<FAssetData> AssetsData, 
	FString SubfolderName,
	bool UseSubfolder,
	bool OverwriteExists,
	int DuplicateNum)
{
	TArray<FAssetData> DuplicatedAssets;

	for (auto Asset : AssetsData) {
		auto AssetPathInfo = GetAssetPathInfo(Asset);
		
		if (UseSubfolder) {
			FString duplicatedDirectoryName = SubfolderName == NULL_FSTRING || SubfolderName == std::get<0>(AssetPathInfo) ? (std::get<0>(AssetPathInfo) + "_Duplicated") : SubfolderName;
			FString destinationDirectory = FPaths::Combine(std::get<1>(AssetPathInfo), duplicatedDirectoryName);
			
			if (!UEditorAssetLibrary::DoesDirectoryExist(destinationDirectory)) {
				UEditorAssetLibrary::MakeDirectory(destinationDirectory);
			}

			DuplicatedAssets.Append(DuplicateAsset(destinationDirectory, std::get<2>(AssetPathInfo),DuplicateNum,OverwriteExists));

		}
		else {
			FString destinationDirectory = std::get<1>(AssetPathInfo);

			DuplicatedAssets.Append(DuplicateAsset(destinationDirectory, std::get<2>(AssetPathInfo), DuplicateNum, OverwriteExists));
		}
	}

	return DuplicatedAssets;
}

//------------------------------Private Functions------------------------------//

void USPAST_BatchEditorAssetsTool::CheckAssetClass(FAssetData AssetData) {
	
	std::string AssetNameResult = TCHAR_TO_UTF8(*AssetData.AssetName.ToString());
	std::string AssetClassResult = TCHAR_TO_UTF8(*AssetData.GetClass()->GetName());
	std::string AssetPrefixCheck = CHECK_RESULT_3State(CheckSTDPrefix(AssetData),"-","V","X");

	// output log
	SPAST_PrintLog_OUTPUTLOG(TABLESTRING_2Column(TablePattern_2Column_Content, "Asset Name", AssetNameResult).c_str() ,MsgSTD::SPAST_MSG_Tips);
	SPAST_PrintLog_OUTPUTLOG(TABLESTRING_2Column(TablePattern_2Column_Content, "Asset Class", AssetClassResult).c_str(), MsgSTD::SPAST_MSG_Tips);
	SPAST_PrintLog_OUTPUTLOG(TABLESTRING_2Column(TablePattern_2Column_Content, "Asset Prefix Check",AssetPrefixCheck).c_str(), MsgSTD::SPAST_MSG_Tips);

	// output screen
	SPAST_PrintLog_SCREEN(std::format("|{:^3}|{:^25}|{:^25}|",AssetPrefixCheck,AssetClassResult,AssetNameResult).c_str(), MsgSTD::SPAST_MSG_Tips);
}

 const FString USPAST_BatchEditorAssetsTool::GetPrefix(FAssetData& AssetData) {
	FString assetname = AssetData.AssetName.ToString();

	std::string assetname_str = TCHAR_TO_UTF8(*assetname);

	bool prefix_found = false;
	std::string prefix;

	const char* split = "_";

	for (auto index : assetname_str) {
		if (index == *split) {
			prefix_found = true;
			prefix = assetname_str.substr(0, assetname_str.find(index)+1);
			break;
		};
	}

	return prefix_found ? prefix.c_str(): NULL_FSTRING;
}

const FString USPAST_BatchEditorAssetsTool::GetSTDPrefix(FAssetData& AssetData) {
	auto STD_Prefix = AssetSTD::mAssetPrefix.Find(AssetData.GetClass());
	return STD_Prefix ? * STD_Prefix : NULL_FSTRING;
}

int USPAST_BatchEditorAssetsTool::CheckSTDPrefix(FAssetData& AssetData) {
	
	FString AssetPrefix = GetPrefix(AssetData);
	FString AssetSTDPrefix = GetSTDPrefix(AssetData);
	
	if (AssetSTDPrefix == NULL_FSTRING) {
		return 2;
	}
	else {
		if (AssetSTDPrefix == AssetPrefix) {
			return 1;
		}
		else {
			return 0;
		}
	}

}

void USPAST_BatchEditorAssetsTool::FixSTDPrefix(FAssetData& AssetData) {

}

const FString USPAST_BatchEditorAssetsTool::GetSubfix(FAssetData& AssetData) {
	FString assetname = AssetData.AssetName.ToString();

	std::string assetname_str = TCHAR_TO_UTF8(*assetname);
	char * assetname_ptr = TCHAR_TO_UTF8(*assetname);
	int assetname_len = assetname_str.length();

	bool subfix_found = false;
	std::string subfix;

	const char* split = "_";

	for (int index = 0; index < assetname_len; index++) {
		char * checkptr = assetname_ptr + assetname_len - 1 - index;
		if (*checkptr == *split) {
			subfix_found = true;
			subfix = assetname_str.substr(assetname_len - (index+1));
			break;
		}
	}

	return subfix_found ? subfix.c_str() : NULL_FSTRING;

};

const AssetSTD::sTextureInfoManaged * USPAST_BatchEditorAssetsTool::GetSTDTextureInfo(FString& subfix, TMap<FString, AssetSTD::sTextureInfoManaged>& textureSTDCategory)
{
	return textureSTDCategory.Find(subfix);
};

TMap<FString, AssetSTD::sTextureInfoManaged>* USPAST_BatchEditorAssetsTool::GetSTDTextureCategoty(FAssetData assetData, AssetSTD::AssetCategory AssetCategory)
{
	if (!assetData.IsInstanceOf(UTexture2D::StaticClass())) {
		return nullptr;
	}

	if (AssetCategory == AssetSTD::Character) {
		if (assetData.AssetName.ToString().Contains("_Hair"))
			return AssetSTD::mTextureSubfix_Char.Find(AssetSTD::Hair);
		else
			return AssetSTD::mTextureSubfix_Char.Find(AssetSTD::Base);
	}
	else if (AssetCategory == AssetSTD::Effect)
	{
		return & AssetSTD::mTextureSubfix_Effects_Base;
	}
	else {
		return &AssetSTD::mTextureSubfix_World_Base;
	}
};

const AssetSTD::sTextureInfoManaged * USPAST_BatchEditorAssetsTool::GetTexture2DInfo(FAssetData assetdata)
{
	if (!assetdata.IsInstanceOf(UTexture2D::StaticClass()))
		return nullptr;

	AssetSTD::sTextureInfoManaged * result = new AssetSTD::sTextureInfoManaged;
	
	UTexture2D* assetObj = (UTexture2D*)UEditorAssetLibrary::LoadAsset(assetdata.GetObjectPathString());
	
	result->CompressionSettings = assetObj->CompressionSettings;
	result->Resolution = std::max(assetObj->GetImportedSize().X, assetObj->GetImportedSize().Y);
	result->iSRGB = assetObj->SRGB;
	result->TxGroup = assetObj->LODGroup;

	return result;
};

FAssetData USPAST_BatchEditorAssetsTool::setTexture2DInfo(UTexture2D* TextureObject, AssetSTD::sTextureInfoManaged textureInfo) {
	TextureObject->CompressionSettings = textureInfo.CompressionSettings;
	TextureObject->SRGB = textureInfo.iSRGB;
	TextureObject->LODGroup = textureInfo.TxGroup;

	return TextureObject;
};


/**
* @brief Return asset name, asset package name(directory path), asset full path
* 
* @return A 3-element tuple(std::tuple) about asset path info.\n
* @return ---------------------
* @retval std::get<0> AssetName
* @retval std::get<1> AssetPackagePath
* @retval std::get<2> AssetFullPath
*		
* @param <AssetData> Asset Data in UE mode
*/
std::tuple<FString, FString, FString> USPAST_BatchEditorAssetsTool::GetAssetPathInfo(FAssetData AssetData) {
	return { AssetData.AssetName.ToString(),AssetData.PackagePath.ToString(), AssetData.GetObjectPathString() };
};

/**
 * @brief Duplicate a single asset. Overwrite is not recommended for material type assets.
 * 
 * @param <DestinationDirectory>[FString] The folder name to duplicate the asset.
 * @param <SourceAssetPath>		[FString] The source asset path.
 * @param <DuplicateNum>		[int]	  How many asset to duplicate.
 * @param <Overwrite>			[bool]	  Whether overwrite the existed asset. index will start with 1. This is not recommended for material type assets.
 * 
 * @return [TArray<FAssetData>] An array of duplicated assets. 
 */
TArray<FAssetData> USPAST_BatchEditorAssetsTool::DuplicateAsset(FString DestinationDirectory, FString SourceAssetPath, int DuplicateNum, bool overwrite) 
{
	TArray<FAssetData> DuplicatedAssets;

	FString assetName = FPaths::GetBaseFilename(SourceAssetPath);
	
	int first_subfix = overwrite ? 1:MaxNumSubfix(DestinationDirectory, assetName) + 1;
	
	for (int count = 0; count < DuplicateNum; count++) {
		FString DestinationAssetPath = FPaths::Combine(DestinationDirectory, assetName + "_" + FormatInt2String((first_subfix + count), 2).c_str());

		if (UEditorAssetLibrary::DoesAssetExist(DestinationAssetPath) && overwrite) {
			UEditorAssetLibrary::DeleteLoadedAsset(UEditorAssetLibrary::LoadAsset(DestinationAssetPath));
		}

		FAssetData assetDuplicated = UEditorAssetLibrary::DuplicateAsset(SourceAssetPath,DestinationAssetPath);
		UEditorAssetLibrary::SaveAsset(assetDuplicated.GetObjectPathString());
		DuplicatedAssets.Add(assetDuplicated);
	}

	return DuplicatedAssets;
};

/**
* @brief Get the max subfix of the existing replicates of a specific asset in a specific directory(recursive false).
* 
* @param <DirectoryPath> [FString]
* @param <AssetName> [FString]
* 
* @return [int] the max existing subfix.
* 
*/
int USPAST_BatchEditorAssetsTool::MaxNumSubfix(FString DirectoryPath, FString AssetName) {
	
	int maxSubfix = 0;

	TArray<FString> AssetsList = UEditorAssetLibrary::ListAssets(DirectoryPath, false);

	for (auto asset : AssetsList) {

		FString basename = FPaths::GetBaseFilename(asset);

		if (basename.StartsWith(AssetName) && basename != AssetName) 
		{
			SPAST_Print("StartFind2");

			int sourceLength = AssetName.Len();

			const char * stdAssetName = TCHAR_TO_UTF8(*basename);
			const char * subfix = stdAssetName + sourceLength + 1;

			auto result = String2Int(std::string(subfix));

			if (result.success) {
				maxSubfix = maxSubfix > result.num ? maxSubfix : result.num;
			}
		};
	}

	return maxSubfix;
};

TArray<struct FTextureParameterValue> & USPAST_BatchEditorAssetsTool::EnableMarerialInstanceEditorOnlyTextureParameter(UMaterialInstance* instance, FName paramterName) {
	TArray< FMaterialParameterInfo > OutParameterInfo;
	TArray< FGuid > OutParameterGUID;
	instance->GetAllTextureParameterInfo(OutParameterInfo, OutParameterGUID);

	for (FTextureParameterValue value: instance->TextureParameterValues) {
		if (value.ParameterInfo.Name == paramterName) {
			return instance->TextureParameterValues;
		}
	}

	for (FMaterialParameterInfo parameterInfo : OutParameterInfo) {
		if (parameterInfo.Name == paramterName) {
			
			UTexture* textureObj;
			instance->GetTextureParameterDefaultValue(parameterInfo, textureObj);

			FTextureParameterValue txv = (FTextureParameterValue)parameterInfo;
			txv.ParameterValue = textureObj;

			instance->TextureParameterValues.Emplace(txv);
		}
	}

	return instance->TextureParameterValues;
};

TArray<struct FTextureParameterValue> & USPAST_BatchEditorAssetsTool::EnableMarerialInstanceEditorOnlyTextureParameters(UMaterialInstance* instance, TArray<FName> paramterNames) {
	TArray< FMaterialParameterInfo > OutParameterInfo;
	TArray< FGuid > OutParameterGUID;
	instance->GetAllTextureParameterInfo(OutParameterInfo, OutParameterGUID);

	for (FMaterialParameterInfo parameterInfo : OutParameterInfo) {

		if ( paramterNames.Contains(parameterInfo.Name)) {

			bool bUnEnabled = false;

			for (FTextureParameterValue value : instance->TextureParameterValues) {
				
				if (value.ParameterInfo.Name == parameterInfo.Name) {
					bUnEnabled = true;
				}
			}

			if (bUnEnabled) {
				UTexture* texDefault;
				instance->GetTextureParameterDefaultValue(parameterInfo, texDefault);

				FTextureParameterValue txv = (FTextureParameterValue)parameterInfo;
				txv.ParameterValue = texDefault;

				instance->TextureParameterValues.Emplace(txv);
			}
			
			
		}
	}

	return instance->TextureParameterValues;
};

TArray<struct FTextureParameterValue> & USPAST_BatchEditorAssetsTool::DisableMarerialInstanceEditorOnlyTextureParameter(UMaterialInstance* instance, FName paramterName) {
	for (auto TextureInfo : instance->TextureParameterValues) {
		if (TextureInfo.ParameterInfo.Name == paramterName) {
			instance->TextureParameterValues.Remove(TextureInfo);
		}
	}

	return instance->TextureParameterValues;
}

TArray<struct FTextureParameterValue>& USPAST_BatchEditorAssetsTool::DisableMarerialInstanceEditorOnlyTextureParameters(UMaterialInstance* instance, TArray<FName> paramterNames) {
	for (auto TextureInfo : instance->TextureParameterValues) {
		if (paramterNames.Contains(TextureInfo.ParameterInfo.Name)) {
			instance->TextureParameterValues.Remove(TextureInfo);
		}
	}

	return instance->TextureParameterValues;
}

void USPAST_BatchEditorAssetsTool::ClearMarerialInstanceAllEditorOnlyTextureParameters(UMaterialInstance* instance) {
	instance->TextureParameterValues.Empty();
};


TArray<struct FScalarParameterValue>& USPAST_BatchEditorAssetsTool::EnableMarerialInstanceEditorOnlyScalarParameter(UMaterialInstance* instance, FName paramterName) {
	TArray< FMaterialParameterInfo > OutParameterInfo;
	TArray< FGuid > OutParameterGUID;
	instance->GetAllScalarParameterInfo(OutParameterInfo, OutParameterGUID);

	for (FScalarParameterValue value : instance->ScalarParameterValues) {
		if (value.ParameterInfo.Name == paramterName) {
			return instance->ScalarParameterValues;
		}
	}

	for (FMaterialParameterInfo parameterInfo : OutParameterInfo) {
		if (parameterInfo.Name == paramterName) {
			float scalarDefault;
			instance->GetScalarParameterDefaultValue(parameterInfo, scalarDefault);

			FScalarParameterValue sv = (FScalarParameterValue)parameterInfo;
			sv.ParameterValue = scalarDefault;

			instance->ScalarParameterValues.Emplace(sv);
		}
	}

	return instance->ScalarParameterValues;
};

TArray<struct FScalarParameterValue>& USPAST_BatchEditorAssetsTool::EnableMarerialInstanceEditorOnlyScalarParameters(UMaterialInstance* instance, TArray<FName> paramterNames) {
	TArray< FMaterialParameterInfo > OutParameterInfo;
	TArray< FGuid > OutParameterGUID;
	instance->GetAllScalarParameterInfo(OutParameterInfo, OutParameterGUID);

	for (FMaterialParameterInfo parameterInfo : OutParameterInfo) {
		if (paramterNames.Contains(parameterInfo.Name)) {

			bool bUnEnabled = false;

			for (FScalarParameterValue value : instance->ScalarParameterValues) {

				if (value.ParameterInfo.Name == parameterInfo.Name) {
					bUnEnabled = true;
				}
			}

			if (bUnEnabled) {
				float scalarDefault;
				instance->GetScalarParameterDefaultValue(parameterInfo, scalarDefault);

				FScalarParameterValue sv = (FScalarParameterValue)parameterInfo;
				sv.ParameterValue = scalarDefault;

				instance->ScalarParameterValues.Emplace(sv);
			}
		}
	}

	return instance->ScalarParameterValues;
};

TArray<struct FScalarParameterValue>& USPAST_BatchEditorAssetsTool::DisableMarerialInstanceEditorOnlyScalarParameter(UMaterialInstance* instance, FName paramterName) {
	for (auto ScalarInfo : instance->ScalarParameterValues) {
		if (ScalarInfo.ParameterInfo.Name == paramterName) {
			instance->ScalarParameterValues.Remove(ScalarInfo);
		}
	}

	return instance->ScalarParameterValues;
};

TArray<struct FScalarParameterValue>& USPAST_BatchEditorAssetsTool::DisableMarerialInstanceEditorOnlyScalarParameters(UMaterialInstance* instance, TArray<FName> paramterNames) {
	for (auto ScalarInfo : instance->ScalarParameterValues) {
		if (paramterNames.Contains(ScalarInfo.ParameterInfo.Name)) {
			instance->ScalarParameterValues.Remove(ScalarInfo);
		}
	}

	return instance->ScalarParameterValues;
};

void USPAST_BatchEditorAssetsTool::ClearMarerialInstanceAllEditorOnlyScalarParameters(UMaterialInstance* instance) {
	instance->ScalarParameterValues.Empty();
};

TArray<struct FVectorParameterValue>& USPAST_BatchEditorAssetsTool::EnableMarerialInstanceEditorOnlyVectorParameter(UMaterialInstance* instance, FName paramterName){
	TArray< FMaterialParameterInfo > OutParameterInfo;
	TArray< FGuid > OutParameterGUID;
	instance->GetAllVectorParameterInfo(OutParameterInfo, OutParameterGUID);

	for (FVectorParameterValue value : instance->VectorParameterValues) {
		if (value.ParameterInfo.Name == paramterName) {
			return instance->VectorParameterValues;
		}
	}

	for (FMaterialParameterInfo parameterInfo : OutParameterInfo) {
		if (parameterInfo.Name== paramterName) {
			FLinearColor vectorDefault;
			instance->GetVectorParameterDefaultValue(parameterInfo, vectorDefault);

			FVectorParameterValue vv = (FVectorParameterValue)parameterInfo;
			vv.ParameterValue = vectorDefault;

			instance->VectorParameterValues.Emplace(vv);
		}
	}

	return instance->VectorParameterValues;
};

TArray<struct FVectorParameterValue>& USPAST_BatchEditorAssetsTool::EnableMarerialInstanceEditorOnlyVectorParameters(UMaterialInstance* instance, TArray<FName> paramterNames){
	TArray< FMaterialParameterInfo > OutParameterInfo;
	TArray< FGuid > OutParameterGUID;
	instance->GetAllVectorParameterInfo(OutParameterInfo, OutParameterGUID);

	for (FMaterialParameterInfo parameterInfo : OutParameterInfo) {
		if (paramterNames.Contains(parameterInfo.Name)) {

			bool bUnEnabled = false;

			for (FVectorParameterValue value : instance->VectorParameterValues) {

				if (value.ParameterInfo.Name == parameterInfo.Name) {
					bUnEnabled = true;
				}
			}

			if (bUnEnabled) {
				FLinearColor vectorDefault;
				instance->GetVectorParameterDefaultValue(parameterInfo, vectorDefault);

				FVectorParameterValue vv = (FVectorParameterValue)parameterInfo;
				vv.ParameterValue = vectorDefault;

				instance->VectorParameterValues.Emplace(vv);
			}
		}
	}

	return instance->VectorParameterValues;
};

TArray<struct FVectorParameterValue>& USPAST_BatchEditorAssetsTool::DisableMarerialInstanceEditorOnlyVectorParameter(UMaterialInstance* instance, FName paramterName){
	for (auto VectorInfo : instance->VectorParameterValues) {
		if (VectorInfo.ParameterInfo.Name== paramterName) {
			instance->VectorParameterValues.Remove(VectorInfo);
		}
	}

	return instance->VectorParameterValues;
};

TArray<struct FVectorParameterValue>& USPAST_BatchEditorAssetsTool::DisableMarerialInstanceEditorOnlyVectorParameters(UMaterialInstance* instance, TArray<FName> paramterNames){
	for (auto VectorInfo : instance->VectorParameterValues) {
		if (paramterNames.Contains(VectorInfo.ParameterInfo.Name)) {
			instance->VectorParameterValues.Remove(VectorInfo);
		}
	}

	return instance->VectorParameterValues;
};

void USPAST_BatchEditorAssetsTool::ClearMarerialInstanceAllEditorOnlyVectorParameters(UMaterialInstance* instance) {
	instance->VectorParameterValues.Empty();
};

TMap<FMaterialParameterInfo, FMaterialParameterMetadata>& USPAST_BatchEditorAssetsTool::EnableMaterialInstanceEidtorOnlyParamter(UMaterialInstance* instance, FName parameterName, EMaterialParameterType parameterType) {
		
	TMap<FMaterialParameterInfo, FMaterialParameterMetadata> * OutParameterInfoMap = new TMap<FMaterialParameterInfo, FMaterialParameterMetadata>;
	instance->GetAllParametersOfType(parameterType, *OutParameterInfoMap);

	FMaterialInstanceParameterUpdateContext context(instance);

	for (auto & parameter : *OutParameterInfoMap) {
		if (parameter.Key.Name == parameterName) {
			parameter.Value.bOverride = 1;
			context.SetParameterValueEditorOnly(parameter.Key, parameter.Value);
		}
	}

	return * OutParameterInfoMap;
};