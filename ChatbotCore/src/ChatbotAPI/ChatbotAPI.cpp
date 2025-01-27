#include "ChatbotAPI.h"
#include "Bot/FSM.h"

static FiniteStateMachine fsm(State::Greeting);

void InitializeChatbot()
{
	fsm.AddTransition(State::Greeting, State::ProblemDesc);
	fsm.AddTransition(State::Greeting, State::Goodbye);

	fsm.AddTransition(State::ProblemDesc, State::Consideration);
	fsm.AddTransition(State::ProblemDesc, State::Escalation);
	fsm.AddTransition(State::ProblemDesc, State::Goodbye);

	fsm.AddTransition(State::Consideration, State::Escalation);
	fsm.AddTransition(State::Consideration, State::ProblemDesc);
	fsm.AddTransition(State::Consideration, State::Goodbye);

	fsm.AddTransition(State::Escalation, State::Goodbye);
}

void ShutdownChatbot()
{
	// Not implemented
}

const char* GenerateResponse(const char* input)
{
	// Not implemented
	return "";
}

const bool GetStatus()
{
	// Not implemented
	return false;
}

const char* GetError()
{
	// Not implemented
	return "";
}