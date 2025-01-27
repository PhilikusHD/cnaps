#pragma once
#include <string>
#include <vector>
#include <regex>
#include <istream>

#include "shared/StringUtils.h"
#include "shared/Logger.h"

namespace Preprocessor
{
	static std::string RemovePunctuation(const std::string& str)
	{
		return std::regex_replace(str, std::regex(R"([^\w\s])"), "");
	}

	static std::vector<std::string> Tokenize(const std::string& str)
	{
		std::istringstream iss(str);
		std::vector<std::string> tokens;
		std::string token;
		while (iss >> token)
		{
			tokens.emplace_back(token);
		}

		return tokens;
	}

	static std::string StemWords(const std::vector<std::string>& tokens)
	{
		std::string stemmedInput;
		for (const auto& word : tokens)
		{
			std::string stemmed = word;

			// Naive stemming example (strip common suffixes)
			if (stemmed.size() > 4)
			{
				if (stemmed.substr(stemmed.size() - 3) == "ing")
				{
					stemmed = stemmed.substr(0, stemmed.size() - 3);
				}
				else if (stemmed.substr(stemmed.size() - 2) == "ed")
				{
					stemmed = stemmed.substr(0, stemmed.size() - 2);
				}
			}

			if (!stemmedInput.empty()) stemmedInput += " ";
			stemmedInput += stemmed;
		}
		return stemmedInput;
	}

	static void PreprocessInput(std::string& input)
	{
		input = StringUtils::ToLower(input);
		input = RemovePunctuation(input);
		input = StringUtils::Trim(input);

		std::vector<std::string> tokens = Tokenize(input);

		// Spell check mabe?

		input = StemWords(tokens);
	}
}