#include "shared/StringUtils.h"
#include "shared/Logger.h"
#include "shared/RandomUtils.h"
#include "shared/Utils.h"

#include "Bot/BotState.h"
#include "Bot/FiniteStateMachine.h"
#include "Bot/ResponseManager.h"


int main()
{
	// Create FSM instance and setup states and transitions
	FiniteStateMachine fsm(BotState::Idle);

	// Add all basic state transitions
	fsm.AddTransition(BotState::Idle, BotState::Asking);
	fsm.AddTransition(BotState::Idle, BotState::ErrorOccurred);
	fsm.AddTransition(BotState::Idle, BotState::Complaining);

	fsm.AddTransition(BotState::Asking, BotState::ReceivingAnswer);
	fsm.AddTransition(BotState::Asking, BotState::AwaitingClarification);
	fsm.AddTransition(BotState::Asking, BotState::Complaining);
	fsm.AddTransition(BotState::Asking, BotState::ErrorOccurred);

	fsm.AddTransition(BotState::AwaitingClarification, BotState::Asking);
	fsm.AddTransition(BotState::AwaitingClarification, BotState::ReceivingAnswer);
	fsm.AddTransition(BotState::AwaitingClarification, BotState::ErrorOccurred);

	fsm.AddTransition(BotState::ReceivingAnswer, BotState::ConsideringSuggestions);
	fsm.AddTransition(BotState::ReceivingAnswer, BotState::Complaining);
	fsm.AddTransition(BotState::ReceivingAnswer, BotState::Escalating);
	fsm.AddTransition(BotState::ReceivingAnswer, BotState::Closing);

	fsm.AddTransition(BotState::ConsideringSuggestions, BotState::Asking);
	fsm.AddTransition(BotState::ConsideringSuggestions, BotState::Closing);
	fsm.AddTransition(BotState::ConsideringSuggestions, BotState::Escalating);
	fsm.AddTransition(BotState::ConsideringSuggestions, BotState::Complaining);

	fsm.AddTransition(BotState::Complaining, BotState::Escalating);
	fsm.AddTransition(BotState::Complaining, BotState::ErrorOccurred);
	fsm.AddTransition(BotState::Complaining, BotState::ReceivingAnswer);
	fsm.AddTransition(BotState::Complaining, BotState::Closing);

	fsm.AddTransition(BotState::Escalating, BotState::Closing);
	fsm.AddTransition(BotState::Escalating, BotState::ErrorOccurred);

	fsm.AddTransition(BotState::Closing, BotState::Idle);
	fsm.AddTransition(BotState::Closing, BotState::ErrorOccurred);

	fsm.AddTransition(BotState::ErrorOccurred, BotState::Idle);
	fsm.AddTransition(BotState::ErrorOccurred, BotState::ReceivingAnswer);



	// Simulate user input
	while (true)
	{
		try
		{
			std::string userInput;
			std::cout << "User: ";
			std::getline(std::cin, userInput);
			fsm.HandleUserInput(userInput);
			std::cout << "Current State: " << static_cast<int>(fsm.GetCurrentState()) << std::endl;
		}
		catch (std::logic_error& e)
		{
			std::cout << "Invalid state transition: " << e.what() << std::endl;
		}
	}


	// Check the current state

	return 0;
}