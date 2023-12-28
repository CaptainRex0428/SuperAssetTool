#pragma once


namespace MsgSTD {

	enum eMessageTarget {
		SCREEN,
		OUTPUTLOG,
		DIALOG
	};

	enum eMessageLevel {
		SPAST_MSG_Error,
		SPAST_MSG_Warning,
		SPAST_MSG_Tips
	};

	TMap<eMessageLevel,FColor> mMessageColor{
		{SPAST_MSG_Error,FColor::Red},
		{SPAST_MSG_Warning,FColor::Yellow},
		{SPAST_MSG_Tips,FColor::Cyan}
	};

	TMap<eMessageLevel,FString> mMessageLevelToString{
		{SPAST_MSG_Error,"Error"},
		{SPAST_MSG_Warning,"Warning"},
		{SPAST_MSG_Tips,"Tips"}
	};
	
}