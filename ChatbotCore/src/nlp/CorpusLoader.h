#pragma once
#include <string>
#include <vector>
#include <functional>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Tokenizer.h"

namespace Ravi
{
	class CorpusLoader
	{
	public:
		explicit CorpusLoader(const std::string& filePath, Tokenizer* tokenizer)
			: m_FilePath(filePath), m_Tokenizer(tokenizer)
		{
		}

		std::vector<std::pair<std::string, std::string>> LoadAndTokenize()
		{
			std::string content = ReadFile();
			return ParseDialogues(content);
		}


	private:
		std::string m_FilePath;
		Tokenizer* m_Tokenizer;
		std::string JoinTokens(const std::vector<std::string>& tokens)
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


		std::string ReadFile()
		{
			std::ifstream file(m_FilePath);
			if (!file.is_open())
			{
				// TODO: Replace with error manager call
				std::cerr << "Error: Unable to open file: " << m_FilePath << std::endl;
				return "";
			}

			std::stringstream buffer;
			buffer << file.rdbuf();
			return buffer.str();
		}

		std::vector<std::pair<std::string, std::string>> ParseDialogues(const std::string& content)
		{
			std::vector<std::pair<std::string, std::string>> dialogues;
			std::istringstream stream(content);
			std::string line;
			std::string userInput, botResponse;

			bool is_user = true;

			while (std::getline(stream, line))
			{
				if (line.substr(0, 5) == "User:")
				{
					if (!userInput.empty())
					{
						userInput = JoinTokens(m_Tokenizer->Tokenize(userInput));
						botResponse = JoinTokens(m_Tokenizer->Tokenize(botResponse));

						dialogues.emplace_back(userInput, botResponse);
					}
					userInput = line.substr(6); // Skip the "User:" part
				}
				else if (line.substr(0, 4) == "Bot:")
				{
					botResponse = line.substr(5); // Skip the "Bot:" part
				}
			}

			// Add the last dialogue pair
			if (!userInput.empty() && !botResponse.empty())
			{
				userInput = JoinTokens(m_Tokenizer->Tokenize(userInput));
				botResponse = JoinTokens(m_Tokenizer->Tokenize(botResponse));

				dialogues.emplace_back(userInput, botResponse);
			}

			return dialogues;
		}
	};
}