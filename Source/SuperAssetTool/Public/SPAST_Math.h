#pragma once

#include <iostream>
#include <string>
#include <format>


struct sResult_String2Int {
	bool success;
	int num;
};

sResult_String2Int String2Int(std::string str) {

	sResult_String2Int result;

	try
	{
		result.num = std::stoi(std::string(str));
		result.success = true;
	}
	catch (const std::exception&)
	{
		result.num = 0;
		result.success = false;
	}

	return result;
}

std::string FormatInt2String(int num, int formatNum = 2) {
	return std::format("{0:0>{1}}",num,formatNum);
}

const char * LStrip(std::string * originStringPtr) {
	const char * ptr =  (*originStringPtr).c_str();

	int substring_first = 0;
	
	for (int index = 0; index < (*originStringPtr).length(); index++) {
		if (*(ptr + index) != 32)
		{
			substring_first = index;
			break;
		}
	} 

	*originStringPtr = (*originStringPtr).substr(substring_first, (*originStringPtr).length());

	return (*originStringPtr).c_str();
}

const char * RStrip(std::string* originStringPtr) {
	const char* ptr = (*originStringPtr).c_str();

	int substring_last = (*originStringPtr).length();

	for (int index = substring_last; index > 0; index--) {
		if (*(ptr + index-1) != 32)
		{
			substring_last = index;
			break;
		}
	}

	*originStringPtr = (*originStringPtr).substr(0,substring_last);

	return (*originStringPtr).c_str();
}

const char* Strip(std::string* originStringPtr) {
	LStrip(originStringPtr);
	RStrip(originStringPtr);
	return (*originStringPtr).c_str();
}