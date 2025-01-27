#pragma once
#include <string>
#include <Bot/States.h>
#include <shared/Types.h>
#include <unordered_map>

class ResponseManager
{
public:
	ResponseManager();

	std::string GetResponse(State state, const std::string& userInput, const std::string& context);
private:
	enum class Category : Raven::uint8
	{
		Default,
		Context
	};

	// State, default(0)/contextual(1) response, {response templates}
	std::unordered_map<State, std::unordered_map<Category, std::vector<std::string>>> m_ResponseTemplates;
private:
	void InitializeResponses();
	// Randomly select a template from the given category
	std::string SelectTemplate(const std::unordered_map<Category, std::vector<std::string>>& templates, Category category);
	void ReplacePlaceholder(std::string& templateStr, const std::string& placeholder, const std::string& value);
	std::string AddDynamicFlavor(const std::string& baseResponse, const std::string& userInput);
	std::string BuildContextString(const std::vector<std::string>& contexts);
};