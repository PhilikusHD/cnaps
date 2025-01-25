#include <iostream>
#include "nlp/Tokenizer.h"
#include <vector>
#include <filesystem>
#include <nlp/CorpusLoader.h>
#include <nlp/MarkovModel.h>
#include <thread>

using namespace Ravi;
int main()
{
	std::cout << std::filesystem::current_path() << std::endl;

	Tokenizer tokenizer;
	MarkovModel bot(5);

	CorpusLoader loader("sample_conversation.txt", &tokenizer);
	auto dialogues = loader.LoadAndTokenize();

	std::vector<std::string> userTokens;
	std::vector<std::string> botTokens;
	for (const auto& dialogue : dialogues)
	{
		botTokens = tokenizer.Tokenize(dialogue.second);
		bot.Train(userTokens, botTokens);
	}

	std::string input;
	std::cout << "Enter text: ";
	std::getline(std::cin, input);

	std::string botResponse = bot.GenerateResponse(input);
	std::cout << "User: " << input << std::endl;
	std::cout << "Bot: " << botResponse << std::endl;

	return 0;
}