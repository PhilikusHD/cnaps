#include "nlp/Tokenizer.h"
#include <algorithm>
#include <unordered_map>
#include <regex>

namespace Ravi
{
	Tokenizer::Tokenizer()
		: m_Delimiters{ ' ', '.', ',', '!', '?', ';', ':', '-', '\n', '\t' }
	{
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

	std::string Tokenizer::Normalize(const std::string& token)
	{
		std::string normalized = token;

		// Remove accents
		normalized = RemoveAccent(normalized);

		// fuck 'em special characters
		normalized.erase(std::remove_if(normalized.begin(), normalized.end(), [](unsigned char c) {
			return !std::isalnum(c) && c != '\'';
			}), normalized.end());

		// turn into lowercase
		std::transform(normalized.begin(), normalized.end(), normalized.begin(), [](unsigned char c) -> int { return std::tolower(c); });

		// Handle contractions
		normalized = ExpandContractions(normalized);

		return normalized;
	}

	std::string Tokenizer::RemoveAccent(const std::string& input)
	{
		std::unordered_map<char, char> accentMap = {
			{'á', 'a'}, {'à', 'a'}, {'ä', 'a'}, {'â', 'a'},
			{'é', 'e'}, {'è', 'e'}, {'ë', 'e'}, {'ê', 'e'},
			{'í', 'i'}, {'ì', 'i'}, {'ï', 'i'}, {'î', 'i'},
			{'ó', 'o'}, {'ò', 'o'}, {'ö', 'o'}, {'ô', 'o'},
			{'ú', 'u'}, {'ù', 'u'}, {'ü', 'u'}, {'û', 'u'},
			{'ñ', 'n'}, {'ç', 'c'}, {'ý', 'y'}, {'ÿ', 'y'},
			{'š', 's'}, {'ž', 'z'}
		};

		std::string result = input;
		std::transform(result.begin(), result.end(), result.begin(), [&](char c) { return accentMap.count(c) ? accentMap[c] : c; });
		return result;
	}

	std::string Tokenizer::ExpandContractions(const std::string& input)
	{
		const std::unordered_map<std::string, std::string> contractionsMap = {
			{"aren't", "are not"}, {"can't", "cannot"}, {"couldn't", "could not"},
			{"didn't", "did not"}, {"doesn't", "does not"}, {"don't", "do not"},
			{"hadn't", "had not"}, {"hasn't", "has not"}, {"haven't", "have not"},
			{"he's", "he is"}, {"here's", "here is"}, {"i'd", "i would"},
			{"i'll", "i will"}, {"i'm", "i am"}, {"i've", "i have"},
			{"isn't", "is not"}, {"it's", "it is"}, {"let's", "let us"},
			{"she's", "she is"}, {"shouldn't", "should not"}, {"that's", "that is"},
			{"there's", "there is"}, {"they'd", "they would"}, {"they'll", "they will"},
			{"they're", "they are"}, {"they've", "they have"}, {"we'd", "we would"},
			{"we're", "we are"}, {"we've", "we have"}, {"weren't", "were not"},
			{"what's", "what is"}, {"where's", "where is"}, {"who's", "who is"},
			{"won't", "will not"}, {"wouldn't", "would not"}, {"you'd", "you would"},
			{"you'll", "you will"}, {"you're", "you are"}, {"you've", "you have"},
			{"y'all", "you all"}
		};

		std::string result = input;

		for (const auto& contraction : contractionsMap)
		{
			// Regex my beloved, replace the whole word
			std::regex contractionRegex("\\b" + contraction.first + "\\b");
			result = std::regex_replace(result, contractionRegex, contraction.second);
		}

		return result;
	}
}