#pragma once
#include "shared/Types.h"

enum class State : Raven::uint8
{
	Greeting,
	ProblemDesc,
	Consideration,
	Escalation,
	Goodbye
};