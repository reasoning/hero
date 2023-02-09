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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "hero/number.h"
#include "hero/binary.h"
#include "hero/hero.h"
#include "hero/time.h"

#include "hero/integer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const int Prime::NumberCount = 500;
const int Prime::Number[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113,
    127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
    233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
    283, 293, 307, 311, 313, 317, 331, 337, 347, 349,
    353, 359, 367, 373, 379, 383, 389, 397, 401, 409,
    419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
    467, 479, 487, 491, 499, 503, 509, 521, 523, 541,
    547, 557, 563, 569, 571, 577, 587, 593, 599, 601,
    607, 613, 617, 619, 631, 641, 643, 647, 653, 659,
    661, 673, 677, 683, 691, 701, 709, 719, 727, 733,
    739, 743, 751, 757, 761, 769, 773, 787, 797, 809,
    811, 821, 823, 827, 829, 839, 853, 857, 859, 863,
    877, 881, 883, 887, 907, 911, 919, 929, 937, 941,
    947, 953, 967, 971, 977, 983, 991, 997, 1009, 1013,
    1019, 1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069,
    1087, 1091, 1093, 1097, 1103, 1109, 1117, 1123, 1129, 1151,
    1153, 1163, 1171, 1181, 1187, 1193, 1201, 1213, 1217, 1223,
    1229, 1231, 1237, 1249, 1259, 1277, 1279, 1283, 1289, 1291,
    1297, 1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 1373,
    1381, 1399, 1409, 1423, 1427, 1429, 1433, 1439, 1447, 1451,
    1453, 1459, 1471, 1481, 1483, 1487, 1489, 1493, 1499, 1511,
    1523, 1531, 1543, 1549, 1553, 1559, 1567, 1571, 1579, 1583,
    1597, 1601, 1607, 1609, 1613, 1619, 1621, 1627, 1637, 1657,
    1663, 1667, 1669, 1693, 1697, 1699, 1709, 1721, 1723, 1733,
    1741, 1747, 1753, 1759, 1777, 1783, 1787, 1789, 1801, 1811,
    1823, 1831, 1847, 1861, 1867, 1871, 1873, 1877, 1879, 1889,
    1901, 1907, 1913, 1931, 1933, 1949, 1951, 1973, 1979, 1987,
    1993, 1997, 1999, 2003, 2011, 2017, 2027, 2029, 2039, 2053,
    2063, 2069, 2081, 2083, 2087, 2089, 2099, 2111, 2113, 2129,
    2131, 2137, 2141, 2143, 2153, 2161, 2179, 2203, 2207, 2213,
    2221, 2237, 2239, 2243, 2251, 2267, 2269, 2273, 2281, 2287,
    2293, 2297, 2309, 2311, 2333, 2339, 2341, 2347, 2351, 2357,
    2371, 2377, 2381, 2383, 2389, 2393, 2399, 2411, 2417, 2423,
    2437, 2441, 2447, 2459, 2467, 2473, 2477, 2503, 2521, 2531,
    2539, 2543, 2549, 2551, 2557, 2579, 2591, 2593, 2609, 2617,
    2621, 2633, 2647, 2657, 2659, 2663, 2671, 2677, 2683, 2687,
    2689, 2693, 2699, 2707, 2711, 2713, 2719, 2729, 2731, 2741,
    2749, 2753, 2767, 2777, 2789, 2791, 2797, 2801, 2803, 2819,
    2833, 2837, 2843, 2851, 2857, 2861, 2879, 2887, 2897, 2903,
    2909, 2917, 2927, 2939, 2953, 2957, 2963, 2969, 2971, 2999,
    3001, 3011, 3019, 3023, 3037, 3041, 3049, 3061, 3067, 3079,
    3083, 3089, 3109, 3119, 3121, 3137, 3163, 3167, 3169, 3181,
    3187, 3191, 3203, 3209, 3217, 3221, 3229, 3251, 3253, 3257,
    3259, 3271, 3299, 3301, 3307, 3313, 3319, 3323, 3329, 3331,
    3343, 3347, 3359, 3361, 3371, 3373, 3389, 3391, 3407, 3413,
    3433, 3449, 3457, 3461, 3463, 3467, 3469, 3491, 3499, 3511,
    3517, 3527, 3529, 3533, 3539, 3541, 3547, 3557, 3559, 3571};

const int Prime::PowerCount = 100;
const Prime::PowerOffset Prime::Power[] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {5, 15, 17, 23, 27, 29, 33, 45, 57, 59},
    {3, 9, 13, 21, 25, 33, 45, 49, 51, 55},
    {3, 5, 11, 15, 27, 33, 41, 47, 53, 57},
    {9, 19, 21, 31, 37, 45, 49, 51, 55, 61},
    {3, 5, 17, 23, 39, 45, 47, 69, 75, 77},
    {1, 13, 21, 25, 31, 45, 69, 75, 81, 91},
    {3, 15, 21, 23, 35, 45, 51, 65, 83, 111},
    {19, 49, 51, 55, 61, 75, 81, 115, 121, 135},
    {15, 17, 39, 57, 87, 89, 99, 113, 117, 123},
    {1, 9, 13, 31, 49, 61, 63, 85, 91, 99},
    {5, 11, 17, 23, 33, 35, 41, 65, 75, 93},
    {1, 19, 27, 31, 45, 57, 67, 69, 85, 87},
    {3, 5, 17, 27, 59, 69, 129, 143, 153, 185},
    {9, 19, 21, 55, 61, 69, 105, 111, 121, 129},
    {3, 17, 27, 33, 57, 87, 105, 113, 117, 123},
    {15, 21, 27, 37, 61, 69, 135, 147, 157, 159},
    {3, 17, 33, 63, 75, 77, 89, 95, 117, 167},
    {39, 49, 61, 85, 91, 115, 141, 159, 165, 183},
    {5, 27, 45, 87, 101, 107, 111, 117, 125, 135},
    {39, 79, 111, 115, 135, 187, 199, 219, 231, 235},
    {57, 89, 95, 119, 125, 143, 165, 183, 213, 273},
    {3, 33, 43, 63, 73, 75, 93, 99, 121, 133},
    {35, 41, 83, 101, 105, 107, 135, 153, 161, 173},
    {1, 19, 61, 69, 85, 99, 105, 151, 159, 171},
    {5, 17, 65, 99, 107, 135, 153, 185, 209, 267},
    {9, 25, 49, 79, 105, 285, 301, 303, 321, 355},
    {41, 77, 113, 131, 143, 165, 185, 207, 227, 281},
    {31, 49, 61, 69, 79, 121, 141, 247, 309, 325},
    {5, 17, 23, 65, 117, 137, 159, 173, 189, 233},
    {25, 31, 45, 69, 123, 141, 199, 201, 351, 375},
    {45, 87, 107, 131, 153, 185, 191, 227, 231, 257},
    {7, 19, 67, 91, 135, 165, 219, 231, 241, 301},
    {87, 167, 195, 203, 213, 285, 293, 299, 389, 437},
    {21, 31, 55, 63, 73, 75, 91, 111, 133, 139},
    {11, 17, 33, 53, 65, 143, 161, 165, 215, 227},
    {57, 67, 117, 175, 255, 267, 291, 309, 319, 369},
    {17, 117, 119, 129, 143, 149, 287, 327, 359, 377},
    {55, 69, 81, 93, 121, 133, 139, 159, 193, 229},
    {21, 57, 63, 77, 167, 197, 237, 287, 305, 311},
    {115, 127, 147, 279, 297, 339, 435, 541, 619, 649},
    {59, 65, 89, 93, 147, 165, 189, 233, 243, 257},
    {81, 111, 123, 139, 181, 201, 213, 265, 283, 339},
    {27, 35, 51, 71, 113, 117, 131, 161, 195, 233},
    {129, 139, 165, 231, 237, 247, 355, 391, 397, 439},
    {47, 143, 173, 183, 197, 209, 269, 285, 335, 395},
    {111, 145, 231, 265, 315, 339, 343, 369, 379, 421},
    {33, 53, 131, 165, 195, 245, 255, 257, 315, 327},
    {55, 67, 99, 127, 147, 169, 171, 199, 207, 267},
    {5, 27, 47, 57, 89, 93, 147, 177, 189, 195},
    {13, 25, 49, 61, 69, 111, 195, 273, 363, 423},
    {27, 57, 63, 137, 141, 147, 161, 203, 213, 251},
    {55, 99, 225, 427, 517, 607, 649, 687, 861, 871},
    {93, 107, 173, 179, 257, 279, 369, 395, 399, 453},
    {1, 31, 45, 229, 259, 283, 339, 391, 403, 465},
    {57, 87, 117, 143, 153, 167, 171, 195, 203, 273},
    {25, 165, 259, 301, 375, 387, 391, 409, 457, 471},
    {59, 83, 95, 179, 189, 257, 279, 323, 353, 363},
    {49, 79, 115, 141, 163, 229, 301, 345, 453, 493},
    {5, 45, 173, 203, 275, 297, 387, 401, 443, 495},
    {19, 31, 49, 57, 61, 75, 81, 165, 181, 237},
    {23, 83, 125, 147, 149, 167, 285, 315, 345, 357},
    {19, 91, 93, 103, 129, 153, 165, 201, 255, 385},
    {35, 71, 167, 215, 263, 267, 273, 447, 473, 585},
    {231, 325, 411, 435, 441, 465, 559, 577, 601, 721},
    {93, 107, 129, 167, 249, 269, 329, 347, 429, 473},
    {69, 181, 199, 273, 319, 433, 475, 501, 523, 645},
    {35, 45, 57, 135, 153, 237, 257, 275, 461, 465},
    {97, 207, 231, 271, 279, 289, 325, 381, 409, 427},
    {15, 63, 117, 123, 143, 189, 215, 267, 285, 347},
    {33, 43, 145, 163, 195, 261, 295, 379, 433, 451},
    {11, 95, 111, 123, 147, 153, 191, 263, 303, 507},
    {67, 199, 249, 277, 355, 367, 405, 447, 477, 511},
    {65, 93, 117, 143, 285, 317, 549, 645, 765, 933},
    {51, 63, 163, 205, 333, 349, 429, 433, 481, 553},
    {57, 113, 185, 315, 363, 365, 375, 453, 623, 635},
    {55, 97, 117, 121, 139, 285, 307, 405, 429, 561},
    {35, 69, 213, 215, 333, 399, 525, 563, 587, 597},
    {19, 61, 181, 295, 411, 433, 469, 519, 531, 823},
    {35, 41, 65, 71, 113, 255, 261, 293, 357, 461},
    {67, 129, 181, 195, 201, 217, 261, 277, 289, 339},
    {299, 455, 483, 563, 605, 719, 735, 743, 753, 797},
    {1, 21, 31, 49, 69, 99, 103, 265, 321, 441},
    {33, 41, 53, 75, 227, 263, 273, 291, 297, 317},
    {45, 81, 111, 201, 315, 339, 567, 619, 655, 771},
    {83, 149, 197, 317, 363, 419, 485, 497, 519, 537},
    {25, 51, 79, 105, 273, 405, 489, 553, 571, 579},
    {3, 11, 105, 173, 273, 297, 321, 395, 407, 431},
    {15, 37, 211, 339, 387, 415, 441, 447, 555, 561},
    {17, 87, 93, 147, 165, 189, 237, 243, 315, 347},
    {141, 165, 349, 399, 453, 595, 729, 741, 859, 885},
    {51, 65, 107, 117, 141, 227, 273, 363, 471, 525},
    {115, 145, 247, 319, 381, 427, 675, 717, 1207, 1231},
    {15, 99, 153, 183, 267, 285, 357, 479, 603, 833}

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const float Float::PositiveZero = 0.0;
const float Float::NegativeZero = -0.0;
const float Float::PositiveInfinity = Float::Infinity();
const float Float::NegativeInfinity = -Float::Infinity();
const float Float::NotANumber = Float::Nan();

const double Double::PositiveZero = 0.0;
const double Double::NegativeZero = -0.0;
const double Double::PositiveInfinity = Double::Infinity();
const double Double::NegativeInfinity = -Double::Infinity();
const double Double::NotANumber = Double::Nan();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

double Frequency::Sum()
{
    double sum = 0;
    for (int i = 0; i < Length(); ++i)
    {
        sum += Data[i].Double();
    }

    return sum;
}

double Frequency::Min()
{
    double min = Number::MaximumDouble();
    double current;
    for (int i = 0; i < Length(); ++i)
    {
        current = Data[i].Double();
        if (min > current)
            min = current;
    }

    if (min < Number::MaximumDouble())
        return min;
    else
        return 0;
}

double Frequency::Max()
{
    double max = 0;
    double current;
    for (int i = 0; i < Length(); ++i)
    {
        current = Data[i].Double();
        if (max < current)
            max = current;
    }

    return max;
}

double Frequency::Mean()
{
    double sum = Sum();
    if (sum > 0) sum /= Length();
    return sum;
}

double Frequency::Mode()
{
    Sort([](const Number& lhs, const Number& rhs) -> int
         { return ((Number&)lhs).Compare((Object*)&rhs, Comparable::COMPARE_GENERAL); });

    double mode = 0;
    int modeCount = 0;
    double value = 0;
    int valueCount = 0;

    for (int i = 0; i < Length(); ++i)
    {
        if (Number::Equals(value, Data[i].Double()))
        {
            ++valueCount;
        }
        else
        {
            if (valueCount > modeCount)
            {
                mode = value;
                modeCount = valueCount;
            }

            value = Data[i].Double();
            valueCount = 1;
        }
    }

    if (modeCount == 0)
    {
        mode = value;
    }

    return mode;
}

double Frequency::Variance()
{
    if (Length() == 0)
        return 0;

    double mean = Mean();
    double variance = 0;

    for (int i = 0; i < Length(); ++i)
    {
        variance += pow(Data[i].Double() - mean, 2);
    }

    return variance;
}

double Frequency::Deviation()
{
    return sqrt(Variance());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Numbers::Hash(int value)
{
    return Hashing::IntegerHash::Hash32Shift(value);
}

int Numbers::Hash(long value)
{
#ifdef HERO_PLATFORM_POSIX
    #ifdef HERO_PLATFORM_X86_64

    return Hashing::IntegerHash::Hash6432Shift(value);

    #else
    return Hashing::IntegerHash::Hash32Shift(value);
    #endif
#else
    return Hashing::IntegerHash::Hash32Shift(value);
#endif
}

long Numbers::Random()
{
    struct CMWC4096
    {
        unsigned long Pool[4096];
        unsigned long Carry;

        CMWC4096() : Carry(0)
        {
            unsigned long nano = Timer::Nanoseconds();
            srand((nano >> 16) ^ (int)nano);
            unsigned long random = 0;
            for (int p = 0; p < 4096; ++p)
            {
                random = rand();
                Pool[p] = random ^ (nano >> (random & 0x1F));
                if (((p ^ nano) & 0x0F) == 0)
                    nano = Timer::Nanoseconds();
            }

            Carry = (long long)Timer::Nanoseconds() % 18782;
        }

        unsigned long operator()(void)
        {
            unsigned long long temp, arg = 18782LL, base = 4294967295LL;
            static unsigned long index = 4095;
            unsigned long range = base - 1;
            index = (index + 1) & 4095;
            temp = arg * Pool[index] + Carry;
            Carry = (temp >> 32);
            temp = (temp & base) + Carry;
            if (temp > range)
            {
                Carry++;
                temp = temp - base;
            }
            return (Pool[index] = range - temp);
        }
    };

    static CMWC4096 generator;
    return generator();
}

int Numbers::ConvertToString(long long number, int radix, char* data, int size, bool padded)
{
    if (!padded)
    {
        char* dest = data;
        int count = 0;
        while (number > 0 && count < size)
        {
            *(data++) = '0' + number % radix;
            number /= radix;
            ++count;
        }

        while (dest < data)
            *(dest++) = *(--data);
    }
    else
    {
        data += size;
        int loop = (size + 3) / 4;
        if (loop > 0)
        {
            switch (size % 4)
            {
            case 0:
                do
                {
                    *(--data) = '0' + number % radix;
                    number /= radix;
                case 3:
                    *(--data) = '0' + number % radix;
                    number /= radix;
                case 2:
                    *(--data) = '0' + number % radix;
                    number /= radix;
                case 1:
                    *(--data) = '0' + number % radix;
                    number /= radix;
                } while (--loop > 0);
            }
        }
    }

    return size;
}

Superstring Numbers::ConvertToString(long long number, int radix)
{
    struct Convert
    {
        static void Two(long long number, int base, char* data)
        {
            data[1] = '0' + number % base;
            data[0] = '0' + number / base;
        }

        static void Four(long long number, int base, char* data)
        {
            int power = base * base;
            int high = number / power;
            data[1] = '0' + high % base;
            data[0] = '0' + high / base;
            int low = number % power;
            data[3] = '0' + low % base;
            data[2] = '0' + low / base;
        }
    };

    int power = radix * radix;
    if (power > number)
    {
        char data[2];
        Convert::Two(number, radix, data);
        Superstring string(data, 2);

        return string;
    }
    else
    {
        power *= power;
        if (power > number)
        {
            char data[4];
            Convert::Four(number, radix, data);
            Superstring string(data, 4);

            return string;
        }
        else
        {
            char data[64];

            int size = Numbers::ConvertToString(number, radix, data, 64, false);
            Superstring string(data, size);
            return string;
        }
    }

    return Superstring();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Identity Number::Instance;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Number::Print(String& print)
{
    if (IsInteger())
    {
        if (IsSigned())
            print.Append((signed long long)Data.Integer);
        else
            print.Append((unsigned long long)Data.Integer);
    }
    else if (IsReal())
    {
        if (IsFloat())
            print.Append((float)Data.Real);
        else
            print.Append((double)Data.Real);
    }

    Raise(new TypeError());
}

signed long long Number::SignedLongLong()
{
    if (IsInteger())
    {
        if (IsLongLong())
        {
            Assert(IsSigned() || Data.Integer > 0);
            if (IsUnsigned() && Data.Integer < 0)
                Raise(new OverflowError());
        }

        return (signed long long)Data.Integer;
    }
    else if (IsFloat())
    {
        return (signed long long)(float)Data.Real;
    }
    else if (IsDouble())
    {
        return (signed long long)(double)Data.Real;
    }

    Raise(new TypeError());
    return 0;
}

unsigned long long Number::UnsignedLongLong()
{
    if (IsInteger())
    {
        if (IsLongLong())
        {
            Assert(IsUnsigned() || Data.Integer > 0);
            if (IsSigned() && Data.Integer < 0)
                Raise(new UnderflowError());
        }
        return (unsigned long long)Data.Integer;
    }
    else if (IsFloat())
    {
        return (unsigned long long)(float)Data.Real;
    }
    else if (IsDouble())
    {
        return (unsigned long long)(double)Data.Real;
    }

    Raise(new TypeError());
    return 0;
}

int Number::Compare(Object* object, int comparitor)
{
    if (object->Identify() == Instance)
    {
        Number* number = (Number*)object;

        if (IsInteger())
        {
            if (IsSigned())
            {
                signed long long lhs = SignedLongLong();
                if (number->IsInteger())
                {
                    if (number->IsSigned())
                        return lhs - number->SignedLongLong();

                    unsigned long long rhs = number->UnsignedLongLong();
                    if (lhs < 0) return -1;

                    return (unsigned long long)lhs - rhs;
                }
            }
            else if (IsUnsigned())
            {
                unsigned long long lhs = UnsignedLongLong();
                if (number->IsInteger())
                {
                    if (number->IsUnsigned())
                        return lhs - number->UnsignedLongLong();

                    signed long long rhs = number->SignedLongLong();
                    if (rhs < 0) return 1;

                    return lhs - (unsigned long long)rhs;
                }
            }
        }

        Numbers::Compare(Double(), number->Double());
    }
    else if (object->Identify() == Substring::Instance)
    {
        Substring* sequence = (Substring*)object;

        String str(sequence->Data, sequence->Size, 0);

        if (IsInteger())
        {
            Signed lhs = Signed(Data.Integer);
            Signed rhs = Hero::Integer::StringToSigned(str);
            lhs -= rhs;
            return lhs.SignedInt();
        }
        else if (IsReal())
        {
            double lhs = (double)Data.Real;
            double rhs = (double)((Substring*)object)->Real();

            return Numbers::Compare((double)lhs, (double)rhs);
        }
    }

    return Identity::Error;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Signed NumberAsSigned(Number& number)
{
    return number.IsNegative() ? Signed(number.SignedLongLong()) : Signed(number.UnsignedLongLong());
}

class Unsigned NumberAsUnsigned(Number& number)
{
    if (number.IsPositive()) return Unsigned(number.UnsignedLongLong());

    Raise(new TypeError());
    return Unsigned();
}

Number& Number::operator=(Number& number)
{
    if (!Type)
    {
        Type = number.Type;
        Data = number.Data;
        Size = number.Size;
    }
    else
    {
        Type = number.Type;
        Size = number.Size;

        if (number.IsInteger())
            Data.Integer = number.Data.Integer;
        else if (number.IsReal())
            Data.Real = number.Data.Real;
    }

    return *this;
}

bool Number::operator==(Number& number)
{
    if (IsInteger() && number.IsInteger())
    {
        Signed lhs = NumberAsSigned(*this);
        Signed rhs = NumberAsSigned(number);
        return lhs == rhs;
    }
    else if (IsReal() || number.IsReal())
    {
        double lhs = Real();
        double rhs = number.Real();
        return Numbers::Equals(lhs, rhs);
    }

    return false;
}

bool Number::operator<=(Number& number)
{
    if (IsInteger() && number.IsInteger())
    {
        Signed lhs = NumberAsSigned(*this);
        Signed rhs = NumberAsSigned(number);
        return lhs.Compare(rhs) <= 0;
    }
    else if (IsReal() || number.IsReal())
    {
        double lhs = Real();
        double rhs = number.Real();
        return Numbers::Compare(lhs, rhs) <= 0;
    }

    return false;
}

bool Number::operator<(Number& number)
{
    if (IsInteger() && number.IsInteger())
    {
        Signed lhs = NumberAsSigned(*this);
        Signed rhs = NumberAsSigned(number);
        return lhs.Compare(rhs) < 0;
    }
    else if (IsReal() || number.IsReal())
    {
        double lhs = Real();
        double rhs = number.Real();
        return Numbers::Compare(lhs, rhs) < 0;
    }

    return false;
}

bool Number::operator>=(Number& number)
{
    if (IsInteger() && number.IsInteger())
    {
        Signed lhs = NumberAsSigned(*this);
        Signed rhs = NumberAsSigned(number);
        return lhs.Compare(rhs) >= 0;
    }
    else if (IsReal() || number.IsReal())
    {
        double lhs = Real();
        double rhs = number.Real();
        return Numbers::Compare(lhs, rhs) >= 0;
    }

    return false;
}

bool Number::operator>(Number& number)
{
    if (IsInteger() && number.IsInteger())
    {
        Signed lhs = NumberAsSigned(*this);
        Signed rhs = NumberAsSigned(number);
        return lhs.Compare(rhs) > 0;
    }
    else if (IsReal() || number.IsReal())
    {
        double lhs = Real();
        double rhs = number.Real();
        return Numbers::Compare(lhs, rhs) > 0;
    }

    return false;
}

Number Number::operator-(Number& number)
{
    if (IsInteger() && number.IsInteger())
    {
        Signed lhs = NumberAsSigned(*this);
        Signed rhs = NumberAsSigned(number);
        lhs -= rhs;
        return lhs.IsNegative() ? lhs.SignedLongLong() : lhs.UnsignedLongLong();
    }
    else if (IsReal() || number.IsReal())
    {
        double lhs = Real();
        double rhs = number.Real();
        return lhs - rhs;
    }

    return 0;
}

Number& Number::operator-=(Number& number)
{
    if (IsInteger() && number.IsInteger())
    {
        Signed lhs = NumberAsSigned(*this);
        Signed rhs = NumberAsSigned(number);
        lhs -= rhs;
        operator=(lhs.IsNegative() ? lhs.SignedLongLong() : lhs.UnsignedLongLong());
    }
    else if (IsReal() || number.IsReal())
    {
        if (IsInteger())
        {
            Signed lhs = NumberAsSigned(*this);
            Signed rhs = NumberAsSigned(number);
            lhs -= rhs;

            if (lhs.IsNegative())
            {
                if (lhs < MinimumSignedLongLong())
                    Raise(new UnderflowError());
                operator=(lhs.SignedLongLong());
            }
            else
            {
                if (lhs > MaximumUnsignedLongLong())
                    Raise(new OverflowError());
                operator=(lhs.UnsignedLongLong());
            }
        }
        else if (IsReal())
        {
            double lhs = Real();
            double rhs = number.Real();
            lhs -= rhs;
            operator=(lhs);
        }
    }

    return *this;
}

Number Number::operator+(Number& number)
{
    if (IsInteger() && number.IsInteger())
    {
        Signed lhs = NumberAsSigned(*this);
        Signed rhs = NumberAsSigned(number);
        lhs += rhs;
        return lhs.IsNegative() ? lhs.SignedLongLong() : lhs.UnsignedLongLong();
    }
    else if (IsReal() || number.IsReal())
    {
        double lhs = Real();
        double rhs = number.Real();
        return lhs + rhs;
    }

    return 0;
}

Number& Number::operator+=(Number& number)
{
    if (IsInteger() && number.IsInteger())
    {
        Signed lhs = NumberAsSigned(*this);
        Signed rhs = NumberAsSigned(number);
        lhs += rhs;
        operator=(lhs.IsNegative() ? lhs.SignedLongLong() : lhs.UnsignedLongLong());
    }
    else if (IsReal() || number.IsReal())
    {
        if (IsInteger())
        {
            Signed lhs = NumberAsSigned(*this);
            Signed rhs = NumberAsSigned(number);
            lhs += rhs;

            if (lhs.IsNegative())
            {
                if (lhs < MinimumSignedLongLong())
                    Raise(new UnderflowError());
                operator=(lhs.SignedLongLong());
            }
            else
            {
                if (lhs > MaximumUnsignedLongLong())
                    Raise(new OverflowError());
                operator=(lhs.UnsignedLongLong());
            }
        }
        else if (IsReal())
        {
            double lhs = Real();
            double rhs = number.Real();
            lhs += rhs;
            operator=(lhs);
        }
    }

    return *this;
}

Number Number::operator/(Number& number)
{
    if (IsInteger() && number.IsInteger())
    {
        Signed lhs = NumberAsSigned(*this);
        Signed rhs = NumberAsSigned(number);
        lhs /= rhs;
        return lhs.IsNegative() ? lhs.SignedLongLong() : lhs.UnsignedLongLong();
    }
    else if (IsReal() || number.IsReal())
    {
        double lhs = Real();
        double rhs = number.Real();
        return lhs / rhs;
    }

    return 0;
}

Number& Number::operator/=(Number& number)
{
    if (IsInteger() && number.IsInteger())
    {
        Signed lhs = NumberAsSigned(*this);
        Signed rhs = NumberAsSigned(number);
        lhs /= rhs;
        operator=(lhs.IsNegative() ? lhs.SignedLongLong() : lhs.UnsignedLongLong());
    }
    else if (IsReal() || number.IsReal())
    {
        if (IsInteger())
        {
            Signed lhs = NumberAsSigned(*this);
            Signed rhs = NumberAsSigned(number);
            lhs /= rhs;

            if (lhs.IsNegative())
            {
                if (lhs < MinimumSignedLongLong())
                    Raise(new UnderflowError());
                operator=(lhs.SignedLongLong());
            }
            else
            {
                if (lhs > MaximumUnsignedLongLong())
                    Raise(new OverflowError());
                operator=(lhs.UnsignedLongLong());
            }
        }
        else if (IsReal())
        {
            double lhs = Real();
            double rhs = number.Real();
            lhs += rhs;
            operator=(lhs);
        }
    }

    return *this;
}

Number Number::operator*(Number& number)
{
    if (IsInteger() && number.IsInteger())
    {
        Signed lhs = NumberAsSigned(*this);
        Signed rhs = NumberAsSigned(number);
        lhs *= rhs;
        return lhs.IsNegative() ? lhs.SignedLongLong() : lhs.UnsignedLongLong();
    }
    else if (IsReal() || number.IsReal())
    {
        double lhs = Real();
        double rhs = number.Real();
        return lhs * rhs;
    }

    return 0;
}

Number& Number::operator*=(Number& number)
{
    if (IsInteger() && number.IsInteger())
    {
        Signed lhs = NumberAsSigned(*this);
        Signed rhs = NumberAsSigned(number);
        lhs *= rhs;
        operator=(lhs.IsNegative() ? lhs.SignedLongLong() : lhs.UnsignedLongLong());
    }
    else if (IsReal() || number.IsReal())
    {
        if (IsInteger())
        {
            Signed lhs = NumberAsSigned(*this);
            Signed rhs = NumberAsSigned(number);
            lhs *= rhs;

            if (lhs.IsNegative())
            {
                if (lhs < MinimumSignedLongLong())
                    Raise(new UnderflowError());
                operator=(lhs.SignedLongLong());
            }
            else
            {
                if (lhs > MaximumUnsignedLongLong())
                    Raise(new OverflowError());
                operator=(lhs.UnsignedLongLong());
            }
        }
        else if (IsReal())
        {
            double lhs = Real();
            double rhs = number.Real();
            lhs *= rhs;
            operator=(lhs);
        }
    }

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace Hero

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
