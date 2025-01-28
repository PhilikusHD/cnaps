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

std::string ResponseManager::GenerateSolutionResponse(const std::string& context)
{
	// Step 1: Split the context into entities (keywords)
	std::vector<std::string> entities = SplitContext(context);
	std::string response;

	// Step 2: Categorize entities and generate a response
	for (const auto& entity : entities)
	{
		// Example: Use `ExtractContext` to map the entity to its category
		ContextCategory category = CategorizeEntity(entity); // We assume a method to map entities to categories

		// Get solution steps for this category
		std::vector<std::string> solutionSteps = GetSolutionSteps(category);

		// Add the solution steps to the response
		response += "For '" + entity + "' try the following steps : \n";
		for (const auto& step : solutionSteps)
		{
			response += "- " + step + "\n";
		}
		response += "\n"; // Add a newline between each entity's solution
	}

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
			"Hi, how's it going? What do you need help with?"
		}},
		{Category::Context, {
			"Hello! I see you're dealing with {context}. How can I assist you further?",
			"Hi there! You're facing an issue with {context}. What can I do to help?",
			"Greetings! Let's address your {context} situation together.",
			"Hey! It seems {context} is the issue. How can I support you?",
			"Hello! I understand {context} is a concern. How can I help?",
			"Hi! I noticed {context} is what you're dealing with. Let's get started.",
			"Hey there! Let's solve the {context} issue together."
		}}
	};

	// Solution responses
	m_ResponseTemplates[State::Solution] = {
		{Category::Default, {
			"Here's what I suggest: try the following steps to resolve the issue.",
			"I've thought of a solution! Let's start with these steps.",
			"Let me guide you through some steps to solve this.",
			"Here's a possible solution you can try. Let's tackle this together.",
			"I've got a suggestion to fix the problem. Here's what we can do.",
			"Let's work on resolving this. Try the steps I'll outline below.",
			"Here's how we can address the issue step by step.",
			"I've figured out some actions we can take to resolve this. Let's get started."
		}},
		{Category::Context, {
			"For your {context} issue, I recommend starting with the following steps.",
			"It seems the problem involves {context}. Here's what we can do to fix it.",
			"Let's address the {context} situation. Try this solution.",
			"For {context}, here's a suggestion that could help resolve the issue.",
			"Here's a step-by-step solution for dealing with {context}.",
			"Considering the {context} problem, try the following actions.",
			"Let's resolve the {context} issue. Here's what you should do first.",
			"To fix {context}, I've come up with a solution. Follow these steps."
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
			"Take your time to consider the options for {context}. I'm here to help.",
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
			"I'll make sure this gets to the right person for you.",
			"I'm escalating this issue right now, please hold on."
		}},
		{Category::Context, {
			"Given the issue with {context}, I'll escalate this to the right team.",
			"Your concern about {context} will be directed to the appropriate supervisor.",
			"I'm escalating the {context} issue to someone who can resolve it.",
			"For {context}, I'll bring in higher-level assistance.",
			"Your {context} issue deserves immediate escalation-working on it now.",
			"This {context} situation needs special attention. I'm escalating it now.",
			"Escalating the {context} issue to ensure it's handled quickly."
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

	m_SolutionSteps[ContextCategory::Windowfly] = {
		"Ensure the WindowFly's suction system is functioning properly.",
		"Clean the WindowFly's cleaning pads and check for wear or dirt buildup.",
		"Verify the window surface is free of debris or excessive grime.",
		"Check the battery charge level and ensure it's sufficient for the task.",
		"Ensure the WindowFly's software is updated to the latest version.",
		"Test the robot on a smaller window to ensure proper adhesion and cleaning.",
		"Inspect the WindowFly's edge detection sensors for proper calibration.",
		"Reattach or replace any loose or damaged cleaning components."
	};

	m_SolutionSteps[ContextCategory::Cleanbug] = {
		"Empty the dustbin and clean the filter to ensure proper airflow.",
		"Inspect the brushes and rollers for hair, debris, or blockages.",
		"Verify the floor type is supported by the CleanBug (e.g., carpet, tile, etc.).",
		"Ensure the charging dock and battery are functioning correctly.",
		"Check if the CleanBug's firmware is updated to the latest version.",
		"Inspect the obstacle detection sensors for dirt or misalignment.",
		"Test the robot in a small, obstacle-free area to verify operation.",
		"Clean the wheels and underside of the CleanBug to maintain mobility."
	};

	m_SolutionSteps[ContextCategory::Gardenbeetle] = {
		"Ensure the Garden Beetle's blades are properly installed and sharp.",
		"Check if the grass height exceeds the Garden Beetle's cutting capacity.",
		"Inspect the wheels and sensors for obstructions or damage.",
		"Verify the robot's boundary settings and adjust them if necessary.",
		"Ensure the Garden Beetle's firmware is up to date.",
		"Calibrate the sensors and ensure the lawn boundary wire (if applicable) is intact.",
		"Test the robot in a small area to confirm proper operation.",
		"Clean the Garden Beetle's undercarriage and blades after use."
	};

	m_SolutionSteps[ContextCategory::Refund] = {
		"Check the original payment receipt to verify the transaction details.",
		"Ensure the refund request complies with the company's return or refund policy.",
		"If the refund is for a defective product or service issue, document the problem (photos, descriptions, etc.).",
		"Contact the support team with the transaction ID, payment method, and reason for the refund.",
		"Follow any specific instructions provided by the support team, such as returning the product or providing additional details.",
		"Confirm the timeframe for the refund process (e.g., 5-10 business days).",
		"Monitor your bank account or payment method for the refunded amount within the expected period.",
		"If the refund does not arrive on time, escalate the issue to a supervisor or file a dispute with the payment provider."
	};

	m_SolutionSteps[ContextCategory::Default] = {
		"Restart the robot to reset its settings and clear minor glitches.",
		"Inspect the robot for visible damage or debris.",
		"Refer to the user manual for specific troubleshooting steps.",
		"Contact the manufacturer or a support center for further assistance.",
		"Update the robot's firmware to ensure optimal performance.",
		"Perform a factory reset if no other solutions work (refer to the manual)."
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

ContextCategory ResponseManager::CategorizeEntity(const std::string& entity)
{
	// Categorize the entity
	if (entity.find("refund") != std::string::npos)
		return ContextCategory::Refund;
	else if (entity.find("gardenbeetle") != std::string::npos)
		return ContextCategory::Gardenbeetle;
	else if (entity.find("windowfly") != std::string::npos)
		return ContextCategory::Windowfly;
	else if (entity.find("cleanbug") != std::string::npos)
		return ContextCategory::Cleanbug;

	// Default fallback category
	return ContextCategory::Default;
}

std::vector<std::string> ResponseManager::GetSolutionSteps(ContextCategory context)
{
	auto it = m_SolutionSteps.find(context);
	if (it != m_SolutionSteps.end())
	{
		return it->second; // Return context-specific steps
	}

	// Fallback to default steps if no match found
	return m_SolutionSteps[ContextCategory::Default];
}

std::vector<std::string> ResponseManager::SplitContext(const std::string& context)
{
	std::vector<std::string> entities;
	std::string remaining = context;

	// First, split by ", and" or " and " explicitly
	size_t pos = remaining.find(", and ");
	if (pos == std::string::npos)
		pos = remaining.find(" and ");

	if (pos != std::string::npos)
	{
		entities.push_back(remaining.substr(0, pos)); // Add the part before "and"
		remaining = remaining.substr(pos + (remaining[pos + 1] == 'a' ? 5 : 4)); // Skip ", and" or " and"
	}

	// Now, split the remaining string by ", "
	std::string delimiter = ", ";
	while ((pos = remaining.find(delimiter)) != std::string::npos)
	{
		entities.push_back(remaining.substr(0, pos));
		remaining.erase(0, pos + delimiter.length());
	}

	// Add any leftover part as the last entity
	if (!remaining.empty())
		entities.push_back(remaining);

	return entities;
}