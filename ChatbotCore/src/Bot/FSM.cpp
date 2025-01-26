#include "FSM.h"
#include "shared/Logger.h"

FiniteStateMachine::FiniteStateMachine(State initialState)
	: m_CurrentState(initialState)
{
	AddWords();
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
	Logger::Log("Processing input: " + input);

	// Iterate over all possible transitions from the current state
	for (const auto& nextState : m_Transitions[m_CurrentState])
	{
		Logger::Log("Checking keywords for transition to state: " + std::to_string(static_cast<int>(nextState)));

		// Check if input matches any keyword for the potential transition state
		if (CheckTransitionForState(nextState, input))
		{
			Logger::Log("Transitioning to state: " + std::to_string(static_cast<int>(nextState)));
			Transition(nextState);
			return m_CurrentState;
		}
	}
	Logger::Log("No valid transition found. Staying in current state: " + std::to_string(static_cast<int>(m_CurrentState)));
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

void FiniteStateMachine::AddWords()
{
	// Greeting state (State 0)
	AddKeywords(State::Greeting, {
		"hi", "hello", "hey", "greetings", "morning", "evening", "what's up",
		"yo", "howdy", "what's good", "salutations", "how's it going", "good day",
		"howdy", "hola", "how are you", "hey there", "how's it going", "what's happening",
		"howdy partner", "hey buddy", "good evening", "how’s your day", "hey there!"
		});

	// Problem Description state (State 1)
	AddKeywords(State::ProblemDesc, {
		"problem", "issue", "complaint", "concern", "trouble", "difficulty",
		"error", "bug", "malfunction", "doesn't work", "not working", "broken",
		"malfunctioning", "fault", "glitch", "doesn't function", "failing",
		"not responding", "not performing", "can't use", "can't work", "it's broken",
		"it's failing", "issue with", "not fixed", "not working as expected", "stop working", "help"
		});

	// Consideration state (State 2)
	AddKeywords(State::Consideration, {
		"consider", "think about", "evaluate", "review", "ponder", "analyze",
		"reflect", "think over", "contemplate", "decision", "weighing options",
		"reviewing", "mulling over", "assessment", "deliberating", "checking out",
		"thinking", "what do you think", "let me think", "let me consider", "what should I do",
		"I need time to think", "still considering", "thinking about it", "can you give me a moment",
		"let me mull it over", "pondering over it", "checking my options", "what are my choices"
		});

	// Escalation state (State 3)
	AddKeywords(State::Escalation, {
		"escalate", "supervisor", "manager", "higher", "boss", "help", "escalate issue",
		"escalate concern", "senior", "executive", "authority", "escalation", "elevate",
		"higher-up", "reach out to manager", "get a supervisor", "send to higher authority", "superior",
		"take it higher", "send to someone else", "need a higher authority", "manager please",
		"I need help from a supervisor", "speak to the manager", "get a senior manager", "need escalation",
		"escalate this to manager", "raise the concern", "talk to someone higher", "I need a senior person"
		});

	// Goodbye state (State 4)
	AddKeywords(State::Goodbye, {
		"bye", "goodbye", "see you", "take care", "thanks", "thank you", "farewell",
		"later", "peace", "have a good day", "good night", "talk to you later", "catch you later",
		"see you soon", "thanks for your help", "appreciate it", "goodbye for now", "see you later",
		"take care now", "talk soon", "bye for now", "thanks again", "thanks for your time", "farewell for now",
		"see you around", "until next time", "have a good one", "I’m off", "bye bye", "have a nice day",
		"appreciate your help", "thank you for helping", "it was nice talking", "goodbye and take care"
		});
}
