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
