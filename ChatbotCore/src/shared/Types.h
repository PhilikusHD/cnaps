/*
* CrowWorks License 1.0
*
* Copyright (c) 2025 CrowWorks and Contributors
*
* By accessing, using, or distributing this software (the "Software"), you agree to abide by the following terms.
* This license is designed to foster open use and contributions while ensuring the integrity of the Software and its community.
*
* 1. Free Use, Modification, and Distribution
*    You are granted permission to use, modify, and distribute the Software, provided that:
*    - Any use, distribution, or modified versions of the Software must include this full license text, or a prominent reference to it.
*    - Attribution to the original authors and CrowWorks must be included in any copies or substantial portions of the Software, whether modified or unmodified.
*    - You must clearly identify if you have made modifications to the Software (we don’t want to play detective, do we?).
*
* 2. No Warranty, No Liability
*    The Software is provided "as is," with no express or implied warranties of any kind, including but not limited to warranties of merchantability, fitness for a particular purpose, or noninfringement.
*    In no event shall the authors or copyright holders be liable for any damages arising from the use or inability to use the Software, even if advised of the possibility of such damages.
*    By using or distributing the Software, you assume full responsibility for any potential risks, damages, or legal liabilities that may arise (don't say we didn’t warn you!).
*
* 3. Prohibited Uses
*    The Software may not be used for any purpose that:
*    - Violates applicable laws, regulations, or legal obligations (we're not in the business of getting you in trouble).
*    - Infringes the rights of others, including intellectual property rights (unless you enjoy being sued, in which case, proceed at your own peril).
*    - Promotes harmful, malicious, or exploitative purposes (e.g., unethical AI, exploitative microtransactions, ransomware, malware, or harmful surveillance) – basically, if you're planning on being a supervillain, look elsewhere.
*    - Encourages harm or illegal activity, or undermines the safety or security of others (don't be that guy).
*
* 4. Contributions and Improvements
*    Contributions to the Software are encouraged, as long as they align with the goals of the project:
*    - Fostering performance, scalability, optimization, and clean code (we’re not looking for spaghetti here, folks).
*    - Keeping the Software lightweight, efficient, and adaptable (no bloatware allowed).
*    Contributions should be submitted through a standard pull request process and must be compliant with this license. Contributions that do not meet these standards may be rejected at the discretion of the maintainers (don't worry, we won't bite... much).
*    Any submitted modifications or improvements become part of the Software and are covered by this license.
*
* 5. Redistribution of Modifications
*    Modifications to the Software are allowed and encouraged. However, any redistributed or modified version must be:
*    - Clearly labeled as a modified version of the original Software (we prefer our code to be transparent).
*    - Accompanied by this full license text, or a prominent reference to it.
*    You may not imply that your modified version is the original or officially endorsed by CrowWorks, unless specifically agreed upon (our approval comes with a stamp of *very official*).
*
* 6. Trademark Use
*    The name "CrowWorks" and any associated logos or trademarks may not be used to promote your derived works without explicit permission from CrowWorks (unless you have a cool logo and are willing to share).
*
* 7. Acknowledgements
*    The Software has been created as a result of the hard work of many individuals. CrowWorks values transparency and open contribution to ensure continuous improvement (we're just a bunch of code nerds, after all).
*
* By using, distributing, or contributing to the Software, you acknowledge that you have read, understood, and agree to comply with the terms of this license.
* If you do not agree with these terms, you must cease use, modification, or distribution of the Software (we won't hold it against you, promise).
*
* Signed,
* The CrowWorks Team
*/
#pragma once

namespace Raven
{
	// Basic integral types
	using int8 = signed char;
	using uint8 = unsigned char;
	using int16 = short;
	using uint16 = unsigned short;
	using int32 = int;
	using uint32 = unsigned int;
	using int64 = long long;
	using uint64 = unsigned long long;

	// Floating point types
	using float32 = float;
	using float64 = double;

	// Boolean type
	using bool8 = bool;

	// Size type (useful for sizes and indices)
	using size_t = unsigned long long;

	// A generic pointer type for objects
	using generic_ptr = void*;

	// 16-bit unsigned integer to store key codes (e.g., keyboard input)
	using Key = uint16;

	// Define uintptr_t as uint64 on a 64-bit architecture.
	// This type is used to store pointer values (addresses), ensuring it has
	// the same size as a pointer type, and is capable of safely holding memory
	// addresses on a 64-bit system.
	using uintptr = uint64;

	// Aligning types for specific memory layouts
	template<typename T>
	constexpr size_t AlignOf() noexcept
	{
		return alignof(T); // Returns alignment of the type T
	}
} // namespace Raven