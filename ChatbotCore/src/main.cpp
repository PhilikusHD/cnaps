#include "shared/StringUtils.h"
#include "shared/Logger.h"
#include "shared/RandomUtils.h"
#include "shared/Utils.h"

int main()
{
	// String utility demo
	std::string input = "   Hello World!   ";
	std::cout << "Trimmed: '" << StringUtils::Trim(input) << "'\n";

	// Logging demo
	Logger::Log("Application started.", Logger::Level::INFO);

	// Random utility demo
	std::vector<int> nums = { 1, 2, 3, 4, 5 };
	RandomUtils::Shuffle(nums);
	for (int n : nums)
	{
		std::cout << n << " ";
	}
	std::cout << "\nRandom Element: " << RandomUtils::RandomElement(nums) << "\n";

	// General utility demo
	Utils::MeasureExecutionTime("Heavy Task", []() {
		for (int i = 0; i < 100000000; ++i); // Dummy work
		});

	return 0;
}
