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
	std::string GenerateSolutionResponse(const std::string& context);
private:
	enum class Category : Raven::uint8
	{
		Default,
		Context
	};

	// State, default(0)/contextual(1) response, {response templates}
	std::unordered_map<State, std::unordered_map<Category, std::vector<std::string>>> m_ResponseTemplates;

	std::unordered_map<ContextCategory, std::vector<std::string>> m_SolutionSteps;
private:
	void InitializeResponses();
	// Randomly select a template from the given category
	std::string SelectTemplate(const std::unordered_map<Category, std::vector<std::string>>& templates, Category category);
	void ReplacePlaceholder(std::string& templateStr, const std::string& placeholder, const std::string& value);
	std::string AddDynamicFlavor(const std::string& baseResponse, const std::string& userInput);
	ContextCategory CategorizeEntity(const std::string& entity);
	std::vector<std::string> GetSolutionSteps(ContextCategory context);
	std::vector<std::string> SplitContext(const std::string& context);
};