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

#include <typeindex>
#include <unordered_map>
#include "shared/Memory/MalevolentAllocator.h"
#include "shared/Memory/StackAllocator.h"

namespace Raven
{
	class MemoryManager
	{
	public:
		static MemoryManager& Get()
		{
			static MemoryManager instance;
			return instance;
		}

		MalevolentAllocator& GetMalevolentAllocator(size_t chunkSize, size_t chunkCount);

		template<typename T>
		StackAllocator<T>& GetStackAllocator(size_t elementCount)
		{
			size_t key = GenerateKey<T>(elementCount);
			auto it = m_StackAllocators.find(key);
			if (it != m_StackAllocators.end())
			{
				return *reinterpret_cast<StackAllocator<T>*>(it->second);
			}

			// Create a new StackAllocator and store it in the map
			auto* allocator = new StackAllocator<T>(elementCount);
			m_StackAllocators[key] = allocator;
			return *allocator;
		}

	private:
		MemoryManager() = default;
		~MemoryManager();

		size_t GenerateKey(size_t chunkSize, size_t chunkCount) { return (chunkSize << 32) | chunkCount; }
		template<typename T>
		size_t GenerateKey(size_t elementCount)
		{
			// Combine type hash and element count into a single key
			return (std::hash<std::type_index>()(typeid(T)) ^ (elementCount << 1));
		}

		std::unordered_map<size_t, MalevolentAllocator*> m_Pools;
		std::unordered_map<size_t, void*> m_StackAllocators;
	};
} // namespace Raven