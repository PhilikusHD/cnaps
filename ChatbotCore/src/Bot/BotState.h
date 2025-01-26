#pragma once

enum class BotState
{
	Idle,               // User is not interacting with the bot
	Asking,             // User is asking a question or making a request
	AwaitingClarification, // User is waiting for the bot to clarify something
	ReceivingAnswer,    // User is receiving the bot's answer or solution
	ConsideringSuggestions, // User is reviewing suggestions offered by the bot
	Complaining,        // User is expressing dissatisfaction or frustration
	Escalating,         // User wants to escalate the issue to a human agent
	Closing,            // User is wrapping up the conversation
	Goodbye,			// User said goodbye
	ErrorOccurred       // User has encountered an error or something unrecognized
};
