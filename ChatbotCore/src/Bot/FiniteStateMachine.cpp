#include "Bot/FiniteStateMachine.h"

std::unordered_map<BotState, std::unordered_map<std::string, BotState>> FiniteStateMachine::m_KeywordTransitions = {
	// Idle State
	{BotState::Idle, {
		{"help", BotState::Asking},
		{"support", BotState::Asking},
		{"complaint", BotState::Complaining},
		{"start", BotState::Asking},
		{"issue", BotState::Asking},
		{"question", BotState::Asking},
		{"problem", BotState::Asking},
		{"hello", BotState::Asking},
		{"hi", BotState::Asking},
		{"start a conversation", BotState::Asking},
		{"need assistance", BotState::Asking}
	}},

	// Asking State
	{BotState::Asking, {
		{"clarify", BotState::AwaitingClarification},
		{"answer", BotState::ReceivingAnswer},
		{"explain", BotState::ReceivingAnswer},
		{"details", BotState::ReceivingAnswer},
		{"more info", BotState::ReceivingAnswer},
		{"solution", BotState::ReceivingAnswer},
		{"suggest", BotState::ConsideringSuggestions},
		{"recommend", BotState::ConsideringSuggestions},
		{"advice", BotState::ConsideringSuggestions},
		{"helpful", BotState::ReceivingAnswer},
		{"assist", BotState::ReceivingAnswer},
		{"cancel", BotState::Closing},
		{"escalate", BotState::Escalating},
		{"feedback", BotState::Complaining},
		{"complaint", BotState::Complaining}
	}},

	// AwaitingClarification State
	{BotState::AwaitingClarification, {
		{"yes", BotState::Asking},
		{"no", BotState::Asking},
		{"clarified", BotState::ReceivingAnswer},
		{"understood", BotState::ReceivingAnswer},
		{"details", BotState::Asking},
		{"not sure", BotState::Asking},
		{"confused", BotState::Asking},
		{"confirm", BotState::Asking},
		{"answer", BotState::ReceivingAnswer},
		{"explain", BotState::ReceivingAnswer}
	}},

	// ReceivingAnswer State
	{BotState::ReceivingAnswer, {
		{"more", BotState::ReceivingAnswer},
		{"suggestions", BotState::ConsideringSuggestions},
		{"offer", BotState::ConsideringSuggestions},
		{"help", BotState::ConsideringSuggestions},
		{"clarify", BotState::AwaitingClarification},
		{"complain", BotState::Complaining},
		{"feedback", BotState::Complaining},
		{"escalate", BotState::Escalating},
		{"escalate to human", BotState::Escalating},
		{"contact agent", BotState::Escalating},
		{"close", BotState::Closing},
		{"end", BotState::Closing},
		{"stop", BotState::Closing}
	}},

	// ConsideringSuggestions State
	{BotState::ConsideringSuggestions, {
		{"yes", BotState::ReceivingAnswer},
		{"no", BotState::Closing},
		{"ask", BotState::Asking},
		{"end", BotState::Closing},
		{"dismiss", BotState::Closing},
		{"stop", BotState::Closing},
		{"thank you", BotState::Closing},
		{"close", BotState::Closing},
		{"escalate", BotState::Escalating},
		{"help", BotState::Asking},
		{"more", BotState::ReceivingAnswer}
	}},

	// Complaining State
	{BotState::Complaining, {
		{"escalate", BotState::Escalating},
		{"retry", BotState::ReceivingAnswer},
		{"fix it", BotState::ReceivingAnswer},
		{"contact agent", BotState::Escalating},
		{"contact support", BotState::Escalating},
		{"refund", BotState::Escalating},
		{"end", BotState::Closing},
		{"close", BotState::Closing},
		{"thanks", BotState::Closing},
		{"goodbye", BotState::Goodbye},
		{"cancel", BotState::Closing}
	}},

	// Escalating State
	{BotState::Escalating, {
		{"end", BotState::Closing},
		{"help", BotState::Asking},
		{"close", BotState::Closing},
		{"stop", BotState::Closing},
		{"thank you", BotState::Closing},
		{"close conversation", BotState::Closing},
		{"goodbye", BotState::Goodbye}
	}},

	// Closing State
	{BotState::Closing, {
		{"restart", BotState::Idle},
		{"end", BotState::Idle},
		{"exit", BotState::Idle},
		{"goodbye", BotState::Goodbye},
		{"thanks", BotState::Goodbye},
		{"stop", BotState::Goodbye},
		{"quit", BotState::Goodbye}
	}},

	// ErrorOccurred State
	{BotState::ErrorOccurred, {
		{"retry", BotState::ReceivingAnswer},
		{"restart", BotState::Idle},
		{"reset", BotState::Idle},
		{"end", BotState::Idle},
		{"close", BotState::Idle}
	}}
};

FiniteStateMachine::FiniteStateMachine(BotState initialState) : m_CurrentState(initialState) {}

void FiniteStateMachine::AddTransition(BotState fromState, BotState toState)
{
	m_Transitions[fromState].emplace_back(toState);
}

// Add keyword-based transitions

void FiniteStateMachine::AddKeywordTransition(BotState fromState, const std::vector<std::string>& keywords, BotState toState)
{
	for (const auto& keyword : keywords)
	{
		m_KeywordTransitions[fromState][keyword] = toState;
	}
}

// Check if a transition from the current state to a new state is valid

bool FiniteStateMachine::CanTransition(BotState newState) const
{
	auto it = m_Transitions.find(m_CurrentState);
	if (it != m_Transitions.end())
	{
		const auto& validStates = it->second;
		return std::find(validStates.begin(), validStates.end(), newState) != validStates.end();
	}

	return false;
}


// Handle user input and transition based on keywords

void FiniteStateMachine::HandleUserInput(const std::string& userInput)
{
	// Try keyword-based transition
	auto keywordTransition = m_KeywordTransitions.find(m_CurrentState);
	if (keywordTransition != m_KeywordTransitions.end())
	{
		for (const auto& [keyword, nextState] : keywordTransition->second)
		{
			if (userInput.find(keyword) != std::string::npos)
			{
				Logger::Log("Keyword detected: " + keyword + ". Transitioning to " + StateToString(nextState));
				Transition(nextState);
				return;
			}
		}
	}

	// Default action (based on normal transitions)
	// This can be extended if needed
	Logger::Log("No matching keyword found, no transition made.");
}

void FiniteStateMachine::Transition(BotState newState)
{
	if (CanTransition(newState))
	{
		Logger::Log("Transitioning from " + StateToString(m_CurrentState) + " to " + StateToString(newState));
		m_CurrentState = newState;
	}
	else
	{
		throw std::logic_error("Invalid transition from " + StateToString(m_CurrentState) + " to " + StateToString(newState));
	}
}

BotState FiniteStateMachine::GetCurrentState() const { return m_CurrentState; }

std::string FiniteStateMachine::StateToString(BotState state) const
{
	return std::to_string(static_cast<int>(state));
}
