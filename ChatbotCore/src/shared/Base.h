#pragma once


// Detect the platform
#if !defined(_WIN32)
#error "This header is intended for Windows DLLs only!"
#endif

// Define import/export macros
#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

// Disable annoying warnings
#pragma warning(disable : 4251) // 'identifier': class 'type' needs to have dll-interface