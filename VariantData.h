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
#include "EnumBitFlags.h"

namespace variant_data
{
    enum class Access
    {
        AssignPut = 1,
        AssignGet = 2,
        ClonePut = 4,
        CloneGet = 8,
        RefConst = 16,
        RefMut = 32,
        ShareConst = 64,
        ShareMut = 128,
        Replace = 256,
        ChangeType = 512,
        RestrictFlags = 1024,
        UnrestrictFlags = 2048,
        None = 0,
        All = (4096 - 1)
    };
}

namespace enum_bit_flags
{
    template <>
    struct enum_type_traits<::variant_data::Access>
    {
        using enum_type = typename ::variant_data::Access;
        using underlying_type = typename ::std::underlying_type<::variant_data::Access>::type;
        using bool_type = bool;
        static const enum_type default_value = ::variant_data::Access::None;
        static const enum_type mask_value = ::variant_data::Access::All;
    };
}

namespace variant_data
{
    class VariantBase
    {
    protected:
        std::shared_ptr<void> m_ptr;
        std::type_index m_type;
        Access m_access;

    protected:
        VariantBase();
        ~VariantBase();
        VariantBase(const VariantBase&) = default;
        VariantBase(VariantBase&&) = default;
        VariantBase& operator= (VariantBase&&) = default;
    };

    class VariantData
        : private VariantBase
    {
    private:
        using VariantBase::m_ptr;
        using VariantBase::m_type;
        using VariantBase::m_access;

    public:
        VariantData();
        ~VariantData();
        VariantData(const VariantData&) = default;
        VariantData(VariantData&&) = default;
        VariantData& operator= (VariantData&&) = default;

    public:
        template <typename T> void AssignPut(const T& newValue);
        template <typename T> void AssignPut(T&& newValue);
        template <typename T> void AssignGet(T& output) const;
        template <typename T> void ClonePut(const T& newValue);
        template <typename T> void ClonePut(T&& newValue);
        template <typename T> std::shared_ptr<T> CloneGet() const;
        template <typename T> const T& RefConst() const;
        template <typename T> T& RefMut() const;
        template <typename T> std::shared_ptr<const T> ShareConst() const;
        template <typename T> std::shared_ptr<T> ShareMut() const;
        template <typename T> std::shared_ptr<T> Replace(const std::shared_ptr<T>& newPtr);
        template <typename T> std::shared_ptr<T> Replace(std::shared_ptr<T>&& newPtr);
        template <typename T> void ChangeType(const std::shared_ptr<T>& newPtr, Access newAccess);
        template <typename T> void ChangeType(const std::shared_ptr<T>& newPtr);
        template <typename T> void ChangeType(std::shared_ptr<T>&& newPtr, Access newAccess);
        template <typename T> void ChangeType(std::shared_ptr<T>&& newPtr);
        void ChangeFlags(Access newAccess);

        // Emplace is a variation of Access::ClonePut, where a new instance is created
        // via std::make_shared, and constructor arguments are forwarded.
        template <typename T, class... Args> void Emplace(Args&&... args);

    public:
        Access GetFlags() const;
        bool HasValue() const;
        std::type_index GetType() const;

    private:
        template <typename T> void AssertTypeAndAccess(Access access, const char* msg) const;
        void AssertAccess(Access access, const char* msg) const;
    };
}

namespace variant_data
{
    struct permission_error
        : public std::logic_error
    {
    public:
        explicit permission_error(const std::string& what_arg)
            : std::logic_error(what_arg)
        {
        }

        explicit permission_error(const char* what_arg)
            : std::logic_error(what_arg)
        {
        }
    };
}
