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

#include <atomic>
#include <ostream>
#include <utility>

namespace Raven
{
    class RefCounted
    {
      public:
        explicit RefCounted() : m_RefCount(0) {}

        // Increment the reference count.
        void AddRef() { m_RefCount.fetch_add(1); }

        // Decrement the reference count and delete if ref count reaches 0.
        void Release()
        {
            int currentRefCount = m_RefCount.fetch_sub(1);
            if (currentRefCount == 1)
            {
                delete this;
            }
        }

        [[nodiscard]] int GetRefCount() const { return m_RefCount.load(); }

      private:
        std::atomic_int m_RefCount;
    };

    template<typename T>
    class Ref
    {
      public:
        explicit Ref(T* ptr = nullptr) noexcept : m_Ptr(ptr)
        {
            if (m_Ptr)
            {
                m_Ptr->AddRef();
            }
        }

        // Copy constructor: Increment the ref count of the copied object
        Ref(const Ref& other) noexcept : m_Ptr(other.m_Ptr)
        {
            if (m_Ptr)
            {
                m_Ptr->AddRef();
            }
        }

        // Copy assignment operator: Release the current object, then copy
        Ref& operator=(const Ref& other) noexcept
        {
            if (this != &other)
            {
                if (m_Ptr)
                {
                    m_Ptr->Release(); // Avoid releasing a null pointer.
                }
                m_Ptr = other.m_Ptr;
                if (m_Ptr)
                {
                    m_Ptr->AddRef();
                }
            }
            return *this;
        }

        // Move Constructor
        Ref(Ref&& other) noexcept : m_Ptr(std::exchange(other.m_Ptr, nullptr)) {}

        // Move Assignment
        Ref& operator=(Ref&& other) noexcept
        {
            if (this != &other)
            {
                swap(other);
            }
            return *this;
        }

        ~Ref()
        {
            if (m_Ptr)
            {
                m_Ptr->Release();
            }
        }

        T* operator->() const { return m_Ptr; }
        T& operator*() const { return *m_Ptr; }

        bool operator==(const Ref& other) const noexcept { return m_Ptr == other.m_Ptr; }

        bool operator!=(const Ref& other) const noexcept { return m_Ptr != other.m_Ptr; }
        explicit operator bool() const noexcept { return m_Ptr != nullptr; }

        friend std::ostream& operator<<(std::ostream& os, const Ref& ref)
        {
            os << (ref ? *ref : "null");
            return os;
        }

        void swap(Ref& other) noexcept { std::swap(m_Ptr, other.m_Ptr); }

      private:
        T* m_Ptr;
    };

    // CreateRef for easy instantiation of Ref objects
    template<typename T, typename... Args>
    Ref<T> CreateRef(Args&&... args)
    {
        return Ref<T>(new T(std::forward<Args>(args)...));
    }
} // namespace Raven
