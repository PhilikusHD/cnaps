#pragma once
#include "shared/Types.h"

enum class State : Raven::uint8
{
	Greeting,
	ProblemDesc,
	Solution,
	Consideration,
	Escalation,
	Goodbye
};

enum class ContextCategory : Raven::uint8
{
	Refund,
	Gardenbeetle,
	Windowfly,
	Cleanbug,
	Default
};