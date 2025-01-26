#pragma once
#include <vector>
#include <string>
#include <type_traits>
#include "shared/Logger.h"
#include "Bot/BotState.h"
#include <unordered_map>

class FiniteStateMachine
{
public:
	explicit FiniteStateMachine(BotState initialState);
	// Add a valid transition from one state to another
	void AddTransition(BotState fromState, BotState toState);

	// Add keyword-based transitions
	void AddKeywordTransition(BotState fromState, const std::vector<std::string>& keywords, BotState toState);


	// Check if a transition from the current state to a new state is valid
	bool CanTransition(BotState newState) const;

	// Handle user input and transition based on keywords
	void HandleUserInput(const std::string& userInput);

	void Transition(BotState newState);

	BotState GetCurrentState() const;

private:
	BotState m_CurrentState;
	std::unordered_map<BotState, std::vector<BotState>> m_Transitions;
	static std::unordered_map<BotState, std::unordered_map<std::string, BotState>> m_KeywordTransitions;

	std::string StateToString(BotState state) const;
};