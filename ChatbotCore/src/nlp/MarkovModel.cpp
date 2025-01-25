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
		// Launch two threads for training on userTokens and botTokens
		std::thread userThread([&]() {
			TrainOnTokens(userTokens, m_UserMarkov);
			});

		std::thread botThread([&]() {
			TrainOnTokens(botTokens, m_BotMarkov);
			});

		userThread.join();
		botThread.join();
	}

	std::string MarkovModel::GenerateResponse(const std::string& userInput)
	{
		std::vector<std::string> userTokens = m_Tokenizer.Tokenize(userInput);
		std::vector<std::string> response = GenerateFromMarkov(userTokens, m_BotMarkov);
		return Utils::JoinTokens(response);
	}

	void MarkovModel::TrainOnTokens(const std::vector<std::string>& tokens, std::map<std::vector<std::string>, std::map<std::string, int>>& markovModel) const
	{
		// Check if there are enough tokens in the vector
		if (tokens.size() >= m_NGrams + 1)
		{
			for (size_t i = 0; i < tokens.size() - m_NGrams; ++i)
			{
				std::vector<std::string> state(tokens.begin() + i, tokens.begin() + i + m_NGrams);
				std::string nextToken = tokens[i + m_NGrams];

				markovModel[state][nextToken]++;
			}
		}
		else
		{
			// Handle the case where there are not enough tokens for m_NGrams + 1
			std::cout << "Not enough tokens to build the model." << std::endl;
		}
	}

	std::vector<std::string> MarkovModel::GenerateFromMarkov(
		const std::vector<std::string>& userInput,
		const std::map<std::vector<std::string>, std::map<std::string, int>>& markovModel)
	{
		std::vector<std::string> response;

		// Choose a context size dynamically based on the user input
		size_t contextSize = std::min(userInput.size(), m_NGrams); // Allow flexibility in context size
		std::vector<std::string> state(userInput.end() - contextSize, userInput.end());

		// Debugging: print the state vector to check its content
		std::cout << "Initial State: ";
		for (const auto& token : state)
		{
			std::cout << token << " ";
		}
		std::cout << std::endl;

		// Limit response length to avoid endless generation
		for (size_t i = 0; i < 25; ++i)
		{
			// Look for the next possible state based on context
			auto it = markovModel.lower_bound(state);
			if (it != markovModel.end())
			{
				auto& transitions = it->second;
				std::string nextToken = ChooseNextTokenFromProbabilities(transitions);

				// Apply some rules to prevent nonsensical sentence structure
				if (i == 0 && (nextToken == "the" || nextToken == "and" || nextToken == "to"))
				{
					continue;  // Avoid starting with filler words
				}

				// Enforce minimal sentence structure:
				if (i == 0 && !response.empty() && (response.back() == "to" || response.back() == "and"))
				{
					continue; // Avoid starting new sentences with prepositions
				}

				// Avoid repetitive phrases
				if (!response.empty() && nextToken == response.back())
				{
					continue;
				}

				response.emplace_back(nextToken);

				// Update the state for the next token generation
				state.emplace_back(nextToken);
				state.erase(state.begin()); // Maintain fixed size context window
			}
			else
			{
				std::cout << "No valid transition found, breaking loop." << std::endl;
				break; // Exit if no valid transition is found
			}
		}

		return response;
	}


	std::string MarkovModel::ChooseNextTokenFromProbabilities(
		const std::map<std::string, int>& transitions)
	{
		// Compute the total frequency of transitions
		int totalFrequency = 0;
		for (const auto& transition : transitions)
		{
			totalFrequency += transition.second;
		}

		// Select a random number between 0 and totalFrequency
		std::uniform_int_distribution<int> dist(0, totalFrequency - 1);
		std::random_device rd;
		std::mt19937 gen(rd());
		int randomChoice = dist(gen);

		// Iterate over the transitions and find the token that corresponds to the random choice
		int cumulativeFrequency = 0;
		for (const auto& transition : transitions)
		{
			cumulativeFrequency += transition.second;
			if (randomChoice < cumulativeFrequency)
			{
				return transition.first; // Return the chosen token
			}
		}

		return ""; // Default return if something goes wrong
	}


	std::string MarkovModel::ChooseNextToken(const std::map<std::string, int>& transitions)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dist(0, transitions.size() - 1);

		std::vector<std::string> keys;
		for (const auto& pair : transitions)
		{
			keys.emplace_back(pair.first);
		}

		return keys[dist(gen)];
	}
}