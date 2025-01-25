#include <iostream>
#include "nlp/Tokenizer.h"
#include <vector>

int main()
{
	Ravi::Tokenizer tokenizer;
	tokenizer.AddDelimiter('-');
	std::string text = "Hello, world! Welcomé to NLP-Tokenizer. Let's tokenize this text.";

	std::vector<std::string> tokens = tokenizer.Tokenize(text);
	std::cout << "Tokens:\n";
	for (const std::string& token : tokens)
	{
		std::cout << "    " << token << std::endl;
	}

	return 0;
}