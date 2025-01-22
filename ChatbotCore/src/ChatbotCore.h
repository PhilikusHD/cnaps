#pragma once

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif // _WIN32


extern "C"
{
	EXPORT const char* GetGreeting();
}