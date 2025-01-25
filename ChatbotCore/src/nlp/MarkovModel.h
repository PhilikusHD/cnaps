#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <map>
#include "Tokenizer.h"

namespace Ravi
{
	class MarkovModel
	{
	public:
		MarkovModel(size_t ngrams);

		void Train(const std::vector<std::string>& userTokens, const std::vector<std::string>& botTokens);
		std::string GenerateResponse(const std::string& userInput);
	private:
		size_t m_NGrams;
		Tokenizer m_Tokenizer;
		std::map<std::vector<std::string>, std::map<std::string, int>> m_BotMarkov;

		void TrainOnTokens(const std::vector<std::string>& tokens, std::map<std::vector<std::string>, std::map<std::string, int>>& markovModel) const;
		std::vector<std::string> GenerateFromMarkov(const std::vector<std::string>& userInput, const std::map<std::vector<std::string>, std::map<std::string, int>>& markovModel);
		std::string ChooseNextTokenFromProbabilities(const std::map<std::string, int>& transitions);
		std::string ChooseNextToken(const std::map<std::string, int>& transitions);
	};
}