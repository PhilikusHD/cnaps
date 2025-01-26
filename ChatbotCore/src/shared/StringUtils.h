#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace StringUtils
{
	// Trim whitespace from the beginning and end of a string
	std::string Trim(const std::string& str)
	{
		size_t start = str.find_first_not_of(" \t\n\r");
		size_t end = str.find_last_not_of(" \t\n\r");
		return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
	}

	// Convert string to lowercase
	std::string ToLower(const std::string& str)
	{
		std::string result = str;
		std::transform(result.begin(), result.end(), result.begin(), ::tolower);
		return result;
	}

	// Split a string by a delimiter
	std::vector<std::string> Split(const std::string& str, char delimiter)
	{
		std::vector<std::string> tokens;
		std::stringstream ss(str);
		std::string token;

		while (std::getline(ss, token, delimiter))
		{
			tokens.push_back(token);
		}

		return tokens;
	}

	// Check if a string contains a substring (case-insensitive)
	bool Contains(const std::string& str, const std::string& substring)
	{
		auto it = std::search(
			str.begin(), str.end(),
			substring.begin(), substring.end(),
			[](char c1, char c2) { return std::tolower(c1) == std::tolower(c2); }
		);
		return it != str.end();
	}
}
