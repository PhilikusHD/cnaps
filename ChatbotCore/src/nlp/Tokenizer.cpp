#include "nlp/Tokenizer.h"
#include <algorithm>

namespace Ravi
{
	Tokenizer::Tokenizer()
		: m_Delimiters{ ' ', '.', ',', '!', '?', ';', ':', '\n', '\t' }
	{
	}

	void Tokenizer::AddDelimiter(char delimiter)
	{
		m_Delimiters.insert(delimiter);
	}

	void Tokenizer::RemoveDelimiter(char delimiter)
	{
		m_Delimiters.erase(delimiter);
	}

	std::vector<std::string> Tokenizer::Tokenize(const std::string& input)
	{
		std::vector<std::string> tokens;
		std::string currentToken;

		for (char c : input)
		{
			if (IsDelimiter(c))
			{
				if (!currentToken.empty())
				{
					tokens.emplace_back(Normalize(currentToken));
					currentToken.clear();
				}
			}
			else
			{
				currentToken += c;
			}
		}

		if (!currentToken.empty())
		{
			tokens.emplace_back(Normalize(currentToken));
		}

		return tokens;
	}

	bool Tokenizer::IsDelimiter(char c) const
	{
		return m_Delimiters.find(c) != m_Delimiters.end();
	}

	std::string Tokenizer::Normalize(const std::string& token) const
	{
		std::string normalized = token;
		std::transform(normalized.begin(), normalized.end(), normalized.begin(), [](unsigned char c) -> int { return std::tolower(c); });
		return normalized;
	}
}