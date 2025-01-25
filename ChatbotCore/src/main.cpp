#include <iostream>
#include "nlp/Tokenizer.h"
#include <vector>

int main()
{
	Ravi::Tokenizer tokenizer;
	std::string text = "Hellô, world! I'm Wêlcomé to NLP-Tokenizer. We're Let's tokenize this text.";

	std::vector<std::string> tokens = tokenizer.Tokenize(text);
	std::cout << "Tokens:\n";
	for (const std::string& token : tokens)
	{
		std::cout << "    " << token << std::endl;
	}

	return 0;
}