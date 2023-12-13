#pragma once

#include <iostream>
#include <string>
#include <format>


struct Result_String2Int {
	bool success;
	int num;
};

Result_String2Int String2Int(std::string str) {

	Result_String2Int result;

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

std::string LStrip(std::string * originString) {
	const char * ptr =  (*originString).c_str();

	int substring_first = 0;
	
	for (int index = 0; index < (*originString).length(); index++) {
		if (*(ptr + index) != 32)
		{
			substring_first = index;
		};
	} 

	* originString = (*originString).substr(substring_first, (*originString->end()));

	return * originString;
}