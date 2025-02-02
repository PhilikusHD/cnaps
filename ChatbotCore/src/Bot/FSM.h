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
	[[nodiscard]] Raven::uint8 GetRemainedInState() const { return m_InState; };
	void ResetContext() { m_Context = ""; }

	bool CheckTransitionForState(State state, const std::string& input) const;

private:
	std::string ExtractContext(const std::string& input);
	void Transition(State newState);
	bool CanTransition(State newState) const;
	void AddWords();
private:
	State m_CurrentState;
	std::string m_Context;
	Raven::uint8 m_InState = 0;
	std::unordered_map<State, std::vector<State>> m_Transitions;
	std::unordered_map<State, std::vector<std::string>> m_Keywords;
};