#include "ResponseManager.h"
#include "shared/RandomUtils.h"

ResponseManager::ResponseManager()
{
	InitializeResponses();
}

std::string ResponseManager::GetResponse(State state, const std::string& userInput, const std::string& context)
{
	auto& templates = m_ResponseTemplates[state];
	std::string response;

	if (!context.empty())
	{
		// Contextual response if available
		response = SelectTemplate(templates, Category::Context);
		ReplacePlaceholder(response, "{context}", context);
	}
	else
	{
		response = SelectTemplate(templates, Category::Default);
	}

	// Add some dynamic flavor based on input
	response = AddDynamicFlavor(response, userInput);
	return response;
}

void ResponseManager::InitializeResponses()
{
	// Greeting responses
	m_ResponseTemplates[State::Greeting] = {
		{Category::Default , {
			"Hello! How can I assist you today?",
			"Hi there! What brings you here?",
			"Greetings! How may I help you?",
			"Hey! What can I do for you?",
			"Hi! Let me know how I can assist."
		}},
		{Category::Context, {
			"Hello! I see you're dealing with {context}. How can I assist you further?",
			"Hi there! You're facing an issue with {context}. What can I do to help?",
			"Greetings! Let's address your {context} situation together.",
			"Hey! It seems {context} is the issue. How can I support you?"
		}}
	};

	// ProblemDesc responses
	m_ResponseTemplates[State::ProblemDesc] = {
		{Category::Default, {
			"What seems to be the problem?",
			"Please describe the issue you're experiencing.",
			"Could you tell me more about the problem?",
			"What's going on? I'm here to help.",
			"Let me know the details of your issue so I can assist."
		}},
		{Category::Context, {
			"I understand you're facing an issue with {context}. Could you elaborate?",
			"You mentioned {context}-could you provide more details?",
			"It seems the problem is related to {context}. Could you explain further?",
			"I'm here to help with {context}. What's the exact issue?",
			"Let's dive into the {context} issue. What's happening?"
		}}
	};

	// Consideration responses
	m_ResponseTemplates[State::Consideration] = {
		{Category::Default, {
			"Take your time to consider the options.",
			"Feel free to weigh your options. I'm here when you're ready.",
			"It's okay to think it over-no rush at all.",
			"Let me know if you have any questions while deciding.",
			"I'm just a message away when you've made your choice."
		}},
		{Category::Context, {
			"Considering your issue with {context}, take your time to decide.",
			"If you need more clarity on {context}, just let me know!",
			"Thinking about {context}? I'm here for any follow-up.",
			"Don't worry! Your {context} decision doesn't have to be rushed.",
			"If you're deliberating over {context}, let me know how I can help."
		}}
	};

	// Escalation responses
	m_ResponseTemplates[State::Escalation] = {
		{Category::Default, {
			"I'll escalate this to someone who can assist further.",
			"This issue requires a higher level of support-let me escalate it.",
			"Hold on, I'm reaching out to a supervisor to help.",
			"This needs attention from someone higher up. Escalating now.",
			"Let me get someone with more expertise to assist you."
		}},
		{Category::Context, {
			"Given the issue with {context}, I'll escalate this to the right team.",
			"Your concern about {context} will be directed to the appropriate supervisor.",
			"I'm escalating the {context} issue to someone who can resolve it.",
			"For {context}, I'll bring in higher-level assistance.",
			"Your {context} issue deserves immediate escalation-working on it now."
		}}
	};

	// Goodbye responses
	m_ResponseTemplates[State::Goodbye] = {
		{Category::Default, {
			"Goodbye! Feel free to reach out if you need further help.",
			"Take care! I'm here whenever you need assistance.",
			"See you later! Don't hesitate to contact me again.",
			"Farewell! I hope I was able to help.",
			"Goodbye! Let me know if you need anything else."
		}},
		{Category::Context, {
			"Goodbye! I hope your issue with {context} gets resolved smoothly.",
			"Take care! Let me know if {context} requires further assistance.",
			"Farewell! Wishing you the best with {context}.",
			"Goodbye! If {context} causes further trouble, I'm here to help.",
			"See you later! I trust your {context} situation will improve soon."
		}}
	};

}

// Randomly select a template from the given category
std::string ResponseManager::SelectTemplate(const std::unordered_map<Category, std::vector<std::string>>& templates, Category category)
{
	auto it = templates.find(category);
	if (it != templates.end() && !it->second.empty())
	{
		return RandomUtils::RandomElement(it->second);
	}
	return "I am unsure what to say...";
}

void ResponseManager::ReplacePlaceholder(std::string& templateStr, const std::string& placeholder, const std::string& value)
{
	size_t pos = templateStr.find(placeholder);
	if (pos != std::string::npos)
	{
		templateStr.replace(pos, placeholder.length(), value);
	}
}

std::string ResponseManager::AddDynamicFlavor(const std::string& baseResponse, const std::string& userInput)
{
	std::string response = baseResponse;

	if (userInput.find("urgent") != std::string::npos)
	{
		response += " I can see this is urgent. Let's resolve it quickly!";
	}
	else if (userInput.find("thanks") != std::string::npos)
	{
		response += " You're welcome!";
	}
	return response;
}
