#include "ChatbotAPI.h"
#include "Bot/FSM.h"
#include <Bot/ResponseManager.h>
#include "shared/Logger.h"

static FiniteStateMachine fsm(State::Greeting);
static ResponseManager responseManager;

void InitializeChatbot()
{
	fsm.AddTransition(State::Greeting, State::ProblemDesc);
	fsm.AddTransition(State::Greeting, State::Solution);
	fsm.AddTransition(State::Greeting, State::Goodbye);

	fsm.AddTransition(State::ProblemDesc, State::Consideration);
	fsm.AddTransition(State::ProblemDesc, State::Solution);
	fsm.AddTransition(State::ProblemDesc, State::Escalation);
	fsm.AddTransition(State::ProblemDesc, State::Goodbye);

	fsm.AddTransition(State::Solution, State::Consideration);
	fsm.AddTransition(State::Solution, State::Escalation);
	fsm.AddTransition(State::Solution, State::Goodbye);

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
	fsm.ProcessInput(input);
	std::string response;
	if (fsm.GetRemainedInState() > 3)
	{
		response = "I am unable to fully understand the issue. Please contact human support at: support@bugland.com";
	}
	else
	{
		response = responseManager.GetResponse(fsm.GetCurrentState(), input, fsm.GetContext());
		if (fsm.GetCurrentState() == State::Solution)
		{
			response += "\n" + responseManager.GenerateSolutionResponse(fsm.GetContext());
			fsm.ResetContext();
		}
	}

	// Dynamically allocate memory for the response
	char* result = new char[response.size() + 1]; // +1 for null terminator
	result[response.size()] = '\0';
	strcpy_s(result, response.size() + 1, response.c_str());
	return result;
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

void FreeResponse(char* response)
{
	delete[] response;
}