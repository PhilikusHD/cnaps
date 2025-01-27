#pragma once
#include "Bot/States.h"
#include <string>
#include <unordered_map>

class FiniteStateMachine
{
public:
	explicit FiniteStateMachine(State initialState);
	void AddTransition(State from, State to);
	void AddKeywords(State from, const std::vector<std::string>& keywords);
	State ProcessInput(const std::string& input);

	[[nodiscard]] State GetCurrentState() const;
	[[nodiscard]] std::string GetContext() const;

	bool CheckTransitionForState(State state, const std::string& input) const;

private:
	std::string ExtractContext(const std::string& input);
	void Transition(State newState);
	bool CanTransition(State newState) const;
	bool StateMatchesKeyword(State state, const std::string& keyword) const;
	void AddWords();
private:
	State m_CurrentState;
	std::string m_Context;
	std::unordered_map<State, std::vector<State>> m_Transitions;
	std::unordered_map<State, std::vector<std::string>> m_Keywords;
};