#include "stdafx.h"
#include "Utility.h"

template<class T>
inline T Utility::GetRandom(T min, T max)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	if constexpr (std::is_integral_v<T>)
	{
		std::uniform_int_distribution<T> uid(min, max);
		return uid(gen);
	}
	else
	{
		std::uniform_real_distribution<T> uid(min, max);
		return uid(gen);
	}
}

std::wstring Utility::strtowstr(const std::string& str)
{
	if (str.empty()) return std::wstring();

	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstr[0], size_needed);
	return wstr;
}