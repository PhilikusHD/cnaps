#pragma once
#include <string>
#include <unordered_set>

namespace Ravi
{
	class Tokenizer
	{
	public:
		Tokenizer();
		void AddDelimiter(char delimiter);
		void RemoveDelimiter(char delimiter);
		std::vector<std::string> Tokenize(const std::string& input);

	private:
		std::unordered_set<char> m_Delimiters;

		bool IsDelimiter(char c) const;
		std::string Normalize(const std::string& token) const;
	};
}