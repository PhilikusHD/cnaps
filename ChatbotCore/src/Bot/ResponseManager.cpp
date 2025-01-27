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
		{Category::Default, {
			"Hello! How can I assist you today?",
			"Hi there! What brings you here?",
			"Greetings! How may I help you?",
			"Hey! What can I do for you?",
			"Hi! Let me know how I can assist.",
			"Welcome! What can I do for you today?",
			"Good day! How can I be of service?",
			"Hi, how’s it going? What do you need help with?"
		}},
		{Category::Context, {
			"Hello! I see you're dealing with {context}. How can I assist you further?",
			"Hi there! You're facing an issue with {context}. What can I do to help?",
			"Greetings! Let's address your {context} situation together.",
			"Hey! It seems {context} is the issue. How can I support you?",
			"Hello! I understand {context} is a concern. How can I help?",
			"Hi! I noticed {context} is what you're dealing with. Let's get started.",
			"Hey there! Let’s solve the {context} issue together."
		}}
	};

	// ProblemDesc responses
	m_ResponseTemplates[State::ProblemDesc] = {
		{Category::Default, {
			"What seems to be the problem?",
			"Please describe the issue you're experiencing.",
			"Could you tell me more about the problem?",
			"What's going on? I'm here to help.",
			"Let me know the details of your issue so I can assist.",
			"Tell me more about what's going wrong.",
			"Could you clarify the issue you're dealing with?",
			"Can you give me some details on what happened?"
		}},
		{Category::Context, {
			"I understand you're facing an issue with {context}. Could you elaborate?",
			"You mentioned {context}-could you provide more details?",
			"It seems the problem is related to {context}. Could you explain further?",
			"I'm here to help with {context}. What's the exact issue?",
			"Let's dive into the {context} issue. What's happening?",
			"Could you walk me through the problem with {context}?",
			"Could you explain the specifics of your {context} issue?"
		}}
	};

	// Consideration responses
	m_ResponseTemplates[State::Consideration] = {
		{Category::Default, {
			"Take your time to consider the options.",
			"Feel free to weigh your options. I'm here when you're ready.",
			"It's okay to think it over-no rush at all.",
			"Let me know if you have any questions while deciding.",
			"I'm just a message away when you've made your choice.",
			"Take your time—I'll be here when you're ready to move forward.",
			"No pressure, feel free to think through your options.",
			"Give it a moment, and let me know if you need more info."
		}},
		{Category::Context, {
			"Considering your issue with {context}, take your time to decide.",
			"If you need more clarity on {context}, just let me know!",
			"Thinking about {context}? I'm here for any follow-up.",
			"Don't worry! Your {context} decision doesn't have to be rushed.",
			"If you're deliberating over {context}, let me know how I can help.",
			"Take your time to consider the options for {context}. I’m here to help.",
			"Let me know if you'd like more info on {context} to help with your decision."
		}}
	};

	// Escalation responses
	m_ResponseTemplates[State::Escalation] = {
		{Category::Default, {
			"I'll escalate this to someone who can assist further.",
			"This issue requires a higher level of support-let me escalate it.",
			"Hold on, I'm reaching out to a supervisor to help.",
			"This needs attention from someone higher up. Escalating now.",
			"Let me get someone with more expertise to assist you.",
			"Let me pass this on to someone who can help you further.",
			"I’ll make sure this gets to the right person for you.",
			"I’m escalating this issue right now, please hold on."
		}},
		{Category::Context, {
			"Given the issue with {context}, I'll escalate this to the right team.",
			"Your concern about {context} will be directed to the appropriate supervisor.",
			"I'm escalating the {context} issue to someone who can resolve it.",
			"For {context}, I'll bring in higher-level assistance.",
			"Your {context} issue deserves immediate escalation-working on it now.",
			"This {context} situation needs special attention. I'm escalating it now.",
			"Escalating the {context} issue to ensure it’s handled quickly."
		}}
	};

	// Goodbye responses
	m_ResponseTemplates[State::Goodbye] = {
		{Category::Default, {
			"Goodbye! Feel free to reach out if you need further help.",
			"Take care! I'm here whenever you need assistance.",
			"See you later! Don't hesitate to contact me again.",
			"Farewell! I hope I was able to help.",
			"Goodbye! Let me know if you need anything else.",
			"Take care and have a great day ahead!",
			"Goodbye! Let me know if you need anything in the future.",
			"Wishing you the best of luck! Let me know if you need assistance again."
		}},
		{Category::Context, {
			"Goodbye! I hope your issue with {context} gets resolved smoothly.",
			"Take care! Let me know if {context} requires further assistance.",
			"Farewell! Wishing you the best with {context}.",
			"Goodbye! If {context} causes further trouble, I'm here to help.",
			"See you later! I trust your {context} situation will improve soon.",
			"Wishing you a quick resolution with {context}! Take care.",
			"Goodbye, {context} will be sorted out in no time!"
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

std::string ResponseManager::BuildContextString(const std::vector<std::string>& contexts)
{
	if (contexts.empty())
	{
		return "";
	}

	// Handle a single context directly
	if (contexts.size() == 1)
	{
		return contexts.front();
	}

	// For two contexts, join with "and"
	if (contexts.size() == 2)
	{
		return contexts[0] + " and " + contexts[1];
	}

	// For more than two contexts, create a comma-separated list with "and" before the last item
	std::string result;
	for (size_t i = 0; i < contexts.size(); ++i)
	{
		if (i > 0)
		{
			if (i == contexts.size() - 1)
			{
				result += " and "; // Add "and" before the last context
			}
			else
			{
				result += ", "; // Commas between other contexts
			}
		}
		result += contexts[i];
	}

	return result;
}
