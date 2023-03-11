/*
Copyright (c) 2023 Emerson Clarke <ZeroToHero>

MIT License

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

#include "hero/assert.h"
#include "hero/structure.h"

#ifdef HERO_USING_STD
#include <string>
#include <array>
#include <vector>
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Kind_, typename _Size_=int>
class Slice 
{
public:

    _Kind_ * Data;
    _Size_ Size;

    Slice():Data(0),Size(0) {}

    #ifdef HERO_USING_STD

    Slice(std::vector<_Kind_> & v):Data(&*v.begin()),Size(v.size()) {}
    Slice(_Kind_* data, _Size_ size):Data(data),Size(size) {}
    #endif

    Slice(Array<_Kind_> & array):Data(array.Data),Size(array.Size) {}
    Slice(Vector<_Kind_> & vector):Data(vector.Data),Size(vector.Size) {}

    Slice(const Slice&) = default;
    Slice& operator=(const Slice&) = default;

    _Kind_ operator[](int index) const {
        Assert(index < Size);
        return Data[index];
    }

    virtual Slice & Clear() {Data=0;Size=0;return *this;}
    bool IsEmpty() const {return Data == 0 || Size == 0;}
    bool IsNull() const {return Data == 0;}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
class Slice<char,int>
{
public:

    char * Data;
    int Size;

    explicit Slice():Data(0),Size(0) {}

    #ifdef HERO_USING_STD

    explicit Slice(std::vector<char> & v):Data(&*v.begin()),Size(v.size()) {}
    explicit Slice(const std::string& s):Data((char*)s.data()),Size((int)s.size()) {}
    #endif

    explicit Slice(Array<char> & array):Data(array.Data),Size(array.Size) {}
    explicit Slice(Vector<char> & vector):Data(vector.Data),Size(vector.Size) {}

    explicit Slice(char* data, int size):Data(data),Size(size) {}
    explicit Slice(const char* data):Data((char*)data),Size((int)Length(data)) {}

    static int Length(const char * data) 
    {
        return (data)?(int)strlen(data):0;
    }

    Slice(const Slice&) = default;
    Slice& operator=(const Slice&) = default;

    char operator[](int index) const {
        Assert(index < Size);
        return Data[index];
    }

    virtual Slice & Clear() {Data=0;Size=0;return *this;}
    bool IsEmpty() const {return Data == 0 || Size == 0;}
    bool IsNull() const {return Data == 0;}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
