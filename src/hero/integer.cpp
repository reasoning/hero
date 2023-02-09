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

#include "hero/integer.h"
#include "hero/error.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace
{

unsigned int BitLen(unsigned int x)
{
    unsigned int size = 0;
    while (x > 0)
    {
        x >>= 1;
        size++;
    }
    return size;
}

unsigned int CeilingDiv(unsigned int a, unsigned int b)
{
    return (a + b - 1) / b;
}
} // namespace

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Integer::Integer(Digit* data, int size, Digit base)
    : Vector<Digit>(data, size), Base(base)
{
    if (base < 2)
        Raise("Integer::Integer(const Digit *, int, Digit): The base must be at least 2");

    for (int i = 0; i < size; i++)
        if (Data[i] >= base)
            Raise("Integer::Integer(const Digit *, int, Digit): A digit is too large for the specified base");

    Trim();
}

Integer::Integer(const Unsigned& x, Digit base)
{
    if (base < 2)
        Raise("Integer(Unsigned, Digit): The base must be at least 2");

    Base = base;

    int maxBitLenOfX = ((Unsigned&)x).Length() * Unsigned::N;
    int minBitsPerDigit = BitLen(base) - 1;
    int maxDigitLenOfX = CeilingDiv(maxBitLenOfX, minBitsPerDigit);

    int amount = maxDigitLenOfX;
    Allocate(amount);
    Size = amount;

    Unsigned _x(x), _base(base);
    int index = 0;

    while (!_x.IsZero())
    {
        Unsigned prev(_x);
        prev.DivideWithRemainder(_base, _x);

        Data[index] = prev.UnsignedShort();

        index++;
    }

    Size = index;
}

Integer::Integer(const Substring& s, Digit base)
{
    if (base > 36)
        Raise("Integer(std::string, Digit): The default string conversion routines use the symbol set 0-9, A-Z and therefore support only up to base 36.  You tried a conversion with a base over 36; write your own string conversion routine.");

    Base = base;

    int amount = s.Size;
    Allocate(amount);
    Size = amount;

    int digit, index;
    for (digit = 0; digit < Size; digit++)
    {
        index = Size - 1 - digit;
        char n = s.Data[index];

        if (n >= '0' && n <= '9')
            Data[digit] = n - '0';
        else if (n >= 'A' && n <= 'Z')
            Data[digit] = n - 'A' + 10;
        else if (n >= 'a' && n <= 'z')
            Data[digit] = n - 'a' + 10;
        else
            Raise("Integer(String, Digit): Bad symbol in input.  Only 0-9, A-Z, a-z are accepted.");

        if (Data[digit] >= base)
            Raise("Integer::Integer(const Digit *, int, Digit): A digit is too large for the specified base");
    }
    Trim();
}

String Integer::Str()
{
    if (Base > 36)
        Raise("Integer ==> String: The default string conversion routines use the symbol set 0-9, A-Z and therefore support only up to base 36.  You tried a conversion with a base over 36; write your own string conversion routine.");

    if (Size == 0)
        return String("0");

    String str;
    str.Reserve(Size + 1);
    str.Size = Size;
    str.Term();

    int digit, symbol;
    for (symbol = 0; symbol < Size; symbol++)
    {
        digit = Size - 1 - symbol;
        Digit num = Data[digit];
        if (num < 10)
            str.Data[symbol] = char('0' + num);
        else
            str.Data[symbol] = char('A' + num - 10);
    }

    return str;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

String Integer::UnsignedToString(const Unsigned& x)
{
    return Integer(x, 10).Str();
}

String Integer::SignedToString(const Signed& x)
{
    return (x.Sign == Signed::SIGN_NEGATIVE)
               ? (String("-") + UnsignedToString(x.Magnitude))
               : (UnsignedToString(x.Magnitude));
}

Unsigned Integer::StringToUnsigned(const String& s)
{
    return Unsigned(Integer(s, 10));
}

Signed Integer::StringToSigned(const String& s)
{
    return (s.Data[0] == '-')   ? Signed(StringToUnsigned(((String&)s).Sub(1, ((String&)s).Length() - 1)), Signed::SIGN_NEGATIVE)
           : (s.Data[0] == '+') ? Signed(StringToUnsigned(((String&)s).Sub(1, ((String&)s).Length() - 1)))
                                : Signed(StringToUnsigned(s));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef HERO_USING_STD

std::ostream& operator<<(std::ostream& os, const Unsigned& x)
{
    Digit base;
    long flags = os.flags();
    if (flags & os.dec)
    {
        base = 10;
    }
    else if (flags & os.hex)
    {
        base = 16;
        if (flags & os.showbase)
            os << "0x";
    }
    else if (flags & os.oct)
    {
        base = 8;
        if (flags & os.showbase)
            os << '0';
    }
    else
    {
        Raise("std::ostream << Unsigned: Could not determine the desired base from output-stream flags");
        return os;
    }

    String s(Integer(x, base));
    os << s.Print();
    return os;
}

std::ostream& operator<<(std::ostream& os, const Signed& x)
{
    if (x.Sign == Signed::SIGN_NEGATIVE)
        os << '-';
    os << x.Magnitude;
    return os;
}

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Unsigned GCD(Unsigned a, Unsigned b)
{
    Unsigned trash;

    for (;;)
    {
        if (b.IsZero())
            return a;
        a.DivideWithRemainder(b, trash);
        if (a.IsZero())
            return b;
        b.DivideWithRemainder(a, trash);
    }
}

void ExtendedEuclidean(Signed m, Signed n, Signed& g, Signed& r, Signed& s)
{
    if (&g == &r || &g == &s || &r == &s)
    {
        Raise("Signed ExtendedEuclidean: Outputs are aliased");
        return;
    }

    Signed r1(1), s1(0), r2(0), s2(1), q;

    for (;;)
    {
        if (n.IsZero())
        {
            r = r1;
            s = s1;
            g = m;
            return;
        }

        m.DivideWithRemainder(n, q);
        r1 -= q * r2;
        s1 -= q * s2;

        if (m.IsZero())
        {
            r = r2;
            s = s2;
            g = n;
            return;
        }

        n.DivideWithRemainder(m, q);
        r2 -= q * r1;
        s2 -= q * s1;
    }
}

Unsigned Modinv(const Signed& x, const Unsigned& n)
{
    Signed g, r, s;
    ExtendedEuclidean(x, n, g, r, s);
    if (g == 1)
    {
        return (r % n).Magnitude;
    }
    else
    {
        Raise("Unsigned Modinv: x and n have a common factor");
        return Unsigned();
    }
}

Unsigned Modexp(const Signed& base, const Unsigned& exponent, const Unsigned& modulus)
{
    Unsigned ans = 1, base2 = (((Signed&)base) % modulus).Magnitude;
    int i = ((Unsigned&)exponent).BitLength();

    while (i > 0)
    {
        i--;

        ans *= ans;
        ans %= modulus;

        if (((Unsigned&)exponent).Bit(i))
        {
            ans *= base2;
            ans %= modulus;
        }
    }

    return ans;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace Hero

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
