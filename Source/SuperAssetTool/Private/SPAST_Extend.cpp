
#include "SPAST_Extend.h"

#include <string>

#include <regex>
#include <format>


bool USPAST_Extend::NameContains(FName name, std::string str) 
{
	const char * name_char = TCHAR_TO_UTF8(* name.ToString());
	int name_len = name.ToString().Len();
	
	const char * str_char = str.c_str();
	int str_len = str.length();

	std::string patternStr = ".*{" + str + "}.*";

	std::regex pattern(patternStr);
	
	return std::regex_match(name_char,pattern);
};