#include "ConvertString.h"

#include <Windows.h>

std::wstring ConvertString::ConvertToWstirng(const std::string& str)
{
	int bufferSize = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);

	wchar_t* wstr = new wchar_t[bufferSize];

	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wstr, bufferSize);

	std::wstring ret(wstr, wstr + bufferSize - 1);

	delete[] wstr;
	return ret;
}
