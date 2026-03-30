#pragma once

#include <array>

namespace mg
{
	template<size_t N1, size_t N2>

	constexpr static std::array<char, N1 + N2> concat(const char *str1, const char *str2)
	{
		std::array<char, N1 + N2> result{};
		for (size_t i = 0; i < N1; ++i)
			result[i] = str1[i];
		for (size_t i = 0; i < N2; ++i)
			result[N1 + i] = str2[i];
		return result;
	}
} // namespace mg
