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

std::string FormateInt2String(int num, int formatNum = 2) {
	return std::format("{0:0>{1}}",num,formatNum);
}