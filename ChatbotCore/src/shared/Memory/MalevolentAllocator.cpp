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
#include "MalevolentAllocator.h"
#include "shared/Types.h"

#include <cassert>

namespace Raven
{
	// Domain Expansion: Malevolent Allocation. The dark force behind memory allocation.
	MalevolentAllocator::MalevolentAllocator(size_t chunkSize, size_t chunkCount) : m_ChunkSize(chunkSize), m_ChunkCount(chunkCount)
	{
		// Align chunk size for optimal memory access
		m_ChunkSize = chunkSize < sizeof(void*) ? sizeof(void*) : chunkSize;
		size_t alignedSize = (m_ChunkSize + alignof(std::max_align_t) - 1) & ~(alignof(std::max_align_t) - 1);

		m_Memory = ::operator new(alignedSize * chunkCount);

		// Use a contiguous array for freelist, which improves cache locality
		m_Freelist = m_Memory;

		// Init freelist
		void* current = m_Memory;
		void* end = static_cast<char*>(m_Memory) + (chunkSize * (chunkCount - 1));

		// Link all chunks in the freelist
		for (char* ptr = static_cast<char*>(current); ptr < static_cast<char*>(end); ptr += chunkSize)
		{
			*reinterpret_cast<void**>(ptr) = ptr + chunkSize;
		}
		*reinterpret_cast<void**>(static_cast<char*>(end)) = nullptr;
	}

	MalevolentAllocator::~MalevolentAllocator()
	{
		::operator delete(m_Memory);
	}

	// Allocate a chunk from the pool. The dark arts of memory management are at play here.
	void* MalevolentAllocator::Allocate()
	{
		assert(m_Freelist && "PoolAllocator out of Memory.");

		// Pull from the freelist and update the pointer to the next chunk
		void* chunk = m_Freelist;
		m_Freelist = *reinterpret_cast<void**>(chunk); // Fast pointer update
		return chunk;
	}

	// Deallocate chunk and return it to the freelist
	void MalevolentAllocator::Deallocate(void* ptr)
	{
		// Push the chunk back to the freelist, effectively creating a new free block at the front
		*reinterpret_cast<void**>(ptr) = m_Freelist;
		m_Freelist = reinterpret_cast<void**>(ptr);
	}
} // namespace Raven