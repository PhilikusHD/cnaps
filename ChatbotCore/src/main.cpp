#include "shared/StringUtils.h"
#include "shared/Logger.h"
#include "shared/RandomUtils.h"
#include "shared/Utils.h"
#include "Bot/FSM.h"
#include "Bot/ResponseManager.h"

int main()
{
	FiniteStateMachine fsm(State::Greeting);
	ResponseManager responseManager;

	// Define transitions
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

	// Simulate user input
	std::string input;
	while (fsm.GetCurrentState() != State::Goodbye)
	{
		std::cout << "You: ";
		std::getline(std::cin, input);

		fsm.ProcessInput(input);
		std::cout << "FSM is now in state: " << static_cast<int>(fsm.GetCurrentState()) << "\n";
		std::string response = responseManager.GetResponse(fsm.GetCurrentState(), input, fsm.GetContext());
		if (fsm.GetCurrentState() == State::Solution)
		{
			response += "\n" + responseManager.GenerateSolutionResponse(fsm.GetContext());
		}
		std::cout << "Bot: " << response << std::endl;
		fsm.ResetContext();
	}
	return 0;
}