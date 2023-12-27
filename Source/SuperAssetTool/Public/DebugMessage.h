#pragma once

#include "MessageStandard.h"
#include "Misc/MessageDialog.h"

#include <format>


#define TablePattern_2Column_Content "|{0:{1}}|{2:{3}}|"
#define TablePattern_2Column_Header "|{0:-<{1}}|{2:-^{3}}|"
#define TablePattern_2Column_Footer "|{0:><{1}}|{2:=^{3}}|"

#define TablePattern_3Column_Content "|{0:{1}}|{2:{3}}|{4:{5}}|"
#define TablePattern_3Column_Header "|{0:-<{1}}|{2:-^{3}}|{4:-^{5}}|"
#define TablePattern_3Column_Footer "|{0:><{1}}|{2:=^{3}}|{4:=^{5}}|"

#define ColumWidth_1 20
#define ColumWidth_2 35
#define ColumWidth_3 35

#define TABLESTRING_2Column(colum_pattern,column1_content,column2_content) std::format(colum_pattern, column1_content, ColumWidth_1, column2_content, ColumWidth_2)
#define TABLESTRING_3Column(colum_pattern,column1_content,column2_content,column3_content) std::format(colum_pattern, column1_content, ColumWidth_1, column2_content, ColumWidth_2, column3_content,ColumWidth_3)

#define MSGTARGET_SCREEN {MsgSTD::SCREEN}
#define MSGTARGET_OUTPUTLOG {MsgSTD::OUTPUTLOG}
#define MSGTARGET_DIALOG {MsgSTD::DIALOG}
#define MSGTARGET_SCREEN_OUTPUTLOG {MsgSTD::SCREEN,MsgSTD::OUTPUTLOG}
#define MSGTARGET_SCREEN_DIALOG {MsgSTD::SCREEN,MsgSTD::DIALOG}
#define MSGTARGET_OUTPUTLOG_DIALOG {MsgSTD::DIALOG,MsgSTD::OUTPUTLOG}
#define MSGTARGET_SCREEN_OUTPUTLOG_DIALOG {MsgSTD::SCREEN,MsgSTD::DIALOG,MsgSTD::OUTPUTLOG}

void SPAST_PrintLog_SCREEN(const FString& Message, MsgSTD::MessageLevel Type) {
	GEngine->AddOnScreenDebugMessage(-1, 5, *(MsgSTD::MessageColor.Find(Type)), Message);
}

void SPAST_PrintLog_OUTPUTLOG(const FString& Message, MsgSTD::MessageLevel Type) {
	if(Type == MsgSTD::SPAST_MSG_Error)
		UE_LOG(LogTemp, Error, TEXT("%s"), *Message)
	else if (Type == MsgSTD::SPAST_MSG_Warning)
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Message)
	else
		UE_LOG(LogTemp, Log, TEXT("%s"), *Message)
}

void SPAST_Print(const FString& Message,
	TArray<MsgSTD::MessageTarget> Target = MSGTARGET_SCREEN_OUTPUTLOG,
	MsgSTD::MessageLevel Type = MsgSTD::SPAST_MSG_Tips,
	const FString& Title = "Message",
	EAppMsgType::Type ButtonType = EAppMsgType::Ok,
	EAppReturnType::Type * DialogReturn = nullptr)
{
	if (Target.Contains(MsgSTD::SCREEN))
		SPAST_PrintLog_SCREEN(Message, Type);

	if (Target.Contains(MsgSTD::OUTPUTLOG))
		SPAST_PrintLog_OUTPUTLOG(Message, Type);

	if (Target.Contains(MsgSTD::DIALOG)) {
		FString TitleResult = FString::Format(TEXT("[{0}]{1}"), { *MsgSTD::MessageLevelToString.Find(Type), Title});
		auto DialogResult = FMessageDialog::Open(ButtonType, FText::FromString(Message), FText::FromString(TitleResult));
		if (DialogReturn) {
			* DialogReturn = DialogResult;
		}
	}
		
}

