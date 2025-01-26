#include "ResponseManager.h"

ResponseManager::ResponseManager()
{
}

void ResponseManager::AddResponse(BotState type, const std::string& response)
{
	m_Responses[type].emplace_back(response);
}

std::string ResponseManager::GenerateResponse(const std::string& userInput)
{
	if (m_TransitionHandler)
	{
		// Get the new state/type based on user input
		BotState newState = m_TransitionHandler(userInput);
		const auto& responses = m_Responses[newState];
		if (!responses.empty())
		{
			return responses[0];
		}
	}

	return "I don't know how to respond to that.";
}

void ResponseManager::SetTransitionHandler(std::function<BotState(const std::string&)> handler)
{
	m_TransitionHandler = std::move(handler);
}
