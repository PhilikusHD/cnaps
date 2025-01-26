#include "shared/StringUtils.h"
#include "shared/Logger.h"
#include "shared/RandomUtils.h"
#include "shared/Utils.h"

#include "Bot/FSM.h"


int main()
{
	FiniteStateMachine fsm(State::Greeting);

	// Define transitions
	fsm.AddTransition(State::Greeting, State::ProblemDesc);
	fsm.AddTransition(State::Greeting, State::Goodbye);

	fsm.AddTransition(State::ProblemDesc, State::Consideration);
	fsm.AddTransition(State::ProblemDesc, State::Escalation);
	fsm.AddTransition(State::ProblemDesc, State::Goodbye);

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
	}
	return 0;
}