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

#include "shared/Types.h"

#include "shared/Memory/MalevolentAllocator.h"
#include "shared/Memory/StackAllocator.h"
#include "shared/Memory/MemoryManager.h"
#include "shared/Memory/AllocatorTraits.h"

#include <utility>

namespace Raven
{
	enum class AllocatorType
	{
		None,
		Stack,
		Malevolent
	};

	template<typename T>
	class Scope
	{
	public:
		explicit Scope(T* ptr = nullptr) noexcept : m_Ptr(ptr) {}

		Scope(const Scope&) = delete;
		Scope& operator=(const Scope&) = delete;

		~Scope() { delete m_Ptr; }

		// Move constructor
		Scope(Scope&& other) noexcept
		{
			delete m_Ptr;
			m_Ptr = other.m_Ptr;
			other.m_Ptr = nullptr;
		}

		// Move assignment
		Scope& operator=(Scope&& other) noexcept
		{
			if (this != &other)
			{
				delete m_Ptr;
				m_Ptr = other.m_Ptr;
				other.m_Ptr = nullptr;
			}
			return *this;
		}

		T* Release() noexcept
		{
			T* temp = m_Ptr;
			m_Ptr = nullptr;
			return temp;
		}

		void Reset(T* ptr = nullptr) noexcept { m_Ptr = ptr; }

		T* Get() const noexcept { return m_Ptr; }
		T& operator*() const { return *m_Ptr; }
		T* operator->() const noexcept { return m_Ptr; }

		// Equality and Inequality Operators
		constexpr bool operator==(const Scope& other) const noexcept { return m_Ptr == other.m_Ptr; }
		constexpr bool operator!=(const Scope& other) const noexcept { return m_Ptr != other.m_Ptr; }
		constexpr bool operator==(const T* ptr) const noexcept { return m_Ptr == ptr; }
		constexpr bool operator!=(const T* ptr) const noexcept { return m_Ptr != ptr; }

		// Boolean Conversion Operator
		explicit constexpr operator bool() const noexcept { return m_Ptr != nullptr; }

		// Comparison Operators
		constexpr bool operator<(const Scope& other) const noexcept { return m_Ptr < other.m_Ptr; }
		constexpr bool operator<=(const Scope& other) const noexcept { return m_Ptr <= other.m_Ptr; }
		constexpr bool operator>(const Scope& other) const noexcept { return m_Ptr > other.m_Ptr; }
		constexpr bool operator>=(const Scope& other) const noexcept { return m_Ptr >= other.m_Ptr; }

		// Swap Operator
		constexpr friend void swap(Scope& first, Scope& second) noexcept
		{
			using std::swap;
			swap(first.m_Ptr, second.m_Ptr);
			swap(first.m_Allocator, second.m_Allocator);
			swap(first.m_AllocatorType, second.m_AllocatorType);
		}

	private:
		T* m_Ptr{ nullptr };
	};

	template<typename T, typename... Args>
	inline constexpr Scope<T> CreateScope(Args&&... args)
	{
		return Scope<T>(new T(std::forward<Args>(args)...));
	}

	template<typename T>
	class ScopeWithAllocator
	{
	public:
		// Regular constructor
		template<typename Allocator, typename... Args>
		explicit ScopeWithAllocator(Allocator* allocator, Args&&... args)
		{
			if constexpr (std::is_base_of_v<StackAllocator<T>, Allocator>)
			{
				m_AllocatorType = AllocatorType::Stack;
				m_Ptr = static_cast<T*>(allocator->Allocate(1));
			}
			else if constexpr (std::is_base_of_v<MalevolentAllocator, Allocator>)
			{
				m_AllocatorType = AllocatorType::Malevolent;
				m_Ptr = static_cast<T*>(allocator->Allocate());
			}
			else
			{
				static_assert(false, "Unsupported allocator type.");
			}
			new (m_Ptr) T(std::forward<Args>(args)...);
			m_Allocator = allocator;
		}

		// Move constructor
		ScopeWithAllocator(ScopeWithAllocator&& other) noexcept
		{
			delete m_Ptr;
			m_Ptr = other.m_Ptr;
			other.m_Ptr = nullptr;

			m_Allocator = other.m_Allocator;
			other.m_Allocator = nullptr;
		}

		// Move assignment operator
		ScopeWithAllocator& operator=(ScopeWithAllocator&& other) noexcept
		{
			if (this != &other)
			{
				this->~ScopeWithAllocator();
				delete m_Ptr;
				m_Ptr = other.m_Ptr;
				other.m_Ptr = nullptr;

				m_Allocator = other.m_Allocator;
				other.m_Allocator = nullptr;
			}
			return *this;
		}

		// Destructor
		~ScopeWithAllocator()
		{
			auto deallocate = [this] {
				if (m_AllocatorType == AllocatorType::Stack)
				{
					// StackAllocator doesn't need deallocation
				}
				else if (m_AllocatorType == AllocatorType::Malevolent)
				{
					static_cast<MalevolentAllocator*>(m_Allocator)->Deallocate(m_Ptr);
				}
				};

			if (m_Ptr != nullptr)
			{
				deallocate(); // Call the lambda
			}
		}

		// Accessor functions
		T* Get() const noexcept { return m_Ptr; }
		T& operator*() const { return *m_Ptr; }
		T* operator->() const noexcept { return m_Ptr; }

	private:
		T* m_Ptr{ nullptr };
		void* m_Allocator{ nullptr };
		AllocatorType m_AllocatorType{ AllocatorType::None }; // Store the allocator type
	};

	template<typename T, typename... Args>
	ScopeWithAllocator<T> CreateScopeWithAllocator(size_t objectCount = 0, Args&&... args)
	{
		if constexpr (AllocatorTraits<T>::UseStack)
		{
			size_t stackSize = sizeof(T) * objectCount; // or some heuristic based on the number of objects
			static StackAllocator<T>& stackAllocator = MemoryManager::Get().GetStackAllocator<T>(stackSize);
			return ScopeWithAllocator<T>(&stackAllocator, std::forward<Args>(args)...);
		}
		else if constexpr (AllocatorTraits<T>::UsePool)
		{
			size_t poolSize = sizeof(T) * objectCount; // again, dynamic size based on object count
			static MalevolentAllocator& malevolentAllocator = MemoryManager::Get().GetMalevolentAllocator(poolSize, objectCount);
			return ScopeWithAllocator<T>(&malevolentAllocator, std::forward<Args>(args)...);
		}
	}

	template<typename T, typename Allocator, typename... Args>
	ScopeWithAllocator<T> CreateScopeWithAllocator(Allocator* allocator, Args&&... args)
	{
		if constexpr (std::is_base_of_v<StackAllocator<T>, Allocator>)
		{
			return ScopeWithAllocator<T>(static_cast<StackAllocator<T>*>(allocator), std::forward<Args>(args)...);
		}
		else if constexpr (std::is_base_of_v<MalevolentAllocator, Allocator>)
		{
			return ScopeWithAllocator<T>(static_cast<MalevolentAllocator*>(allocator), std::forward<Args>(args)...);
		}
		else
		{
			static_assert(false, "Unsupported allocator type.");
		}
	}
} // namespace Raven