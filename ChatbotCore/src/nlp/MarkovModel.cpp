#include "MarkovModel.h"
#include <sstream>
#include <random>
#include <iostream>
#include <thread>

namespace Ravi
{
	namespace Utils
	{
		static std::string JoinTokens(const std::vector<std::string>& tokens)
		{
			if (tokens.empty()) return ""; // Return empty string if the vector is empty

			std::stringstream ss;
			ss << tokens[0];  // Add the first token directly

			for (size_t i = 1; i < tokens.size(); ++i)
			{
				ss << " " << tokens[i];  // Add space only between tokens, starting from the second one
			}

			return ss.str();
		}
	}

	MarkovModel::MarkovModel(size_t ngrams)
		: m_NGrams(ngrams)
	{
	}

	// Purely for internal training purposes. If you use this in prod, I will fucking explode.
	void MarkovModel::Train(const std::vector<std::string>& userTokens, const std::vector<std::string>& botTokens)
	{
		TrainOnTokens(botTokens, m_BotMarkov);
	}

	std::string MarkovModel::GenerateResponse(const std::string& userInput)
	{
		std::vector<std::string> userTokens = m_Tokenizer.Tokenize(userInput);
		std::vector<std::string> response = GenerateFromMarkov(userTokens, m_BotMarkov);
		return Utils::JoinTokens(response);
	}

	void MarkovModel::TrainOnTokens(const std::vector<std::string>& tokens, std::map<std::vector<std::string>, std::map<std::string, float>>& markovModel) const
	{
		if (tokens.size() >= m_NGrams + 1)
		{
			for (size_t i = 0; i < tokens.size() - m_NGrams; ++i)
			{
				std::vector<std::string> state(tokens.begin() + i, tokens.begin() + i + m_NGrams);
				std::string nextToken = tokens[i + m_NGrams];

				markovModel[state].insert_or_assign(nextToken, markovModel[state][nextToken] + 1);
			}
		}
		else
		{
			std::cout << "Not enough tokens to build the model." << std::endl;
		}

		// Normalize the transitions to probabilities
		for (auto& state : markovModel)
		{
			int totalFrequency = 0;
			// Calculate the total frequency for normalization
			for (auto& transition : state.second)
			{
				totalFrequency += transition.second;
			}

			// Normalize to probabilities
			if (totalFrequency > 0)
			{
				for (auto& transition : state.second)
				{
					transition.second = static_cast<float>(transition.second) / totalFrequency;
				}
			}
		}
	}

	std::vector<std::string> MarkovModel::GenerateFromMarkov(
		const std::vector<std::string>& userInput,
		const std::map<std::vector<std::string>, std::map<std::string, float>>& markovModel)
	{
		std::vector<std::string> response;

		size_t contextSize = std::min(userInput.size(), m_NGrams);
		std::vector<std::string> state(userInput.end() - contextSize, userInput.end());

		for (size_t i = 0; i < 25; ++i)
		{
			auto it = markovModel.lower_bound(state);
			if (it == markovModel.end() || it->second.empty())
			{
				break; // Exit if no valid transition is found
			}

			auto& transitions = it->second;
			std::string nextToken = ChooseNextTokenFromProbabilities(transitions);

			// Enforce grammar rules:
			if (i == 0)
			{
				// Avoid starting with filler words like "the", "and"
				if (nextToken == "the" || nextToken == "and" || nextToken == "but")
				{
					continue;
				}

				// Make sure we start with a subject or verb
				if (IsSubjectOrVerb(nextToken) == false)
				{
					continue;
				}
			}

			// Prevent consecutive repetitions
			if (!response.empty() && nextToken == response.back())
			{
				continue;
			}

			// Enforce minimal sentence structure:
			if (i == 0 && !response.empty() && (response.back() == "to" || response.back() == "and"))
			{
				continue; // Avoid starting new sentences with prepositions
			}

			// Ensure sentence ends with punctuation
			if (i == 24)
			{
				if (nextToken != "." && nextToken != "!" && nextToken != "?")
				{
					nextToken += "."; // Add period at the end if necessary
				}
			}

			response.push_back(nextToken);

			// Update the state for the next token generation
			state.push_back(nextToken);
			state.erase(state.begin()); // Maintain fixed size context window
		}

		return response;
	}



	std::string MarkovModel::ChooseNextTokenFromProbabilities(
		const std::map<std::string, float>& transitions)
	{
		// Compute total probability (sum of weights, which are now probabilities)
		float totalProbability = 0.0f;
		for (const auto& transition : transitions)
		{
			if (transition.second < 0)
			{
				std::cout << "Warning: Negative probability detected for token " << transition.first << std::endl;
			}
		}
		// Select based on weighted probability
		std::uniform_real_distribution<float> dist(0.0f, totalProbability);
		std::random_device rd;
		std::mt19937 gen(rd());
		float randomChoice = dist(gen);

		// Iterate over transitions and find the token corresponding to the random choice
		float cumulativeProbability = 0.0f;
		for (const auto& transition : transitions)
		{
			cumulativeProbability += transition.second;
			if (randomChoice < cumulativeProbability)
			{
				return transition.first;
			}
		}

		return " "; // Default return if something goes wrong
	}

	bool MarkovModel::IsSubjectOrVerb(const std::string& token)
	{
		// List of common subjects and verbs (this can be expanded for more complex grammar)
		std::vector<std::string> subjects = { "I", "You", "He", "She", "It", "We", "They" };
		std::vector<std::string> verbs = { "am", "is", "are", "was", "were", "do", "does", "did", "has", "have", "had" };

		if (std::find(subjects.begin(), subjects.end(), token) != subjects.end())
		{
			return true; // It's a subject
		}

		if (std::find(verbs.begin(), verbs.end(), token) != verbs.end())
		{
			return true; // It's a verb
		}

		return false; // It's neither a subject nor a verb
	}

}