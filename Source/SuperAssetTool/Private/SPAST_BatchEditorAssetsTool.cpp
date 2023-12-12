
#include "SPAST_BatchEditorAssetsTool.h"
#include "DebugMessage.h"

// ------------------------------Public Functions------------------------------//
// ------------------------------(Call In Editor)------------------------------//

void USPAST_BatchEditorAssetsTool::DebugTool() {
	TArray<FAssetData> a = UEditorUtilityLibrary::GetSelectedAssetData();

	for (auto b : a) {
		DuplicateAsset(b.PackagePath.ToString(), b.GetObjectPathString(), 3, true);
	}
		
};

void USPAST_BatchEditorAssetsTool::CheckAssetsClass() {
	SPAST_CheckAssetsClass(UEditorUtilityLibrary::GetSelectedAssetData());
};


// ------------------------------Public Functions------------------------------//

void USPAST_BatchEditorAssetsTool::SPAST_CheckAssetsName(TArray<FAssetData> AssetsData) {
	SPAST_PrintLog_OUTPUTLOG("DebugAssetsName is under development.", AssetSTD::SPAST_MSG_Tips);
	SPAST_PrintLog_SCREEN((FString)"DebugAssetsName is under development.", AssetSTD::SPAST_MSG_Warning); 
};

void USPAST_BatchEditorAssetsTool::SPAST_CheckAssetsClass(TArray<FAssetData> AssetsData) {

	int count = 0;

	for (auto AssetData : AssetsData) {
		count++;

		// output log header
		FString Column_Header = TABLESTRING_2Column(TablePattern_2Column_Header, "Index", count).c_str();
		SPAST_PrintLog_OUTPUTLOG(Column_Header,AssetSTD::SPAST_MSG_Tips);

		CheckAssetClass(AssetData);
	}

	// output log footer
	FString Column_Footer = TABLESTRING_2Column(TablePattern_2Column_Footer, "Assets Count", count).c_str();
	SPAST_PrintLog_OUTPUTLOG(Column_Footer, AssetSTD::SPAST_MSG_Tips);
	
	// output screen count
	SPAST_PrintLog_SCREEN(FString::FromInt(count),AssetSTD::SPAST_MSG_Tips);

};

TArray<FAssetData> USPAST_BatchEditorAssetsTool::SPAST_DuplicateAssets(TArray<FAssetData> AssetsData, FString SubfolderName,
	bool UseSubfolder, bool OverwriteExists,
	int DuplicateNum)
{
	TArray<FAssetData> DuplicatedAssets;
	int Counter = 0;

	for (auto Asset : AssetsData) {
		auto AssetPathInfo = GetAssetPathInfo(Asset);
		
		if (UseSubfolder) {
			FString destinationDirectory = FPaths::Combine(std::get<1>(AssetPathInfo), SubfolderName);
			
			if (!UEditorAssetLibrary::DoesDirectoryExist(destinationDirectory)) {
				UEditorAssetLibrary::MakeDirectory(destinationDirectory);
			}

		}
		else {

		}
	}

	return DuplicatedAssets;
};

//------------------------------Private Functions------------------------------//

void USPAST_BatchEditorAssetsTool::CheckAssetClass(FAssetData AssetData) {
	
	std::string AssetNameResult = TCHAR_TO_UTF8(*AssetData.AssetName.ToString());
	std::string AssetClassResult = TCHAR_TO_UTF8(*AssetData.GetClass()->GetName());
	std::string AssetPrefixCheck = CHECK_RESULT_3State(CheckSTDPrefix(AssetData),"-","V","X");

	// output log
	SPAST_PrintLog_OUTPUTLOG(TABLESTRING_2Column(TablePattern_2Column_Content, "Asset Name", AssetNameResult).c_str() ,AssetSTD::SPAST_MSG_Tips);
	SPAST_PrintLog_OUTPUTLOG(TABLESTRING_2Column(TablePattern_2Column_Content, "Asset Class", AssetClassResult).c_str(), AssetSTD::SPAST_MSG_Tips);
	SPAST_PrintLog_OUTPUTLOG(TABLESTRING_2Column(TablePattern_2Column_Content, "Asset Prefix Check",AssetPrefixCheck).c_str(), AssetSTD::SPAST_MSG_Tips);

	// output screen
	SPAST_PrintLog_SCREEN(std::format("|{:^3}|{:^25}|{:^25}|",AssetPrefixCheck,AssetClassResult,AssetNameResult).c_str(), AssetSTD::SPAST_MSG_Tips);
}

 const FString USPAST_BatchEditorAssetsTool::GetPrefix(FAssetData& AssetData) {
	FString assetname = AssetData.AssetName.ToString();

	std::string assetname_str = TCHAR_TO_UTF8(*assetname);
	int assetname_len = assetname_str.length();

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
};

const FString USPAST_BatchEditorAssetsTool::GetSTDPrefix(FAssetData& AssetData) {
	auto STD_Prefix = AssetSTD::EditorPrefix.Find(AssetData.GetClass());
	return STD_Prefix ? * STD_Prefix : NULL_FSTRING;
};

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

};

void USPAST_BatchEditorAssetsTool::FixSTDPrefix(FAssetData& AssetData) {

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

TArray<FAssetData> USPAST_BatchEditorAssetsTool::DuplicateAsset(FString DestinationDirectory, FString SourceAssetPath, int DuplicateNum, bool overwrite) 
{
	TArray<FAssetData> DuplicatedAssets;

	FString assetName = FPaths::GetBaseFilename(SourceAssetPath);
	
	int first_subfix = overwrite ? 1:MaxNumSubfix(DestinationDirectory, assetName) + 1;
	
	for (int count = 0; count < DuplicateNum; count++) {
		FString DestinationAssetPath = FPaths::Combine(DestinationDirectory, assetName + "_" + FormateInt2String((first_subfix + count), 2).c_str());

		if (UEditorAssetLibrary::DoesAssetExist(DestinationAssetPath) && overwrite) {
			UEditorAssetLibrary::DeleteLoadedAsset(UEditorAssetLibrary::LoadAsset(DestinationAssetPath));
		}

		FAssetData assetDuplicated = UEditorAssetLibrary::DuplicateAsset(SourceAssetPath,DestinationAssetPath);
		UEditorAssetLibrary::SaveAsset(assetDuplicated.GetObjectPathString());
		DuplicatedAssets.Add(assetDuplicated);
	}



	return DuplicatedAssets;
};

int USPAST_BatchEditorAssetsTool::MaxNumSubfix(FString DirectoryPath, FString AssetName) {
	
	int maxSubfix = 0;

	TArray<FString> AssetsList = UEditorAssetLibrary::ListAssets(DirectoryPath, false);

	for (auto asset : AssetsList) {

		FString basename = FPaths::GetBaseFilename(asset);

		if (basename.StartsWith(AssetName) && basename != AssetName) 
		{
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