#include "ContextExtractor.h"
#include <sstream>
#include "shared/Types.h"
#include <unordered_set>
#include <algorithm>

ContextExtractor::ContextExtractor()
{
	InitEntityKeywords();
}

std::string ContextExtractor::ExtractContext(const std::string& input)
{
	std::unordered_map<std::string, std::string> selectedEntities;

	// Match against entity keywords
	for (const auto& [entity, keywords] : m_EntityKeywords)
	{
		for (const auto& keyword : keywords)
		{
			if (input.find(keyword) != std::string::npos)
			{
				// Add only the first match from each entity category
				if (selectedEntities.find(entity) == selectedEntities.end())
				{
					selectedEntities[entity] = keyword;
				}
				break;
			}
		}
	}

	if (!selectedEntities.empty())
	{
		return BuildContextString(selectedEntities);
	}

	return "";
}

std::string ContextExtractor::BuildContextString(const std::unordered_map<std::string, std::string>& entities)
{
	std::vector<std::string> contextList;

	for (const auto& [category, keyword] : entities)
	{
		contextList.push_back(keyword);
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


void ContextExtractor::InitEntityKeywords()
{
	m_EntityKeywords = {
		// Bugs and errors
		{"bug", {
			"error", "glitch", "malfunction", "problem", "fault",
			"defect", "flaw", "hitch", "snag", "anomaly", "quirk", "breakage",
			"imperfection", "mishap", "oversight", "irregularity", "failure",
			"mistake", "blunder", "shortcoming"
		}},

		// Floors, tiles, and ground-related
		{"floor", {
			"floor", "tile", "ground", "scratch", "surface", "deck", "carpet",
			"hardwood", "laminate", "vinyl", "parquet", "marble", "ceramic",
			"granite", "concrete", "plank", "rug", "mat", "linoleum", "base",
			"pavement", "stone", "floorboard", "brick", "asphalt", "glass", "grass",
			"window", "pane", "wheats"
		}},

		// Damage-related terms
		{"damage", {
			"damage", "broken", "scratch", "crack", "fault", "ruined", "fracture",
			"shatter", "destroy", "impair", "weaken", "bust", "split", "chip",
			"dent", "corrode", "wrecked", "mar", "blemish", "erode", "gouge",
			"degrade", "torn", "split", "scuff", "scar", "wear", "tear",
			"deform", "impact", "debris", "fragment"
		}},


		// Hardware and devices
		{"hardware", {
			"hardware", "device", "equipment", "machine", "gadget", "component",
			"tool", "appliance", "gear", "instrument", "unit", "mechanism",
			"contraption", "processor", "chip", "controller", "peripheral", "server",
			"sensor", "windowfly", "garden beetle", "clean bug", "window fly", "cleanbug",
			"gardenbeetle"
		}}
	};
}