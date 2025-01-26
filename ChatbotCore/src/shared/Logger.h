#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

namespace Logger
{
	enum class Level { INFO, WARNING, ERROR };

	void Log(const std::string& message, Level level = Level::INFO)
	{
		const char* levelStr = (level == Level::INFO) ? "INFO" :
			(level == Level::WARNING) ? "WARNING" : "ERROR";

		// Get the current timestamp
		std::time_t now = std::time(nullptr);
		char timestamp[20];
		std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

		// Print to console
		std::cout << "[" << timestamp << "] [" << levelStr << "] " << message << std::endl;

		// Write to a log file, can remove or not idc
		std::ofstream logFile("app.log", std::ios::app);
		logFile << "[" << timestamp << "] [" << levelStr << "] " << message << std::endl;
	}
}
