#include "stdafx.h"
#include "Utility.h"

float Utility::getTime()
{
	// todo: 여기 채워야 함

	return 0.0f;
}

std::wstring Utility::strtowstr(const std::string& str)
{
	if (str.empty()) return std::wstring();

	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstr[0], size_needed);
	return wstr;
}