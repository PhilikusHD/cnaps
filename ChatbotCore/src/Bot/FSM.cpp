#include "FSM.h"
#include "shared/Logger.h"

FiniteStateMachine::FiniteStateMachine(State initialState)
	: m_CurrentState(initialState)
{
}

void FiniteStateMachine::AddTransition(State from, State to)
{
	m_Transitions[from].emplace_back(to);
}

void FiniteStateMachine::AddKeywords(State from, const std::vector<std::string>& keywords)
{
	m_Keywords[from] = keywords;
}

State FiniteStateMachine::ProcessInput(const std::string& input)
{
	std::cout << "Processing input: " << input << "\n";

	// Iterate over all possible transitions from the current state
	for (const auto& nextState : m_Transitions[m_CurrentState])
	{
		std::cout << "Checking keywords for transition to state: " << static_cast<int>(nextState) << "\n";

		// Check if input matches any keyword for the potential transition state
		if (CheckTransitionForState(nextState, input))
		{
			std::cout << "Transitioning to state: " << static_cast<int>(nextState) << "\n";
			Transition(nextState);
			return m_CurrentState;
		}
	}
	std::cout << "No valid transition found. Staying in current state: " << static_cast<int>(m_CurrentState) << "\n";
	return m_CurrentState;
}

State FiniteStateMachine::GetCurrentState()
{
	return m_CurrentState;
}

bool FiniteStateMachine::CheckTransitionForState(State state, const std::string& input) const
{
	auto it = m_Keywords.find(state);
	if (it != m_Keywords.end())
	{
		for (const auto& keyword : it->second)
		{
			if (input.find(keyword) != std::string::npos)
			{
				return true;
			}
		}
	}
	return false;
}

void FiniteStateMachine::Transition(State newState)
{
	if (CanTransition(newState))
	{
		m_CurrentState = newState;
	}
	else
	{
		Logger::Log("Invalid state transition!", Logger::Level::ERROR);
	}
}

bool FiniteStateMachine::CanTransition(State newState) const
{
	auto it = m_Transitions.find(m_CurrentState);
	return it != m_Transitions.end() && std::find(it->second.begin(), it->second.end(), newState) != it->second.end();
}

bool FiniteStateMachine::StateMatchesKeyword(State state, const std::string& keyword) const
{
	auto it = m_Keywords.find(state);
	return it != m_Keywords.end() &&
		std::find(it->second.begin(), it->second.end(), keyword) != it->second.end();
}
