#include "ConvertString.h"

#include <Windows.h>

std::wstring ConvertString::ConvertToWstring(const std::string& str)
{
	int bufferSize = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);

	wchar_t* wstr = new wchar_t[bufferSize];

	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wstr, bufferSize);

	std::wstring ret(wstr, wstr + bufferSize - 1);

	delete[] wstr;
	return ret;
}

std::string ConvertString::ConvertToStirng(const std::wstring& str)
{
    int iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, str.c_str(), -1, (char*)NULL, 0, NULL, NULL);

    CHAR* cpMultiByte = new CHAR[iBufferSize];

    WideCharToMultiByte(CP_OEMCP, 0, str.c_str(), -1, cpMultiByte, iBufferSize, NULL, NULL);

    std::string ret(cpMultiByte, cpMultiByte + iBufferSize - 1);

    delete[] cpMultiByte;
    return ret;
}
