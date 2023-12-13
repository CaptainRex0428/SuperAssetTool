#pragma once


namespace AssetSTD {

	enum MessageTarget {
		SCREEN,
		OUTPUTLOG,
		DIALOG
	};

	enum MessageLevel {
		SPAST_MSG_Error,
		SPAST_MSG_Warning,
		SPAST_MSG_Tips
	};

	TMap<MessageLevel,FColor> MessageColor{
		{SPAST_MSG_Error,FColor::Red},
		{SPAST_MSG_Warning,FColor::Yellow},
		{SPAST_MSG_Tips,FColor::Cyan}
	};

	TMap<MessageLevel,FString> MessageLevelToString{
		{SPAST_MSG_Error,"Error"},
		{SPAST_MSG_Warning,"Warning"},
		{SPAST_MSG_Tips,"Tips"}
	};
	
}