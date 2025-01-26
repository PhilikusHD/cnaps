/*
 * CrowWorks License 1.0
 * 
 * Copyright (c) 2025 CrowWorks and Contributors
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to use, modify, and distribute the Software,
 * subject to the following conditions:
 * 
 * 1. Attribution
 *    Any use of the Software, whether modified or unmodified, must include this license in all
 *    copies or substantial portions of the Software. Additionally, visible credit must be given
 *    to CrowWorks in associated documentation or in the end product.
 * 
 * 2. No Warranty
 *    The Software is provided "as is," without any warranty of any kind, express or implied,
 *    including but not limited to the warranties of merchantability, fitness for a particular
 *    purpose, and noninfringement. In no event shall the authors or copyright holders be liable
 *    for any claim, damages, or other liability, whether in an action of contract, tort, or
 *    otherwise, arising from, out of, or in connection with the Software or the use or other
 *    dealings in the Software.
 * 
 * 3. Prohibited Uses
 *    The Software may not be used for any purposes that violate applicable laws or infringe on
 *    the rights of others. Use of the Software in malicious or exploitative projects (e.g.,
 *    unethical AI, exploitative microtransactions, or harmful software) is strictly prohibited.
 * 
 * 4. Redistribution of Modifications
 *    Modifications to the Software are permitted, provided they are distributed under this same
 *    license. Sharing improvements is encouraged to benefit the community.
 * 
 * 5. Contribution Guidelines
 *    Contributions to this Software are welcome. Contributions should align with the Software's
 *    goals of performance, clean code, and optimization. Submissions that do not meet these
 *    standards may be rejected.
 * 
 * By using or contributing to the Software, you agree to these terms and conditions. If you do
 * not agree, you must refrain from using, modifying, or distributing the Software.
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