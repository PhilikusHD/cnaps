#include "shared/StringUtils.h"
#include "shared/Logger.h"
#include "shared/RandomUtils.h"
#include "shared/Utils.h"
#include "ChatbotAPI/ChatBotApi.h"
#include "Bot/FSM.h"

extern FiniteStateMachine fsm;

int main()
{
	InitializeChatbot();

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