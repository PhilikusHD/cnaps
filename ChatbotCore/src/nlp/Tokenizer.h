#pragma once
#include <string>
#include <unordered_set>

namespace Ravi
{
	class Tokenizer
	{
	public:
		Tokenizer();
		std::vector<std::string> Tokenize(const std::string& input);

	private:
		std::unordered_set<char> m_Delimiters;

		bool IsDelimiter(char c) const;
		std::string Normalize(const std::string& token);
		std::string RemoveAccent(const std::string& input);
		std::string ExpandContractions(const std::string& input);
	};
}