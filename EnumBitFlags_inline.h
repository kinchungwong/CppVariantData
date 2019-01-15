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

template <typename enum_type>
inline typename enum_bit_flags::enum_type_traits<enum_type>::enum_type operator | (const enum_type& arg0, const enum_type& arg1)
{
    using val_type = typename enum_bit_flags::enum_type_traits<enum_type>::underlying_type;
    val_type mask = static_cast<val_type>(enum_bit_flags::enum_type_traits<enum_type>::mask_value);
    val_type val0 = static_cast<val_type>(arg0);
    val_type val1 = static_cast<val_type>(arg1);
    return static_cast<enum_type>((val0 | val1) & mask);
}

template <typename enum_type>
inline typename enum_bit_flags::enum_type_traits<enum_type>::enum_type operator & (const enum_type& arg0, const enum_type& arg1)
{
    using val_type = typename enum_bit_flags::enum_type_traits<enum_type>::underlying_type;
    val_type mask = static_cast<val_type>(enum_bit_flags::enum_type_traits<enum_type>::mask_value);
    val_type val0 = static_cast<val_type>(arg0);
    val_type val1 = static_cast<val_type>(arg1);
    return static_cast<enum_type>((val0 & val1) & mask);
}

template <typename enum_type>
inline typename enum_bit_flags::enum_type_traits<enum_type>::enum_type operator ^ (const enum_type& arg0, const enum_type& arg1)
{
    using val_type = typename enum_bit_flags::enum_type_traits<enum_type>::underlying_type;
    val_type mask = static_cast<val_type>(enum_bit_flags::enum_type_traits<enum_type>::mask_value);
    val_type val0 = static_cast<val_type>(arg0);
    val_type val1 = static_cast<val_type>(arg1);
    return static_cast<enum_type>((val0 ^ val1) & mask);
}

template <typename enum_type>
inline typename enum_bit_flags::enum_type_traits<enum_type>::enum_type operator ~ (const enum_type& arg0)
{
    using val_type = typename enum_bit_flags::enum_type_traits<enum_type>::underlying_type;
    val_type mask = static_cast<val_type>(enum_bit_flags::enum_type_traits<enum_type>::mask_value);
    val_type val0 = static_cast<val_type>(arg0);
    return static_cast<enum_type>((~val0) & mask);
}

// typename enum_type_traits<enum_type>::enum_type 
template <typename enum_type>
inline typename enum_bit_flags::enum_type_traits<enum_type>::bool_type operator ! (const enum_type& arg0)
{
    using val_type = typename enum_bit_flags::enum_type_traits<enum_type>::underlying_type;
    using bool_type = typename enum_bit_flags::enum_type_traits<enum_type>::bool_type;
    val_type mask = static_cast<val_type>(enum_bit_flags::enum_type_traits<enum_type>::mask_value);
    val_type val0 = static_cast<val_type>(arg0);
    return static_cast<bool_type>(!(val0 & mask));
}

template <typename enum_type>
inline typename enum_bit_flags::enum_type_traits<enum_type>::enum_type& operator &= (enum_type& argOut0, const enum_type& arg1)
{
    argOut0 = argOut0 & arg1;
    return argOut0;
}

template <typename enum_type>
inline typename enum_bit_flags::enum_type_traits<enum_type>::enum_type& operator |= (enum_type& argOut0, const enum_type& arg1)
{
    argOut0 = argOut0 | arg1;
    return argOut0;
}

template <typename enum_type>
inline typename enum_bit_flags::enum_type_traits<enum_type>::enum_type& operator ^= (enum_type& argOut0, const enum_type& arg1)
{
    argOut0 = argOut0 ^ arg1;
    return argOut0;
}

template <typename enum_type>
inline typename enum_bit_flags::enum_type_traits<enum_type>::bool_type validate(
    const enum_type& actual, const enum_type& required,
    const enum_type& allowed = enum_bit_flags::enum_type_traits<enum_type>::mask_value)
{
    return !(actual & ~allowed) &&
        ((actual & required) == required);
}
