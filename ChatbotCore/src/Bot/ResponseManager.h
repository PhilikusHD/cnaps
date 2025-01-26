#pragma once
#include <Bot/BotState.h>
#include <string>
#include <functional>
#include <map>

class ResponseManager
{
public:
	ResponseManager();

	// Adds a predefined response for a given type
	void AddResponse(BotState type, const std::string& response);
	// Generates a response based on input and state
	std::string GenerateResponse(const std::string& userInput);
	// Set transition handler (Can be used for mapping input to response type)
	void SetTransitionHandler(std::function<BotState(const std::string&)> handler);
private:
	// Map storing responses for each type
	std::map<BotState, std::vector<std::string>> m_Responses;

	// A function to transition between different types based on user input
	std::function<BotState(const std::string&)> m_TransitionHandler;
};