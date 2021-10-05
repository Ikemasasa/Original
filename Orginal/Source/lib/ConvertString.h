#pragma once
#include <string>

namespace ConvertString
{
	std::wstring ConvertToWstring(const std::string& str);
	std::string ConvertToStirng(const std::wstring& str);
}