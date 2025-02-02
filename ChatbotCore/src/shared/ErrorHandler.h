#pragma once

#include <iostream>        
#include <map>             
#include <string>          
#include "shared/Logger.h"        
#include "shared/Types.h"         


enum ErrorCode : Raven::uint8
{
	INVALID_INPUT,
	UNKNOWN_ERROR,
	INVALID_STATETRANSITION
};

class ErrorHandler
{
public:

	static const std::map<ErrorCode, std::string> errorMessages;

	static void LogError(ErrorCode code);

	static ErrorCode HandleCriticalError(ErrorCode code);
};