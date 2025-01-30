#pragma once

class Utility
{
public:
	template <class T>
	static T GetRandom(T min, T max);

    std::wstring strtowstr(const std::string& str);
};
