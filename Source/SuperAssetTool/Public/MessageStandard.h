#pragma once


namespace AssetSTD {

	enum MessageType {
		SPAST_MSG_Error,
		SPAST_MSG_Warning,
		SPAST_MSG_Tips
	};

	TMap<MessageType, FColor> MessageColor{
		{SPAST_MSG_Error,FColor::Red},
		{SPAST_MSG_Warning,FColor::Yellow},
		{SPAST_MSG_Tips,FColor::Cyan}
	};

}