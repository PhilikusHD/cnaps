#include "ErrorHandler.h"

const std::map<ErrorCode, std::string> errorMessages;

void ErrorHandler::logError(ErrorCode code)
{
	auto iterator = errorMessages.find(code);
	if (iterator != errorMessages.end())
	{
		Logger::Log("Error [" + std::to_string(static_cast<int>(code)) + "]: " + iterator->second, Logger::Level::ERROR);
	}
	else
	{
		std::cerr << "Error [" << code << "]: Unknown error occurred." << std::endl;
	}
}

ErrorCode ErrorHandler::handleCriticalError(ErrorCode code)
{
	logError(code);
	return code;
}
const std::map<ErrorCode, std::string> ErrorHandler::errorMessages =
{
	{ INVALID_INPUT, "Invalid input" },
	{ UNKNOWN_ERROR, "Unknown error" },
	{ INVALID_STATETRANSITION, "Invalid Statetransition" }
};
