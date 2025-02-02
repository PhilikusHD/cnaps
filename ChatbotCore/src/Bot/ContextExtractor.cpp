#include "ContextExtractor.h"
#include <sstream>
#include "shared/Types.h"
#include <unordered_set>
#include <algorithm>
#include "States.h"

ContextExtractor::ContextExtractor()
{
	InitEntityKeywords();
}

std::string ContextExtractor::ExtractContext(const std::string& input)
{
	std::unordered_map<ContextCategory, std::string> selectedEntities; // Use the enum for categories
	std::unordered_map<ContextCategory, int> matchScores; // To prioritize matches

	// Match against entity keywords
	for (const auto& [category, keywords] : m_EntityKeywords)
	{
		for (const auto& keyword : keywords)
		{
			// Check if the keyword is found in the input
			size_t position = input.find(keyword);
			if (position != std::string::npos)
			{
				// Calculate relevance score (e.g., position in text, length of match)
				int score = static_cast<int>(keyword.length()) - static_cast<int>(position);

				// Add or update the best match for this category
				if (selectedEntities.find(category) == selectedEntities.end() || score > matchScores[category])
				{
					selectedEntities[category] = keyword;
					matchScores[category] = score;
				}
			}
		}
	}

	if (!selectedEntities.empty())
	{
		// Use the enhanced BuildContextString to format the output
		return BuildContextString(selectedEntities);
	}

	return "";
}

std::string ContextExtractor::BuildContextString(const std::unordered_map<ContextCategory, std::string>& entities)
{
	std::vector<std::string> contextList;

	// Convert categories to readable strings and build the context
	for (const auto& [category, keyword] : entities)
	{
		// Get the string representation of the category
		std::string categoryStr = GetCategoryString(category);
		contextList.emplace_back(categoryStr);
	}

	if (contextList.empty())
		return "";

	// Make the list more natural (e.g., "bug, floor, and cleanbug")
	std::string result;
	for (size_t i = 0; i < contextList.size(); ++i)
	{
		if (i > 0 && i == contextList.size() - 1)
			result += " and "; // Last item
		else if (i > 0)
			result += ", "; // Separator

		result += contextList[i];
	}

	return result;
}

std::string ContextExtractor::GetCategoryString(ContextCategory category)
{
	// Return a string representation of the category
	switch (category)
	{
	case ContextCategory::Refund:
		return "refund";
	case ContextCategory::Gardenbeetle:
		return "gardenbeetle";
	case ContextCategory::Windowfly:
		return "windowfly";
	case ContextCategory::Cleanbug:
		return "cleanbug";
	default:
		return "default";
	}
}

void ContextExtractor::InitEntityKeywords()
{
	m_EntityKeywords = {
		// Refund-related
		{ContextCategory::Refund, {
			"money", "money back", "refund"
		}},

		// Bugs and errors (related to gardenbeetle)
		{ContextCategory::Gardenbeetle, {
			"gardenbeetle", "garden beetle", "lawn", "grass", "wheat"
		}},

		// Cleanbug-related (floor or ground related)
		{ContextCategory::Cleanbug, {
			"cleanbug", "clean bug" ,"floor", "tile", "ground", "surface", "deck", "carpet",
			"hardwood", "laminate", "vinyl", "parquet", "marble", "ceramic",
			"granite", "concrete", "plank", "rug", "mat", "linoleum", "base",
		}},

		// Windowfly-related (window and glass)
		{ContextCategory::Windowfly, {
			"window", "pane", "glass", "windowfly", "clean window", "window cleaner", "window fly"
		}}
	};
}