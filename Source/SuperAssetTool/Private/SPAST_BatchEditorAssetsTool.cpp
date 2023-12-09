// Fill out your copyright notice in the Description page of Project Settings.


#include "SPAST_BatchEditorAssetsTool.h"
#include "DebugMessage.h"


void USPAST_BatchEditorAssetsTool::DebugTool() {

};

void USPAST_BatchEditorAssetsTool::CheckAssetsName(TArray<FAssetData> AssetsData) {
	SPAST_PrintLog_OUTPUTLOG("DebugAssetsName is under development.");
	SPAST_PrintLog_SCREEN((FString)"DebugAssetsName is under development.", AssetSTD::SPAST_MSG_Warning);
};

void USPAST_BatchEditorAssetsTool::CheckAssetsClass(TArray<FAssetData> AssetsData) {

	int count = 0;

	for (auto AssetData : AssetsData) {
		count++;

		FString Column_Header = Column(ColumnPatternHeader, "Index", count).c_str();
		SPAST_PrintLog_OUTPUTLOG(Column_Header);

		CheckAssetClass(AssetData);
	}

	FString Column_Footer = Column(ColumnPatternFooter, "Assets Count", count).c_str();
	SPAST_PrintLog_OUTPUTLOG(Column_Footer);
	SPAST_PrintLog_SCREEN(FString::FromInt(count),AssetSTD::SPAST_MSG_Tips);

};

void USPAST_BatchEditorAssetsTool::CheckAssetClass(FAssetData AssetData) {
	
	std::string AssetNameResult = TCHAR_TO_UTF8(*AssetData.AssetName.ToString());
	std::string AssetClassResult = TCHAR_TO_UTF8(*AssetData.GetClass()->GetName());
	std::string AssetPrefixCheck = CHECKPREFIX("-","V","X");


	// output log

	SPAST_PrintLog_OUTPUTLOG(Column(ColumnPatternContent, "Asset Name", AssetNameResult).c_str());
	SPAST_PrintLog_OUTPUTLOG(Column(ColumnPatternContent, "Asset Class", AssetClassResult).c_str());
	SPAST_PrintLog_OUTPUTLOG(Column(ColumnPatternContent, "Asset Prefix Check",AssetPrefixCheck).c_str());

	SPAST_PrintLog_SCREEN((FString)std::format("|{:^3}|{:^25}|{:^25}|",AssetPrefixCheck,AssetClassResult,AssetNameResult).c_str(), AssetSTD::SPAST_MSG_Tips);
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
