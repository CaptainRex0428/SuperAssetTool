#pragma once

#include "MessageStandard.h"

void SPAST_PrintLog_SCREEN(const FString & Message, AssetSTD::MessageType Type) {
	GEngine -> AddOnScreenDebugMessage(-1, 5, *(AssetSTD::MessageColor.Find(Type)), Message);
}

void SPAST_PrintLog_SCREEN(const FName& Message, AssetSTD::MessageType Type) {
	GEngine->AddOnScreenDebugMessage(-1, 5, *(AssetSTD::MessageColor.Find(Type)), Message.ToString());
}

void SPAST_PrintLog_OUTPUTLOG(const FString& Message) {
	UE_LOG(LogTemp, Display, TEXT("%s"), *Message);
}

