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

#include "hero/tokenizer.h"
#include "hero/parser.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Tokenizer::Tokenizer()
{
}

Tokenizer::Tokenizer(Substring& str) : Substring(str)
{
}

void Tokenizer::Tokenise()
{
    Numbers.Destroy();
    Words.Destroy();
    Punctuation.Destroy();

    StringParser parser;
    parser.Assign(*this);

    while (!parser.Eof())
    {
        if (parser.ParseWord())
        {
            Words.Append(parser.Token);
        }
        else if (parser.ParseNumber())
        {
            Numbers.Append(parser.Token);
        }
        else if (parser.ParsePunctuation())
        {
            Punctuation.Append(parser.Token);
        }
        else
        {
            if (!parser.ParseWhitespace() && !parser.Eof())
                parser.Next();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TokenizerStream::TokenizerStream(class Stream& stream) : Stream(stream)
{
}

TokenizerStream::~TokenizerStream()
{
}

bool TokenizerStream::Move()
{
    return Move(1);
}

bool TokenizerStream::Move(int amount)
{
    amount *= Enumeration.Direction;

    if (amount > 0)
    {
        ++Enumeration.Index;
        Enumeration.Prev = Token;

        if (Enumeration.Next.IsNull())
        {
            Token = Next();
        }
        else
        {
            Token = Enumeration.Next;
            Enumeration.Next.Release();
        }
    }
    else if (amount < 0)
    {
        --Enumeration.Index;
        Enumeration.Next = Token;

        if (Enumeration.Prev.IsNull())
        {
            Token = Prev();
        }
        else
        {
            Token = Enumeration.Prev;
            Enumeration.Prev.Release();
        }
    }

    return !Token.IsNull();
}

bool TokenizerStream::Forward()
{
    Enumeration.Index = 0;
    Enumeration.Direction = 1;

    Buffer.Release();
    return Move();
}

bool TokenizerStream::Reverse()
{
    Raise("StreamEnumerator::Last - Method not implemented.\n");
    return true;
}

bool TokenizerStream::Has()
{
    return !Token.IsNull();
}

Substring TokenizerStream::Next()
{
    if (Buffer.Allocated == 0)
    {
        if (Buffer.Remaining() < 256)
            Buffer.Reserve(256);
        Buffer.Size = Stream.Read(Buffer.Data + Buffer.Size, 256);
        Buffer.Data[Buffer.Size] = 0;
    }

    Path::Enumerator separator(Separators);
    Substring match;

    if (Separators.IsEmpty()) return match;

    int found = -1;
    int index = 0;

    Buffer.Left(-(Token.Size + Separator.Size));

    Token.Release();
    Separator.Release();

    while (match.IsNull() && (Stream.IsReadable() || Buffer.Size > 0))
    {
        for (separator.Forward(); separator.Has(); separator.Move())
        {
            index = Buffer.IndexOf(separator.Reference());
            if ((index >= 0) && (index < found || found < 0))
            {
                match = Buffer.Slice(0, index);
                found = index;
                Separator.Assign(separator.Reference());

                if (index == 0) break;
            }
        }

        if (match.IsNull())
        {
            if (Stream.IsReadable())
            {
                if (Buffer.Remaining() < 256)
                    Buffer.Reserve(256);
                Buffer.Size += Stream.Read(Buffer.Data + Buffer.Size, 256);
                Buffer.Data[Buffer.Size] = 0;
            }
            else
            {
                match = Buffer;
            }
        }
    }

    return match;
}

Substring TokenizerStream::Prev()
{
    Raise("StreamEnumerator::Last - Method not supported.\n");
    Substring found;
    return found;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace Hero

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
