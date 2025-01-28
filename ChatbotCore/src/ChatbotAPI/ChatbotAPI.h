#include "shared/Base.h"

extern "C"
{
	DLL_EXPORT void InitializeChatbot();
	DLL_EXPORT void ShutdownChatbot();
	DLL_EXPORT const char* GenerateResponse(const char* input);
	DLL_EXPORT const bool GetStatus();
	DLL_EXPORT const char* GetError();
	DLL_EXPORT void FreeResponse(char* response);
}