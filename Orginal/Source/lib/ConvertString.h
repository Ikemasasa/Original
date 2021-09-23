#pragma once
#include <string>

namespace ConvertString
{
	std::wstring ConvertToWstirng(const std::string& str);
	std::string ConvertToStirng(const std::wstring& str);
}