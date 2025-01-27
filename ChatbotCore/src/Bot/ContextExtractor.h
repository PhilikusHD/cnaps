#pragma once
#include <string>
#include <unordered_map>
#include <vector>

class ContextExtractor
{
public:
	ContextExtractor();
	// Extract context from preprocessed input
	std::string ExtractContext(const std::string& input);
private:
	// Entity-to-keyword map
	std::unordered_map<std::string, std::vector<std::string>> m_EntityKeywords;

	void InitEntityKeywords();
	std::string BuildContextString(const std::unordered_map<std::string, std::string>& entities);
};