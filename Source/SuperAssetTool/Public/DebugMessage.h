#pragma once

#include "MessageStandard.h"

#define ColumWidth_1 20
#define ColumWidth_2 35

#define ColumnPatternContent "|{0:{1}}|{2:{3}}|"
#define ColumnPatternHeader "|{0:-<{1}}|{2:-^{3}}|"
#define ColumnPatternFooter "|{0:><{1}}|{2:=^{3}}|"
#define Column(colum_pattern,column1_content,column2_content) std::format(colum_pattern, column1_content, ColumWidth_1, column2_content, ColumWidth_2)

#define CHECK_RESULT_3State(judge_condition ,undefine,positive,negtive) judge_condition> 1 ? undefine : CheckSTDPrefix(AssetData)? positive : negtive


void SPAST_PrintLog_SCREEN(const FString & Message, AssetSTD::MessageType Type) {
	GEngine -> AddOnScreenDebugMessage(-1, 5, *(AssetSTD::MessageColor.Find(Type)), Message);
}

void SPAST_PrintLog_SCREEN(const FName& Message, AssetSTD::MessageType Type) {
	GEngine->AddOnScreenDebugMessage(-1, 5, *(AssetSTD::MessageColor.Find(Type)), Message.ToString());
}

void SPAST_PrintLog_OUTPUTLOG(const FString& Message) {
	UE_LOG(LogTemp, Display, TEXT("%s"), *Message);
}

