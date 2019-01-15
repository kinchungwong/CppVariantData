/*

MIT License

Copyright (c) 2019 Ryan Wong

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#pragma once
#include "VariantData.h"
#include "EnumBitFlags_inline.h"

namespace variant_data
{
    template <typename T>
    void VariantData::AssignPut(const T& newValue)
    {
        AssertTypeAndAccess<T>(Access::AssignPut, "AssignPut");
        *static_cast<T*>(m_ptr.get()) = newValue;
    }

    template <typename T>
    void VariantData::AssignPut(T&& newValue)
    {
        AssertTypeAndAccess<T>(Access::AssignPut, "AssignPut");
        *static_cast<T*>(m_ptr.get()) = std::move(newValue);
    }

    template <typename T>
    void VariantData::AssignGet(T& output) const
    {
        AssertTypeAndAccess<T>(Access::AssignGet, "AssignGet");
        const T& t = *static_cast<const T*>(m_ptr.get());
        output = t;
    }

    template <typename T>
    void VariantData::ClonePut(const T& newValue)
    {
        AssertTypeAndAccess<T>(Access::ClonePut, "ClonePut");
        m_ptr = std::make_shared<T>(newValue);
    }

    template <typename T>
    void VariantData::ClonePut(T&& newValue)
    {
        AssertTypeAndAccess<T>(Access::ClonePut, "ClonePut");
        m_ptr = std::make_shared<T>(std::move(newValue));
    }

    template <typename T, class... Args> 
    void VariantData::Emplace(Args&&... args)
    {
        AssertTypeAndAccess<T>(Access::ClonePut, "Emplace requires ClonePut");
        m_ptr = std::make_shared<T>(std::forward<args>...);
    }

    template <typename T>
    std::shared_ptr<T> VariantData::CloneGet() const
    {
        AssertTypeAndAccess<T>(Access::CloneGet, "CloneGet");
        const T& t = *static_cast<const T*>(m_ptr.get());
        return std::shared_ptr<T>(t);
    }

    template <typename T>
    const T& VariantData::RefConst() const
    {
        AssertTypeAndAccess<T>(Access::RefConst, "RefConst");
        const T& t = *static_cast<const T*>(m_ptr.get());
        return t;
    }

    template <typename T>
    T& VariantData::RefMut() const
    {
        AssertTypeAndAccess<T>(Access::RefMut, "RefMut");
        T& t = *static_cast<T*>(m_ptr.get());
        return t;
    }

    template <typename T>
    std::shared_ptr<const T> VariantData::ShareConst() const
    {
        AssertTypeAndAccess<T>(Access::ShareConst, "ShareConst");
        return std::shared_ptr<const T>(m_ptr);
    }

    template <typename T>
    std::shared_ptr<T> VariantData::ShareMut() const
    {
        AssertTypeAndAccess<T>(Access::ShareMut, "ShareMut");
        return std::shared_ptr<T>(m_ptr);
    }

    template <typename T> 
    std::shared_ptr<T> VariantData::Replace(const std::shared_ptr<T>& newPtr)
    {
        AssertTypeAndAccess<T>(Access::Replace, "Replace");
        std::shared_ptr<T> oldPtr(std::move(m_ptr));
        m_ptr = newPtr;
        return oldPtr;
    }

    template <typename T> 
    std::shared_ptr<T> VariantData::Replace(std::shared_ptr<T>&& newPtr)
    {
        AssertTypeAndAccess<T>(Access::Replace, "Replace");
        std::shared_ptr<T> oldPtr(std::move(m_ptr));
        m_ptr = std::move(newPtr);
        return oldPtr;
    }

    template <typename T>
    void VariantData::ChangeType(const std::shared_ptr<T>& newPtr, Access newAccess)
    {
        // If ChangeType is allowed, it can be replaced with a shared_ptr 
        // of the same type or a different type.
        AssertAccess(Access::ChangeType, "ChangeType");
        m_type = std::type_index(typeid(T));
        m_ptr = newPtr;
        m_access = newAccess;
    }

    template <typename T>
    void VariantData::ChangeType(const std::shared_ptr<T>& newPtr)
    {
        ChangeType(newPtr, m_access);
    }

    template <typename T>
    void VariantData::ChangeType(std::shared_ptr<T>&& newPtr, Access newAccess)
    {
        // If ChangeType is allowed, it can be replaced with a shared_ptr 
        // of the same type or a different type.
        AssertAccess(Access::ChangeType, "ChangeType");
        m_type = std::type_index(typeid(T));
        m_ptr = std::move(newPtr);
        m_access = newAccess;
    }

    template <typename T>
    void VariantData::ChangeType(std::shared_ptr<T>&& newPtr)
    {
        ChangeType(newPtr, m_access);
    }

    inline void VariantData::ChangeFlags(Access newAccess)
    {
        const bool hasRestrict = !!(m_access & ~newAccess);
        const bool hasUnrestrict = !!(newAccess & ~m_access);
        if (hasRestrict)
        {
            AssertAccess(Access::RestrictFlags, "RestrictFlags");
        }
        if (hasUnrestrict)
        {
            AssertAccess(Access::UnrestrictFlags, "UnrestrictFlags");
        }
        if (hasRestrict || hasUnrestrict)
        {
            m_access = newAccess;
        }
    }

    inline Access VariantData::GetFlags() const
    {
        return m_access;
    }

    inline bool VariantData::HasValue() const
    {
        return (m_ptr.operator bool)();
    }

    inline std::type_index VariantData::GetType() const
    {
        return m_type;
    }

    template <typename T> 
    void VariantData::AssertTypeAndAccess(Access access, const char* msg) const
    {
        if (std::type_index(typeid(T)) != m_type)
        {
            throw std::bad_cast(msg);
        }
        if (std::type_index(typeid(T)) != m_type ||
            !validate(m_access, access))
        {
            throw permission_error(msg);
        }
    }

    inline void VariantData::AssertAccess(Access access, const char* msg) const
    {
        if (!validate(m_access, access))
        {
            throw permission_error(msg);
        }
    }
}
