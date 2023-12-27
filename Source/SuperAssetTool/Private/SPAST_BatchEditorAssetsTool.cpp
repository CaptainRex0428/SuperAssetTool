#include "SPAST_BatchEditorAssetsTool.h"

// ------------------------------Public Functions------------------------------//
// ------------------------------(Call In Editor)------------------------------//

void USPAST_BatchEditorAssetsTool::dbTool() {	
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
			SPAST_Print(FString::FromInt(info->Resolution));
			SPAST_Print(info->iSRGB ? "sRGB" : "No sRGB");
			SPAST_Print(* AssetSTD::mCompressionSettings2FSrting.Find(info->CompressionSettings));
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

const AssetSTD::sTextureStandardInfo * USPAST_BatchEditorAssetsTool::GetStandardTextureInfo(FString& subfix, AssetSTD::eTextureCategory texturestandard)
{
	return AssetSTD::mTextureSubfix.Find(texturestandard)->Find(subfix);
};

const AssetSTD::sTextureStandardInfo * USPAST_BatchEditorAssetsTool::GetTexture2DInfo(FAssetData assetdata)
{
	if (!assetdata.IsInstanceOf(UTexture2D::StaticClass()))
		return nullptr;

	AssetSTD::sTextureStandardInfo * result = new AssetSTD::sTextureStandardInfo;
	
	UTexture2D* assetObj = (UTexture2D*)UEditorAssetLibrary::LoadAsset(assetdata.GetObjectPathString());

	result->CompressionSettings = assetObj->CompressionSettings;
	result->Resolution = std::max(assetObj->GetImportedSize().X, assetObj->GetImportedSize().Y);
	result->iSRGB = assetObj->SRGB;

	return result;
};

AssetSTD::eTextureCategory * USPAST_BatchEditorAssetsTool::GetTextureCategoty(FAssetData assetData)
{
	AssetSTD::eTextureCategory * result = new AssetSTD::eTextureCategory;

	if (assetData.IsInstanceOf(UTexture::StaticClass()))
		return nullptr;

	if (USPAST_Extend::NameContains(assetData.AssetName, "_Hair"))
		*result = AssetSTD::Hair;
	else
		*result = AssetSTD::Base;

	return result;
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