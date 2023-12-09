// Fill out your copyright notice in the Description page of Project Settings.


#include "SPAST_BatchEditorAssetsTool.h"
#include "DebugMessage.h"

#include <iostream>
#include <string>
#include <regex>
#include <format>

#define ColumWidth_1 20
#define ColumWidth_2 35

#define ColumnPatternContent "|{0:{1}}|{2:{3}}|"
#define ColumnPatternHeader "|{0:-<{1}}|{2:-^{3}}|"
#define ColumnPatternFooter "|{0:><{1}}|{2:=^{3}}|"
#define Column(colum_pattern,column1_content,column2_content) std::format(colum_pattern, column1_content, ColumWidth_1, column2_content, ColumWidth_2)

void USPAST_BatchEditorAssetsTool::DebugTool() {

};

void USPAST_BatchEditorAssetsTool::DebugAssetsName() {

	for (auto AssetData : UEditorUtilityLibrary::GetSelectedAssetData()) {
		SPAST_PrintLog_SCREEN(GetPrefix(AssetData), AssetSTD::SPAST_MSG_Tips);
		SPAST_PrintLog_OUTPUTLOG(GetPrefix(AssetData));
	}
};

void USPAST_BatchEditorAssetsTool::DebugAssetsClass(TArray<FAssetData> AssetsData) {

	int count = 0;

	for (auto AssetData : AssetsData) {
		count++;

		FString Column_Header = Column(ColumnPatternHeader, "Index", count).c_str();
		SPAST_PrintLog_OUTPUTLOG(Column_Header);

		DebugAssetClass(AssetData);
	}

	FString Column_Footer = Column(ColumnPatternFooter, "Assets Count", count).c_str();
	SPAST_PrintLog_OUTPUTLOG(Column_Footer);
	SPAST_PrintLog_SCREEN(FString::FromInt(count),AssetSTD::SPAST_MSG_Tips);

};

void USPAST_BatchEditorAssetsTool::DebugAssetClass(FAssetData AssetData) {
	
	std::string AssetNameResult = TCHAR_TO_UTF8(*AssetData.AssetName.ToString());
	std::string AssetClassResult = TCHAR_TO_UTF8(*AssetData.GetClass()->GetName());
	std::string AssetPrefixCheck = (CheckSTDPrefix(AssetData)) ? "V" : "X";


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

bool USPAST_BatchEditorAssetsTool::CheckSTDPrefix(FAssetData& AssetData) {
	return GetSTDPrefix(AssetData) == GetPrefix(AssetData);
};

void USPAST_BatchEditorAssetsTool::FixSTDPrefix(FAssetData& AssetData) {

};
