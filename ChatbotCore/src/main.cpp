#include <iostream>
#include "nlp/Tokenizer.h"
#include <vector>
#include <filesystem>
#include <nlp/CorpusLoader.h>

using namespace Ravi;
int main()
{
	std::cout << std::filesystem::current_path() << std::endl;

	Tokenizer tokenizer;
	// Measure time before loading and tokenizing
	auto start = std::chrono::high_resolution_clock::now();
	CorpusLoader loader("sample_conversation.txt", &tokenizer);
	auto dialogues = loader.LoadAndTokenize();

	// Measure time after the operation
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	std::cout << "Time taken to load and tokenize: " << duration.count() << " ms" << std::endl;

	for (const auto& dialogue : dialogues)
	{
		std::cout << "User: ";
		std::cout << dialogue.first;
		std::cout << std::endl;

		std::cout << "Bot: ";
		std::cout << dialogue.second;
		std::cout << std::endl;
	}

	return 0;
}