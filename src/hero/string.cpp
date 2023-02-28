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


#ifdef HERO_PLATFORM_POSIX

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wfloat-conversion"
#pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"
#pragma clang diagnostic ignored "-Wunused-parameter"

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "hero/hero.h"
#include "hero/string.h"
#include "hero/assert.h"
#include "hero/format.h"
#include "hero/regex.h"
#include "hero/memory.h"

#ifdef HERO_USING_STD
#include <ostream>
#endif

#ifdef HERO_USING_TRACY
#include "tracy/Tracy.hpp"
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <math.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const int Characters::Upper[] = {
	  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
	 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
	 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
	 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
	 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
	 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
	 96, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
	 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90,123,124,125,126,127,
	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
	144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
	176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
	192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,
	208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255};

const int Characters::Lower[] = {
	  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
	 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
	 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
	 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
	 64, 97, 98, 99,100,101,102,103,104,105,106,107,108,109,110,111,
	112,113,114,115,116,117,118,119,120,121,122, 91, 92, 93, 94, 95,
	 96, 97, 98, 99,100,101,102,103,104,105,106,107,108,109,110,111,
	112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
	144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
	176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
	192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,
	208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255};

bool Characters::Equals(char * left, int leftSize, char * right, int rightSize, bool caseless) 
{
	if (leftSize != rightSize) return false;
	if (left == right) return true;
	return Compare(left,leftSize,right,rightSize,caseless)==0;    
}

bool Characters::Equals(char * left, char * right, int size, bool caseless) 
{

	Assert(size >= 0);

    if (size < 0) return false;
	if ((left == 0 || right == 0) && left != right) return false;
	if (left == right || size == 0) return true;
	if (Characters::Normalise(left[size-1],caseless) != Characters::Normalise(right[size-1],caseless)) return false;

	for (int index=(size>>2);index>0;--index) 
	{
		if (*(int*)left != *(int*)right) 
		{
			if (Characters::Normalise(*left++,caseless)!=Characters::Normalise(*right++,caseless)) return false;
			if (Characters::Normalise(*left++,caseless)!=Characters::Normalise(*right++,caseless)) return false;
			if (Characters::Normalise(*left++,caseless)!=Characters::Normalise(*right++,caseless)) return false;
			if (Characters::Normalise(*left++,caseless)!=Characters::Normalise(*right++,caseless)) return false;
		}
		else 
		{
			left+=4;
			right+=4;
		}
	}

	for (int index=(size&0x00000003);index>0;--index)
	{
		if (Characters::Normalise(*left++,caseless) != Characters::Normalise(*right++,caseless)) return false;
	}

	return true;
}

int Characters::Compare(char * left, int leftSize, char * right, int rightSize, bool caseless) 
{
	int res = Compare(left,right,(leftSize<rightSize)?leftSize:rightSize,caseless);
	return (res)?res:leftSize-rightSize;
}

int Characters::Compare(char * left, char * right, int size, bool caseless) 
{

	Assert(size >= 0);

    int res;
	for (int index = (size>>2);index>0;--index) 
	{
		if (*(int*)left != *(int*)right) 
		{
			res = Characters::Normalise(*left++,caseless)-Characters::Normalise(*right++,caseless);
			if (res) return res;

			res = Characters::Normalise(*left++,caseless)-Characters::Normalise(*right++,caseless);
			if (res) return res;

			res = Characters::Normalise(*left++,caseless)-Characters::Normalise(*right++,caseless);
			if (res) return res;

			res = Characters::Normalise(*left++,caseless)-Characters::Normalise(*right++,caseless);
			if (res) return res;
		}
		else 
		{
			left    +=4;
			right   +=4;
		}
	}

	for (int index = (size&0x00000003);index>0;--index)
	{
		res = Characters::Normalise(*left++,caseless)-Characters::Normalise(*right++,caseless);
		if (res) return res;
	}

	return 0;
}

char * Characters::LFind(char * left, int leftSize, char * right, int rightSize, bool caseless) {
{
	if (right == 0 || rightSize < 1 || rightSize > leftSize) return 0;

	int limit = leftSize-rightSize;
	for(int n=0;n <= limit;++n) 
	{
		if (Characters::Normalise(left[n],caseless) == Characters::Normalise(right[0],caseless) && 
            Characters::Normalise(left[n+rightSize-1],caseless) == Characters::Normalise(right[rightSize-1],caseless))
			if (Equals((left+n),right,rightSize,caseless)) return (char *) (left+n);
	}

	return 0;	
}
}

char * Characters::RFind(char * left, int leftSize, char * right, int rightSize, bool caseless) {
	if (right == 0 || rightSize < 1 || rightSize > leftSize)
		return 0;

	int limit = leftSize-rightSize;
	for(int n=limit;n >= 0;--n) 
	{
		if (Characters::Normalise(left[n],caseless) == Characters::Normalise(right[0],caseless) && 
            Characters::Normalise(left[n+rightSize-1],caseless) == Characters::Normalise(right[rightSize-1],caseless))
			if (Equals((left+n),right,rightSize,caseless)) return (char *) (left+n);
	}

	return 0;	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Characters::Compare(const char * left, const char * right)
{

	if (left == right) return 0;

	while(true)
	{
		char l = *left++;
		char r = *right++;

		char d = l-r;
		if (d) return d;

		if (l == 0 || r == 0) return 0;	
	}

}

int Characters::Compare(const char * left, const char * right, const int size)
{
	int res;
	for (int index = (size>>2);index>0;--index)
	{
		if (*(int*)left != *(int*)right)
		{
			res = (*left++)-(*right++);
			if (res) return res;

			res = (*left++)-(*right++);
			if (res) return res;

			res = (*left++)-(*right++);
			if (res) return res;

			res = (*left++)-(*right++);
			if (res) return res;
		}
		else
		{
			left+=4;
			right+=4;
		}
	}

	for (int index = (size&0x00000003);index>0;--index)
	{
		res = (*left++)-(*right++);
		if (res) return res;
	}

	return 0;
}

int Characters::Compare(const char * left, const int leftSize, const char * right, const int rightSize)
{
	int res = Compare(left,right,(leftSize<rightSize)?leftSize:rightSize);
	return (res)?res:leftSize-rightSize;
}

int Characters::CompareCaseless(const char * left, const char * right)
{

	if (left == right) return 0;

	while(true)
	{
		char l = Characters::Caseless(*left++);
		char r = Characters::Caseless(*right++);

		char d = l-r;
		if (d) return d;

		if (l == 0 || r == 0) return 0;	
	}

	return 0;
}

int Characters::CompareCaseless(const char * left, const char * right, const int size)
{
	int res;
	for (int index = (size>>2);index>0;--index)
	{
		if (*(int*)left != *(int*)right)
		{
			res = Characters::Caseless(*left++)-Characters::Caseless(*right++);
			if (res) return res;

			res = Characters::Caseless(*left++)-Characters::Caseless(*right++);
			if (res) return res;

			res = Characters::Caseless(*left++)-Characters::Caseless(*right++);
			if (res) return res;

			res = Characters::Caseless(*left++)-Characters::Caseless(*right++);
			if (res) return res;
		}
		else
		{
			left+=4;
			right+=4;
		}
	}

	for (int index = (size&0x00000003);index>0;--index)
	{
		res = Characters::Caseless(*left++)-Characters::Caseless(*right++);
		if (res) return res;
	}

	return 0;
}

int Characters::CompareCaseless(const char * left, const int leftSize, const char * right, const int rightSize)
{
	int res = CompareCaseless(left,right,(leftSize<rightSize)?leftSize:rightSize);
	return (res)?res:leftSize-rightSize;
}

int Characters::CompareReverse(const char * left, const char * right)
{
	int res;
	int leftSize = Length(left);
	int rightSize = Length(right);
	res = CompareReverse(left,right,(leftSize<rightSize)?leftSize:rightSize);
	return (res)?res:leftSize-rightSize;
}

int Characters::CompareReverse(const char * left, const char * right, const int size)
{
	int res;
	left+=size;
	right+=size;
	for (int index = (size&0x00000003);index>0;--index)
	{
		res = (*--left)-(*--right);
		if (res) return res;
	}

	for (int index = (size>>2);index>0;--index)
	{
		if (*(int*)(left-=4) != *(int*)(right-=4))
		{
			res = (left[3])-(right[3]);
			if (res) return res;

			res = (left[2])-(right[2]);
			if (res) return res;

			res = (left[1])-(right[1]);
			if (res) return res;

			res = (left[0])-(right[0]);
			if (res) return res;
		}
	}

	return 0;
}

int Characters::CompareReverse(const char * left, const int leftSize, const char * right, const int rightSize)
{
	int res = CompareReverse(left,right,(leftSize<rightSize)?leftSize:rightSize);
	return (res)?res:leftSize-rightSize;
}

int Characters::CompareReverseCaseless(const char * left, const char * right)
{
	int res;
	int leftSize = Length(left);
	int rightSize = Length(right);
	res = CompareReverseCaseless(left,right,(leftSize<rightSize)?leftSize:rightSize);
	return (res)?res:leftSize-rightSize;
}

int Characters::CompareReverseCaseless(const char * left, const char * right, const int size)
{
	int res;
	left+=size;
	right+=size;

	for (int index = (size&0x00000003);index>0;--index)
	{
		res = Characters::Caseless(*--left)-Characters::Caseless(*--right);
		if (res) return res;
	}

	for (int index = (size>>2);index>0;--index)
	{
		if (*(int*)(left-=4) != *(int*)(right-=4))
		{
			res = Characters::Caseless(left[3])-Characters::Caseless(right[3]);
			if (res) return res;

			res = Characters::Caseless(left[2])-Characters::Caseless(right[2]);
			if (res) return res;

			res = Characters::Caseless(left[1])-Characters::Caseless(right[1]);
			if (res) return res;

			res = Characters::Caseless(left[0])-Characters::Caseless(right[0]);
			if (res) return res;
		}
	}

	return 0;

}

int Characters::CompareReverseCaseless(const char * left, const int leftSize, const char * right, const int rightSize)
{
	int res = CompareReverseCaseless(left,right,(leftSize<rightSize)?leftSize:rightSize);
	return (res)?res:leftSize-rightSize;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Characters::Equals(const char * left, const char * right)
{

	if (left == right) return true;

	while (true)
	{
		char l = *left++;
		char r = *right++;

		if ((l == r) && (l == 0 || r == 0)) return true;		

		if (l != r) return false;
	}

}

bool Characters::Equals(const char * left, const char * right, const int size)
{

	Assert(size >= 0);

	if ((left == 0 || right == 0) && left != right) return false;	
	if (left == right || size == 0) return true;	
	if (left[size-1] != right[size-1] || left[0] != right[0]) return false;

	if (size > 2)
	{
		for (int index = (size>>2);index>0;--index)
		{
			if (*(int*)left != *(int*)right) return false;

			left+=4;
			right+=4;
		}

		for (int index = (size&0x00000003);index>0;--index)
		{
			if ((*left++)!=(*right++)) return false;
		}
	}

	return true;
}

bool Characters::Equals(const char * left, const int leftSize, const char * right, const int rightSize)
{
	if (leftSize != rightSize) return false;
	if (left == right) return true;
	return Equals(left,right,rightSize);
}

bool Characters::EqualsCaseless(const char * left, const char * right)
{
	if (left == right) return true;
	return Characters::CompareCaseless(left,right)==0;
}

bool Characters::EqualsCaseless(const char * left, const char * right, const int size)
{

	Assert(size >= 0);

	if ((left == 0 || right == 0) && left != right) return false;
	if (left == right || size == 0) return true;
	if (Characters::Caseless(left[size-1]) != Characters::Caseless(right[size-1])) return false;

	for (int index=(size>>2);index>0;--index)
	{
		if (*(int*)left != *(int*)right)
		{
			if (Characters::Caseless(*left++)!=Characters::Caseless(*right++)) return false;
			if (Characters::Caseless(*left++)!=Characters::Caseless(*right++)) return false;
			if (Characters::Caseless(*left++)!=Characters::Caseless(*right++)) return false;
			if (Characters::Caseless(*left++)!=Characters::Caseless(*right++)) return false;
		}
		else
		{
			left+=4;
			right+=4;
		}
	}

	for (int index=(size&0x00000003);index>0;--index)
	{
		if (Characters::Caseless(*left++) != Characters::Caseless(*right++)) return false;
	}

	return true;
}

bool Characters::EqualsCaseless(const char * left, const int leftSize, const char * right, const int rightSize)
{
	if (leftSize != rightSize) return false;
	if (left == right) return true;
	return EqualsCaseless(left,right,rightSize);
}

bool Characters::EqualsReverse(const char * left, const char * right)
{
	if (left == right) return true;
	int leftSize = Length(left);
	int rightSize = Length(right);
	if (leftSize != rightSize) return false;
	return EqualsReverse(left,right,rightSize);
}

bool Characters::EqualsReverse(const char * left, const char * right, const int size)
{

	Assert(size >= 0);

	if ((left == 0 || right == 0) && left != right) return false;
	if (left == right || size == 0) return true;
	if (left[0] != right[0]) return false;

	left+=size;
	right+=size;

	for (int index = (size&0x00000003);index>0;--index)
	{
		if ((*--left) != (*--right)) return false;
	}

	for (int index = (size>>2);index>0;--index)
	{
		if (*(int*)(left-=4) != *(int*)(right-=4))
			return false;
	}

	return true;
}

bool Characters::EqualsReverse(const char * left, const int leftSize, const char * right, const int rightSize)
{
	if (leftSize != rightSize) return false;
	if (left == right) return true;
	return EqualsReverse(left,right,rightSize);
}

bool Characters::EqualsReverseCaseless(const char * left, const char * right)
{
	if (left == right) return true;
	return Characters::CompareReverseCaseless(left,right)==0;
}

bool Characters::EqualsReverseCaseless(const char * left, const char * right, const int size)
{
	if (left == right) return true;
	if (Characters::Caseless(left[0]) != Characters::Caseless(right[0])) return false;

	left+=size;
	right+=size;

	for (int index = (size&0x00000003);index>0;--index)
	{
		if (Characters::Caseless(*--left) != Characters::Caseless(*--right)) return false;
	}

	for (int index = (size>>2);index>0;--index)
	{
		if (*(int*)(left-=4) != *(int*)(right-=4))
		{
			if (Characters::Caseless(left[3])!=Characters::Caseless(right[3])) return false;
			if (Characters::Caseless(left[2])!=Characters::Caseless(right[2])) return false;
			if (Characters::Caseless(left[1])!=Characters::Caseless(right[1])) return false;
			if (Characters::Caseless(left[0])!=Characters::Caseless(right[0])) return false;
		}
	}

	return true;
}

bool Characters::EqualsReverseCaseless(const char * left, const int leftSize, const char * right, const int rightSize)
{
	if (leftSize != rightSize) return false;
	if (left == right) return true;
	return EqualsReverseCaseless(left,right,rightSize);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Characters::Length(const char * data)
{

	return (data)?(int)strlen(data):0;
}

int Characters::Hash(char *data, int size)
{

	unsigned int key = 5381;

	char * eof = data+size;
	for (char * c = data;c && c < eof;)

		key = ((key << 5) + key) + Hashing::Djb2Hash::Ascii[*c++]; 

	return key;
}

int Characters::HashCaseless(char *data, int size)
{

	unsigned int key = 5381;

	char * eof = data+size;
	for (char * c = data;c && c < eof;)
		key = ((key << 5) + key) + Hashing::Djb2Hash::Ascii[Characters::Caseless(*c++)]; 

	return key;
}

bool Characters::IsUppercase(char *data, int size)
{
	if ( data==0 || !(size>0) ) return false;

	for (int i=0;i<size;++i)
	{
		if (! isupper(data[i]))
			return false;
	}

	return true;
}

bool Characters::IsLowercase(char *data, int size)
{
	if ( data==0 || !(size>0) ) return false;

	for (int i=0;i<size;++i)
	{
		if (!isupper(data[i]))
			return false;
	}

	return true;
}

bool Characters::IsProppercase(char *data, int size)
{
	if ( data==0 || !(size>0) || !isupper(*data)) return false;

	for (int i=1;i<size;++i)
	{
		if (!islower(data[i]))
			return false;
	}

	return true;
}

bool Characters::IsAlpha(char *data, int size)
{
	if ( data==0 || !(size>0) ) return false;

	for (int i=0;i<size;++i)
	{

		if ((data[i]|0x20) < 'a' || (data[i]|0x20) > 'z')
			return false;
	}

	return true;
}

bool Characters::IsNumeric(char *data, int size)
{
	if ( data==0 || !(size>0) ) return false;

	for (int i=0;i<size;++i)
	{
		if (!isdigit(data[i]))
			return false;
	}

	return true;
}

bool Characters::IsAlphanumeric(char *data, int size)
{
	if ( data==0 || !(size>0) ) return false;

	for (int i=0;i<size;++i)
	{
		if (!isalnum(data[i]))
			return false;
	}

	return true;
}

bool Characters::IsBinary(char * data, int size)
{
	if ( data==0 || !(size>0) ) return false;

	for (char * eof=data+size;data != eof;++data)
		if (*data != '1' && *data != '0')
			return false;

	return true;
}

long long Characters::Binary(char * data, int size)
{
	long long bin;
	Binary(bin,data,size);
	return bin;
}

bool Characters::Binary(long long & value, char * data, int size)
{
	if ( data==0 || !(size>0) ) return false;

	const char * bof = data;
	const char * eof = data+size;

	long long res = 0;
	long long sign=1;

	if (*data == '-')
	{
		++data;--size;
		sign=-1;
	}
	else
	if (*data == '+')
	{
		++data;--size;
	}

	--size;
	while (size > -1)
	{
		res <<= 1;
		if (*data == '1')
			res |= 1;
		else
		if (*data == '0')
			res |= 0;
		else
			break;

		++data;--size;
	}

	value = res * sign;
	return data != bof;
}

bool Characters::IsOctal(char *data, int size)
{
	if ( data==0 || !(size>0) ) return false;

	for (char * eof=data+size;data != eof;++data)
		if (!(*data >= '0' && *data <= '7'))
			return false;

	return true;
}

long long Characters::Octal(char * data, int size)
{
	long long value;
	Octal(value,data,size);
	return value;
}

bool Characters::Octal(long long & value, char *data, int size)
{
	if ( data==0 || !(size>0) ) return 0;

	const char * bof = data;
	const char * eof = data+size;

	long long res = 0;
	long long sign=1;

	if (*data == '-')
	{
		++data;--size;
		sign=-1;
	}
	else
	if (*data == '+')
	{
		++data;--size;
	}

	if (size > 1 && data[0] == '0' && (data[1]|0x20) == 'x')
	{
		data += 2;size -= 2;
	}
	else	
	if (size > 0 && data[0] == '0')
	{
		++data;--size;
	}

	--size;

	while (size > -1)
	{
		if (Characters::IsOctal(*data))
		{

			res = res*8 + (*data-0x30);
		}
		else
		{
			break;
		}

		++data;--size;
	}

	value =  res * sign;
	return data != bof;
}

bool Characters::IsHex(char *data, int size)
{
	if ( data==0 || !(size>0) ) return false;

	for (char * eof=data+size;data != eof;++data)
		if (!(isdigit(*data) ||
			(*data == 'A') || (*data == 'B') || (*data == 'C') || (*data == 'D') || (*data == 'E') || (*data == 'F') ||
			(*data == 'a') || (*data == 'b') || (*data == 'c') || (*data == 'd') || (*data == 'e') || (*data == 'f')
			)) return false;

	return true;
}

long long Characters::Hex(char * data, int size)
{
	long long value;
	Hex(value,data,size);
	return value;
}

bool Characters::Hex(long long & value, char *data,int size)
{
	if ( data==0 || !(size>0) ) return 0;

	const char * bof = data;
	const char * eof = data+size;

	long long res = 0;
	long long sign=1;

	if (*data == '-')
	{
		++data;--size;
		sign=-1;
	}
	else
	if (*data == '+')
	{
		++data;--size;
	}	

	if (size > 1 && data[0] == '0' && (data[1]|0x20) == 'x')
	{
		data += 2;size -= 2;
	}
	else
	if (size > 0 && data[0] == '#')
	{
		++data;--size;
	}

	--size;

	while (size > -1)
	{
		if (isdigit(*data))
		{

			res = res*16 + (*data-0x30);
		}
		else
		if (Characters::IsHex(*data))
		{

			res = res*16 + ((*data|0x20)-0x57);
		}
		else
		{
			break;
		}

		++data;--size;
	}

	value = res * sign;

	return data != bof;
}

long long Characters::Decimal(char * data, int size)
{
	long long value;
	Decimal(value,data,size);
	return value;
}

bool Characters::Decimal(long long & value, char *data,int size)
{
	if ( data==0 || !(size>0) )
	{

		return 0;
	}

	const char * bof = data;
	const char * eof = data+size;

	long long res=0;
	long long sign=1;

	if (*data == '-')
	{
		++data;--size;
		sign=-1;
	}
	else
	if (*data == '+')
	{
		++data;--size;
	}

	while(data+4 <= eof)
	{	

		if ( (*data&0xF0) != 0x30 || (*data&0x0F) > 9 )	break;
		res = res*10 + (*(data++)-0x30);

		if ( (*data&0xF0) != 0x30 || (*data&0x0F) > 9 )	break;
		res = res*10 + (*(data++)-0x30);

		if ( (*data&0xF0) != 0x30 || (*data&0x0F) > 9 )	break;
		res = res*10 + (*(data++)-0x30);

		if ( (*data&0xF0) != 0x30 || (*data&0x0F) > 9 )	break;				
		res = res*10 + (*(data++)-0x30);
	}

	while(data != eof)
	{	
		if ( (*data&0xF0) != 0x30 || (*data&0x0F) > 9 )	
			break;

		res = res*10 + (*(data++)-0x30);
	}

	if (data == bof)
	{

	}

	value = res * sign;

	return data != bof;

}

long long Characters::Integer(int radix, char * data, int size)
{
	long long value;
	Integer(value,radix,data,size);
	return value;
}

bool Characters::Integer(long long & value, int radix, char * data, int size)
{

	bool res=0;

	switch(radix)
	{
	case 0: res = Characters::Integer(value,data,size);break; 
	case 2: res = Characters::Binary(value,data,size);break;
	case 8: res = Characters::Octal(value,data,size);break;
	case 10: res = Characters::Decimal(value,data,size);break;
	case 16: res = Characters::Hex(value,data,size);break;
	}

	return res;
}

long long Characters::Integer(char * data, int size)
{
	long long value;
	Integer(value,data,size);
	return value;
}

bool Characters::Integer(long long & value, char *data,int size)
{

	if ( data==0 || !(size>0) ) return 0;

	const char * bof = data;
	const char * eof = data+size;

	int radix = 0;
	long long res=0;

	long long sign=1;
	if (*data == '-')
	{
		++data;--size;
		sign=-1;
	}
	else
	if (*data == '+')
	{
		++data;--size;
	}

	if (size > 0 && *data >= '1' && *data <= '9')
	{
		radix = 10;
	}
	else
	if (size > 0 && *data == '0')
	{
		++data;--size;
		if (size >0)
		{
			if (*data >= '0' && *data <= '7')
			{
				radix = 8;
			}
			else
			if (*data == 'x' || *data == 'X')
			{
				++data;--size;
				radix = 16;
			}
		}
	}

	switch (radix)
	{
	case 8: res = Characters::Octal(data,size);break;
	case 10: res = Characters::Decimal(data,size);break;
	case 16: res = Characters::Hex(data,size);break;
	}

	value = res * sign;

	return data != bof;
}

double Characters::Real(char * data, int size)
{
	double value;
	Real(value,data,size);
	return value;
}

bool Characters::Real(double & value, char * data,int size)
{
	if ( data==0 || !(size>0) )
	{
		value = Number::Nan();
		return false;
	}

	const char * bof = data;
	const char * eof = data+size;

	double res = 0.0;

	char * integer		=0;
	int	   integerSize	=0;
	double integerSign	=1;	
	char * fraction		=0;
	int	   fractionSize	=0;
	char * exponent		=0;
	int    exponentSize =0;
	double exponentSign	=1;

	int i=0;

	if (data[i] == '+')
	{
		++i;
	}
	else
	if (data[i] == '-')
	{
		++i;
		integerSign = -1;
	}

	if (i < size && Characters::IsNumeric(data[i]))
	{
		integer = (char*) &data[i];
		while (i < size)
		{
			if (Characters::IsNumeric(data[i]))
			{
				++integerSize;
			}
			else

			{
				break;
			}

			++i;
		}
	}

	if (i < size && data[i] == '.')
	{
		++i;

		if (i < size && Characters::IsNumeric(data[i]))
		{

			fraction = (char*) &data[i];
			while (i < size)
			{
				if (Characters::IsNumeric(data[i]))
				{
					++fractionSize;
				}
				else
				{
					break;
				}

				++i;
			}
		}

	}

	if (i < size && (integerSize > 0 || fractionSize > 0) && (data[i]|0x20) == 'e')
	{

		++i;

		if (data[i] == '+')
		{
			++i;
		}
		else
		if (data[i] == '-')
		{
			++i;
			exponentSign = -1;
		}

		if (i < size && Characters::IsNumeric(data[i]))
		{
			exponent = (char*) &data[i];
			while (i < size)
			{
				if (Characters::IsNumeric(data[i]))
				{
					++exponentSize;
				}
				else
				{
					break;
				}

				++i;
			}
		}

	}

	if (integerSize == 0 && fractionSize == 0 )
	{
		value = Number::Nan();
		return false;
	}

	res = Characters::Decimal(integer,integerSize);

	if (fractionSize > 0)
	{

		long long fractionValue = Characters::Decimal(fraction,fractionSize);
		res += fractionValue * (double)pow((double)10,-fractionSize);
	}

	res *= integerSign;

	if (exponentSize > 0)
	{
		res = res * (double)pow((double)10,Characters::Decimal(exponent,exponentSize) * exponentSign);
	}

	value = res;

	return integerSize > 0; 
}

bool Characters::Boolean(char * data, int size)
{
	bool value;
	Boolean(value, data, size);
	return value;
}

bool Characters::Boolean(bool & value, char * data, int size)
{
	if (CompareCaseless(data,size,"true",4) || CompareCaseless(data,size,"1",1))
		return value=true;

	if (CompareCaseless(data,size,"false",5) || CompareCaseless(data,size,"0",1))
		return value=false;

	if (data && *data != 0)
		return value=true;

	return value=false;
}

void Characters::Lowercase(char *data, int size)
{
	if ( data==0 || !(size>0) ) return;

	{
		char * p = (char*)data;
		char * s = (char*)data + size;
		do
		{
			if (Characters::IsUppercase(*p))
				*p |= 0x20;
		}
		while (++p != s);
	}
}

void Characters::Uppercase(char *data, int size)
{

	{
		char * p = (char*)data;
		char * s = (char*)data + size;
		do
		{
			if (Characters::IsLowercase(*p))
				*p &= ~0x20;
		}
		while (++p != s);
	}
}

void Characters::Proppercase(char *data, int size)
{
	if ( data==0 || !(size>0) ) return;

	{

		char * p = (char*)data;
		char * s = (char*)data + size;

		while (p != s)
		{	
			if (Characters::IsAlpha(*p))
			{
				*p++ &= ~0x20;
				while (p != s && Characters::IsAlpha(*p))
					*p++ |= 0x20;
			}
			else
			{
				++p;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char * Characters::Find(const char * left, const char * right)
{
	Raise("Characters::FindReverseCaseless - Method not implemented\n");
	AssertBreak();
	return (char*)strstr(left,right);
}

char * Characters::Find(const char * left, const char * right, const int size)
{
	Raise("Characters::FindReverseCaseless - Method not implemented\n");
	AssertBreak();
	return 0;
}

char * Characters::Find(const char * left, const int leftSize, const char * right, const int rightSize)
{
	if (right == 0 || rightSize < 1 || rightSize > leftSize) return 0;

	int limit = leftSize-rightSize;
	for(int n=0;n <= limit;++n)
	{
		if (left[n] == right[0] && left[n+rightSize-1] == right[rightSize-1])
			if (Characters::Equals((left+n),right,rightSize)) return (char *) (left+n);
	}

	return 0;	
}

char * Characters::FindCaseless(const char * left, const char * right)
{
	Raise("Characters::FindReverseCaseless - Method not implemented\n");
	AssertBreak();
	return 0;
}

char * Characters::FindCaseless(const char * left, const char * right, const int size)
{
	Raise("Characters::FindReverseCaseless - Method not implemented\n");
	AssertBreak();
	return 0;
}

char * Characters::FindCaseless(const char * left, const int leftSize, const char * right, int rightSize)
{
	if (right == 0 || rightSize < 1 || rightSize > leftSize) return 0;

	int limit = leftSize-rightSize;
	for(int n=0;n <= limit;++n)
	{
		if ( Characters::Caseless(left[n]) == Characters::Caseless(right[0]) && Characters::Caseless(left[n+rightSize-1]) == Characters::Caseless(right[rightSize-1]) )
			if (Characters::EqualsCaseless((left+n),right,rightSize)) return (char *) (left+n);
	}

	return 0;	
}

char * Characters::FindReverse(const char * left, const char * right)
{
	Raise("Characters::FindReverseCaseless - Method not implemented\n");
	AssertBreak();
	return 0;
}

char * Characters::FindReverse(const char * left, const char * right, const int size)
{
	Raise("Characters::FindReverseCaseless - Method not implemented\n");
	AssertBreak();
	return 0;
}

char * Characters::FindReverse(const char * left, const int leftSize, const char * right, int rightSize)
{
	if (right == 0 || rightSize < 1 || rightSize > leftSize)
		return 0;

	int limit = leftSize-rightSize;
	for(int n=limit;n >= 0;--n)
	{
		if (left[n] == right[0] && left[n+rightSize-1] == right[rightSize-1])
			if (Characters::Equals((left+n),right,rightSize)) return (char *) (left+n);
	}

	return 0;	
}

char * Characters::FindReverseCaseless(const char * left, const char * right)
{
	Raise("Characters::FindReverseCaseless - Method not implemented\n");
	AssertBreak();
	return 0;
}

char * Characters::FindReverseCaseless(const char * left, const char * right, const int size)
{
	Raise("Characters::FindReverseCaseless - Method not implemented\n");
	AssertBreak();
	return 0;
}

char * Characters::FindReverseCaseless(const char * left, const int leftSize, const char * right, int rightSize)
{

	if (right == 0 || rightSize < 1 || rightSize > leftSize)
		return 0;

	int limit = leftSize-rightSize;
	for(int n=limit;n >= 0;--n)
	{
		if ( Characters::Caseless(left[n]) == Characters::Caseless(right[0]) && Characters::Caseless(left[n+rightSize-1]) == Characters::Caseless(right[rightSize-1]) )
			if (Characters::EqualsCaseless((left+n),right,rightSize)) return (char *) (left+n);
	}

	return 0;	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Characters::Format(char * data, int size, const char * format, ...)
{
	va_list va;
	va_start(va, format);
	int count = FormatVa(data,size,format,va);
	va_end(va);
	return count;
}

int Characters::FormatVa(char * data, int size, const char * format, va_list va)		
{

	#ifdef HERO_PLATFORM_WINDOWS

	#if defined(_MSC_VER) && (_MSC_VER <= 1310)
	String reformat;
	char * ll = 0;
	if ((ll = (char*)strstr(format,"%ll")) != 0)
	{
		reformat.Construct(format);
		int index = ll-format;
		reformat.Replace(index,index+4,"%I64d");
		format = reformat.Data;
	}
	#endif

	size = (size <= 0)?0:size;
	int count = _vsnprintf (data,size,format,va); 

	#endif

	#ifdef HERO_PLATFORM_POSIX

	va_list val;
	va_copy(val,va);

	size = (size <= 0)?0:size;
	int count = vsnprintf (data,size,format,val);
	va_end(val);

	#endif

	if (count > -1 && count < size)
		data[count] = 0;

	return count;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Identity Bytes::Instance;

char & Bytes::ReferenceAt(int index)
{	
	Indices::Normalise(index,Size);
	Assert(Data != 0);
	Assert(index >= 0 && index <= Size);

	static char null=0;
	return (Data)?*(Data+index):null;
}

char * Bytes::PointerAt(int index)
{
	if (Indices::Normalise(index,Size))
	{

		Assert(Data != 0);
		Assert(index >= 0);

		return Data+index;
	}

	return 0;
}

Bytes & Bytes::operator = (const Bytes & bytes)
{
	Data = bytes.Data;
	Size = bytes.Size;
	return *this;
}

void Bytes::Print(class String & string)
{
	string.Append(Data,Size);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Identity Substring::Instance;

Substring & Substring::Move(Substring & str) 
{
    Data = str.Data;Size = str.Size;
    str.Data = 0;str.Size = 0;
    return *this;
}

Substring & Substring::Swap(Substring & str) 
{
    char * data = Data;int size = Size;
    Data = str.Data;Size = str.Size;
    str.Data = data;str.Size = size;
    return *this;        
}

Substring & Substring::Left(int amount)
{

	Assert((Size > 0 && Data != 0) || Size == 0);

	if ((Size+amount) < 0)
		amount = Size;

	if (Data != 0)
	{
		Data -= amount;
		Size += amount;
	}

	return *this;
}

Substring & Substring::Right(int amount)
{
	Assert((Size > 0 && Data != 0) || Size == 0);

	if ((Size+amount) < 0)
		amount = Size;

	if (Data != 0)
	{	
		Size += amount;
	}

	return *this;
}

bool Substring::Contains(char *data, int size, bool caseless)
{ 
	if (Data == 0) return 0; 

	return Characters::Find(Data,Size,data,size,caseless) != 0;
}

int Substring::Match(int from, char * data, int size)
{
	if (Indices::Normalise(from,Size))
	{
		Regex regex(data,size);
		if (regex.Match(Data+from,Size-from))
			return regex.Matches[0][0].Data-Data;
	}

	return -1;
}

int Substring::Match(int from, Regex & regex)
{

	if (Indices::Normalise(from,Size))
	{
		if (regex.Match(Data+from,Size-from))
			return regex.Matches[0][0].Data-Data;
	}

	return -1;
}

bool Substring::Is(char *data, int size, bool caseless)
{
	return (caseless)?Characters::EqualsCaseless(Data,Size,data,size):Characters::Equals(Data,Size,data,size);
}

bool Substring::Is(char data, bool caseless)
{
	if (Size==0||Size>1) return false;
	return (caseless)?Characters::Caseless(*Data)==Characters::Caseless(data):*Data==data;
}

Substring & Substring::Trim(int amount)
{
	LTrim(amount);
	RTrim(amount);
	return *this;
}

Substring & Substring::LTrim(int amount)
{	

	if (amount > 0)
		Left(-amount);
	return *this;
}

Substring & Substring::RTrim(int amount)
{

	if (amount > 0)
		Right(-amount);
	return *this;
}

Substring & Substring::Trim(char * matches, int matchesSize) 
{
    LStrip(matches,matchesSize);
    RStrip(matches,matchesSize);
    return *this;
}

Substring & Substring::LTrim(char * matches, int matchesSize) 
{
	if (IsEmpty()) return *this;

	char *lhs = Data;
	char *rhs = Data+Size;
	char *match = 0;

	while (lhs != rhs)
	{

		match = lhs;
		for (int m=0;match != rhs && m < matchesSize;++m)
			if (*match == matches[m])
				++match;

		if (match == lhs)
			break;

		lhs = match;
	}

	if (lhs != Data)
		Left((int)(Data-lhs));

	return *this;
}

Substring & Substring::RTrim(char * matches, int matchesSize) 
{
	if (IsEmpty()) return *this;

	char *rhs = (Data+Size-1);
	char *lhs = Data-1;
	char *match = 0;

	while (rhs != lhs)
	{

		match = rhs;
		for (int m=0;match != lhs && m < matchesSize;++m)
			if (*match == matches[m])
				--match;

		if (match == rhs)
			break;

		rhs = match;
	}

	if (rhs != (Data+Size-1))
		Right((int)(rhs-(Data+Size-1)));

	return *this;    
}

int Substring::IndexOf(int from, char * data, int size, bool caseless) 
{
    if (from >= Size) return -1;

    char * offset = Characters::LFind(Data+from,Size-from,data,size,caseless);
    if (offset) return (int)(offset-Data);

    return -1;

}

int Substring::LastIndexOf(int from, char *data, int size, bool caseless)
{
	Indices::Normalise(from,Size);

	if (from >= 0)
	{
		char * offset = Characters::FindReverse(Data,Min(Size,from+1),data,size,caseless);
		if (offset)
			return offset-Data;
	}

	return -1;
}

int Substring::LastIndexOf(int from, const char c,  bool caseless)
{
	Indices::Normalise(from,Size);

	if (from >= 0)
	{

		char *offset = Data+Min(Size-1,from);
		if (caseless)
		{
			while (offset != Data)
			{
				if (Characters::Caseless(*offset) == Characters::Caseless(c)) 
					return offset-Data;
				--offset;
			}
		}
		else
		{
			while (offset != Data)
			{
				if (*offset == c) 
					return offset-Data;
				--offset;
			}
		}
	}

	return -1;
}

bool Substring::StartsWith(char * data, int size, bool caseless) {
	if (Size == 0 || Size < size || size == 0) return false;

	return Characters::Equals(Data,data,size,caseless);    
}

bool Substring::EndsWith(char * data, int size, bool caseless) {
	if (Size == 0 || Size < size || data == 0) return false;

	char *end = Data+Size-size;
	return Characters::Equals(end,data,size,caseless);
}

int Substring::Hash()
{
	return Characters::Hash(Data,Size);
}

int Substring::Compare(Object *object,int comparitor)
{

	if (this == object) return 0;

	if (object->InstanceOf(Substring::Instance))
	{
		Substring * str = ((Substring *)object);

		switch(comparitor)
		{
		case Comparable::COMPARE_GENERAL:case Comparable::COMPARE_PRECISE:
			return Characters::Compare(Data,Size,str->Data,str->Size,!comparitor);
		default:
			return this - str;
		}
	}
	else
	{
		Raise("Substring::Compare - Invalid object type for comparison.\n");
		return Identity::Error;
	}
}

bool Substring::Equals(Object * object,int comparitor)
{
	if (this == object) return true;

	if (object->InstanceOf(this))
	{
		Substring * str = ((Substring *)object);

		switch(comparitor)
		{
		case Comparable::COMPARE_GENERAL:
		case Comparable::COMPARE_PRECISE:
			return Characters::Equals(Data,Size,str->Data,str->Size,!comparitor);
		default:
			return false;
		}
	}
	else
	{
		Raise("Substring::Equals - ERROR: Invalid object type for equality.\n");
		return false;
	}
}

void Substring::Print(class String & string)
{
	string.Append(Data,Size);
}

Substring Substring::Slice(int from)
{

	if (Indices::Normalise(from,Size))
		return Substring(Data+from,Size-from);

	return Substring();
}

Substring Substring::Slice(int from, int to)
{

	Indices::Normalise(from,Size);
	Indices::Normalise(to,Size);

	if (from >= 0 && from < Size && to > from)
		return Substring(Data+from,Min(Size,to)-from);

	return Substring();
}

Superstring Substring::Super() 
{
	return Superstring(Data,Size);
}

String Substring::Str() 
{
	return String(Data,Size);
}		

#ifdef HERO_USING_STD

int Substring::Write(std::ostream & stream, int amount) {
    amount = (amount > Size)?Size:amount;
    if (amount == 0) amount = Size;
    stream.write(Data,amount);
    return amount;
}

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

String::String(const Substring & str, int from, int to):Allocated(0)
{
	Construct(Superstring(str,from,to));
}

String::String(const Substring & str, int from):Allocated(0)
{
	Construct(Superstring(str,from));
}

String & String::Construct()
{

	if (IsSubstring())
	{
		if (Allocated)
		{
			Construct("");
		}
		else
		{
			char * data = Data;
			int size = Size;
			Data = 0;
			Size = 0;
			Allocated = 0;

			if (data && size > 0)
				Construct(data,size);
		}

	}

	return *this;
}

String & String::Construct(const Superstring & str)
{
	return Attach((String&)str);	
}

String & String::Construct(const Substring &str)
{

	if (((Substring&)str).Identify() == Superstring::Instance)
		return Attach((String&)str);	
	else
		return Construct(str.Data,str.Size);
}

String & String::Construct(char * data, int size)
{
	#ifdef HERO_USING_TRACY

	#endif

	Assert(Data == 0 || (data < Data || data >= (Data+Size)));
	if (data >= Data && data < (Data+Size)) return *this;

	Stackstring stack;
	if (data >= Data && data < (Data+Size))
	{
		stack.Construct(data,size);
		data = stack.Data;
		size = stack.Size;
	}

	Assert((data == 0 && size == 0) || (data != 0 && size >= 0)); 

	if (data == 0) 
	{

		Destroy();
		return *this;
	}
	else
	{

	}

	if (size != Size || Data == 0)
		Resize(size);

	if (data && size > 0)
	{

		memcpy(Data,data,size);
	}

	return Term();
}

String & String::Construct(int i)
{
	return Construct((long long)i);
}

String & String::Construct(long long ll)
{
	String string;
	string.Allocate(32);
	#ifdef HERO_PLATFORM_WINDOWS
	string.Format("%I64d",ll);
	#else
	string.Format("%lld",ll);
	#endif	

	return Construct(string);

}

String & String::Construct(double d)
{
	String string;
	string.Allocate(512);
	string.Format("%f",d);
	return Construct(string);

}

String & String::Construct(float f)
{
	String string;
	string.Allocate(64);
	string.Format("%g",f);
	return Construct(string);

}

String & String::Construct(char c)
{	
	Allocate(1);
	Data[0]=c;			
	Size=1;
	return Term();
}

String & String::Construct(void *p)
{
	FormatValue(0,"%p",p);
	return *this;
}

String & String::Construct(int times, char * data, int size)
{
	if (times > 0)
	{
		int pow = 0;
		Size = 0;
		Allocate(size*times);
		Append(data,size);
		if (size > 0 && *data != 0)
		{
			while ((1 << (pow+1)) < times)
			{
				Append(*this);
				++pow;
			}

			times = times - (1<<pow);
			while (times-- > 0)
			{
				Append(data,size);
			}
		}
	}

	return *this;
}

void String::Destroy()
{
	if (Allocated) 
	{
		Assert(Data);
		delete[] Data;        
	}

	Data = 0;
	Size = 0;
	Allocated = 0;
}

void String::Release()
{
	if (Allocated)
	{

		Size = 0;	
		Assert(Data != 0);
		Term();
	}
	else
	{
		Data = 0;
		Size = 0;
		Allocated = 0;
	}
}

String & String::Move(Substring & str) 
{
    Reset();
    Data = str.Data;Size = str.Size;
    str.Data = 0;str.Size = 0;
    return *this;
}

String & String::Move(String & str) 
{
    Reset();
    Data = str.Data;Size = str.Size;Allocated = str.Allocated;
    str.Data = 0;str.Size = 0;str.Allocated = 0;
    return *this;
}

String & String::Swap(Substring & str) 
{

    Reset();
    char * data = Data;int size = Size;
    Data = str.Data;Size = str.Size;
    str.Data = data;str.Size = size;
    return *this;        
}    

String & String::Swap(String & str) 
{
    char * data = Data;int size = Size; int capacity = Allocated;
    Data = str.Data;Size = str.Size;Allocated = str.Allocated;
    str.Data = data;str.Size = size;str.Allocated= capacity;
    return *this;        
}    

char * String::Print() 
{
	Assert(Allocated == 0 || Data != 0);
	Assert(Size >= 0);

	static char * null = (char*)"";
	if (Data == 0) return null;

	return (Data == 0 || (Allocated && Data[Size]==0))?Data:Printable::Print();
}

String & String::Resize(int amount) 
{

	if (!Allocated) 
	{	

		if (Data != 0 && Size > 0)
			Size = amount;

		Reserve(amount);
	}

	if (amount == 0 || amount >  0)
	{

		return Right(amount-Size);
	}
	else
	if (amount < 0)
	{
		Assert(amount >= 0);		
	}

	return *this;
}

int String::Remaining()
{

	return (Allocated-Size-1);
}

String & String::Allocate(int amount)
{

	Assert((Size > 0 && Data != 0) || (Size == 0));

	if (amount == 0)
	{

		int remaining = Remaining();	
		if (Allocated > 0)
			amount = Allocated * (1 + Increment());		
		else
		if (Size > 0)
			amount = Size;
		else
			amount = 1;
	}

	if (amount < 0 || amount < Size)
		amount = Size;

	amount += 1;	

	char * data = new char[amount];
	if (Size > 0) 
		memcpy(data,Data,sizeof(char)*Size);

	if (Allocated)
		delete [] Data;

	Data = data;
	Allocated = amount;

	Term();

	return *this;
}

String & String::Reserve(int amount) 
{
	Assert((Size > 0 && Data != 0) || (Size == 0));

    int remaining = Remaining();

	if (amount == 0)
	{

        if (Allocated > 0)
            amount = Allocated * (1 + Increment());
        else
        if (Size > 0)
            amount = Size;
        else
            amount = 1;        
	}
	else
	if (amount < 0 )
	{
		amount = Size+(remaining-amount);
	}
	else
	if (amount > 0)
	{
		if (amount <= remaining) return *this;

		amount = (int)(Size * (1 + Increment()) + amount);
	}

	return Allocate(amount);
}

String & String::Left(int amount)
{
	Assert((Size > 0 && Data != 0) || (Size == 0));

	if (!Allocated && (Data != 0))
	{	
		Assert(Size >= 0);
		Substring::Left(amount);
		return *this;
	}

	if (amount > 0)
	{
		Reserve(amount);

		Assert(Size+amount < Allocated);
		memmove(Data+amount,Data,Size);
	}
	else
	if (amount < 0)
	{
		if (!Allocated)
			return *this;

		Assert(Size < Allocated);
		memmove(Data,Data-amount,Size+amount);
	}
	else
	{
		return *this;
	}

	Size += amount; 
	Term();
	return *this;
}

String & String::Right(int amount)
{
	Assert((Size > 0 && Data != 0) || (Size == 0));

	if (!Allocated && (Data != 0))
	{	
		Assert(Size >= 0);
		Substring::Right(amount);
		return *this;
	}

	if (amount > 0)
	{
		Reserve(amount);
	}
	else
	if (amount < 0)
	{
		if (!Allocated)
			return *this;
	}
	else
	{

		return *this;
	}

	Assert(Size+amount < Allocated);
	Size += amount;
	Term();
	return *this;
}

String & String::Fill(char data) 
{
    if (Allocated) 
	{
        memset(Data,data,sizeof(char)*Allocated);
    }

    return *this;
}

String & String::Assign(char *data, int size)
{
	Assert((size > 0 && data != 0) || (size == 0));

	if (Allocated)
	{

		Assert(data+size <= Data || data >= Data+Size);
		if (data+size >= Data && data <= Data+Size) return *this;

		Destroy();
	}

	Data = (char*)data;
	Size = (int)size;

	if (Size < 0) Size = 0;

	Allocated = 0;
	return *this;
}

String & String::Assign(char * data, int size, int allocated) 
{

    Assign(data,size);
	Allocated = allocated;
	return *this;
}

String & String::Assign(String & str, bool move) 
{
    if (move) return this->Move(str);

	Assign(str.Data,str.Size,str.Allocated);
	return *this;
}

String & String::Attach(char *& data, int & size, int & allocated)
{
	if (Allocated)
	{

		Assert(data+size <= Data || data >= Data+Size);
		if (data+size >= Data && data <= Data+Size) return *this;

		Destroy();
	}

	Data = data;
	Size = size;
	Allocated = allocated;

	data = 0;
	size = 0;
	allocated = 0;
	return *this;
}

String & String::Attach(char *& data, int & size)
{
	int allocated=size;
	return Attach(data,size,size);
}

String & String::Attach(const Substring & str)
{
	int allocated = str.Size;
	return Attach((char*&)str.Data,(int&)str.Size,allocated);
}

String & String::Attach(const String & str)
{
	return Attach((char*&)str.Data,(int&)str.Size,(int&)str.Allocated);
}

char * String::Detach() 
{
    if (Allocated) 
    {
        char * data = Data;
        Data = 0;
		Size = 0;
		Allocated = 0;
        return data;
    }

	Assert(Allocated != 0);
    return 0;        
}

String & String::Append(const Superstring & str)
{
	if (IsEmpty())
		return Construct(str);
	else
		return Append(str.Data,str.Size);
}

String & String::Prepend(const Superstring & str)
{
	if (IsEmpty())
		return Construct(str);
	else
		return Prepend(str.Data,str.Size);
}

String & String::Insert(char * data, int size, int index) 
{

    Assert((data == 0 && size == 0) || (data != 0 && size >= 0)); 

	if (data == 0 || size < 0) 	return *this;

	Stackstring string;
	if (data >= Data && data < (Data+Size))
	{
		string.Assign(data,size);
		data = string.Data;
		size = string.Size;
	}

	Indices::Normalise(index,Size);

	Assert(index >=0 && index <= Size);

	int offset = Size;

	Resize(Size+size);

	Assert(Data != 0);

	if (index == offset)
	{
		memcpy(Data+offset,data,size);
	}
	else
	{
		memmove(Data+index+size,Data+index,offset-index);
		memcpy(Data+index,data,size);
	}

	Term();
    return *this;
}

String & String::Insert(int i, int index)                  
{
	return FormatValue(index,"%d",i);
}

String & String::Insert(unsigned int i, int index)          
{
	return FormatValue(index,"%u",i);
}

String & String::Insert(long l, int index)					
{
	return FormatValue(index,"%ld",l);
}

String & String::Insert(unsigned long l, int index)         
{	
	return FormatValue(index,"%lu",l);
}

String & String::Insert(long long ll, int index)            
{
	#ifdef REASON_PLATFORM_WINDOWS
	return FormatValue(index,"%I64d",ll);
	#else
	return FormatValue(index,"%lld",ll);
	#endif	
}

String & String::Insert(unsigned long long ll, int index)   
{
	#ifdef REASON_PLATFORM_WINDOWS
	return FormatValue(index,"%I64u",ll);
	#else
	return FormatValue(index,"%llu",ll);
	#endif
}

String & String::Insert(float f, int index)
{
	String string;
	string.Allocate(64);
	string.Format("%g",f);
	return Insert(string,index);
}

String & String::Insert(double d, int index)                
{
	String string;
	string.Allocate(512);
	string.Format("%f",d);
	return Insert(string,index);
}

String & String::Insert(const void * p, int index)          
{
	return FormatValue(index,"%p",p);
}

String & String::Replace(char c) 
{
    Destroy();
    Append(c);
	return *this;
}

String & String::Replace(int i) 
{
	Reserve(32);
	Format("%d",i);
	return *this;
}

String & String::Replace(unsigned int i) 
{
	Reserve(32);
	Format("%u",i);
	return *this;
}

String & String::Replace(long l) 
{
	Reserve(32);
	Format("%ld",l);
	return *this;
}

String & String::Replace(unsigned long l) 
{
	Reserve(32);
	Format("%lu",l);
	return *this;
}

String & String::Replace(long long ll) 
{
	Reserve(64);
	#ifdef REASON_PLATFORM_WINDOWS
	Format("%I64d",ll);
	#else
	Format("%lld",ll);
	#endif
	return *this;
}

String & String::Replace(unsigned long long ll) 
{
	Reserve(64);	
	#ifdef REASON_PLATFORM_WINDOWS
	Format("%I64u",ll);
	#else
	Format("%llu",ll);
	#endif
	return *this;
}

String & String::Replace(float f)
{
	Reserve(64);
	Format("%g",f);
	return *this;
}

String & String::Replace(double d) 
{	
	Reserve(512);
	Format("%f",d);
	return *this;

}

String & String::Replace(const void * p) 
{
	Reserve(64);
	Format("%p",p);
	return *this;
}

String & String::Replace(char * data, int size) 
{
	return Construct(data,size);
}

String & String::Replace(int from, int to, char * data, int size)
{

	if (from < 0 || !(from <= Size)) return *this; 
	if (!(to >= from) || !(to <= Size)) return *this;

	if (!Allocated) Reserve();

	int diff = size-(to-from);

	int amount = Size;

	if (diff < 0)
	{
        memmove(Data+to+diff,Data+to,amount-to);
		Right(diff);
	}
	else
	if (diff > 0)
	{
		Right(diff);
		memmove(Data+to+diff,Data+to,amount-to);
	}

    memcpy(Data+from,data,size); 
	return Term();
}

String & String::Replace(int limit, char *match, int matchSize,char *data, int dataSize)
{
	if (IsEmpty())
		return *this;

	if (!Allocated) Reserve();

	int index = IndexOf(match,matchSize);
	int diff = dataSize - matchSize;

	if (limit > 0) ++limit;
	while (index != -1 && (limit == 0 || --limit > 0))
	{
		if (diff == 0)
		{

			memcpy(Data+index,data,dataSize);
		}
		else
		if (diff < 0)
		{

			memcpy(Data+index,data,dataSize);
			memmove(Data+index+dataSize,Data+index+matchSize,Size-(index+matchSize));

			Right(diff);
		}
		else
		if (diff > 0)
		{

			int size = Size;

			Right(diff);

			memmove(Data+index+dataSize,Data+index+matchSize,size - (Data+index+matchSize-Data));

			memcpy(Data+index,data,dataSize);
		}

		index = IndexOf(index+dataSize,match,matchSize);
	}

	return *this;
}

String & String::Replace(int limit, Regex & regex, char * data, int size)
{

	if (limit > 0) ++limit;

	regex.Match(limit,Data,Size);

	Array< Array< Pair<int,int> > > offsets;
	for (int m = 0;m < regex.Matches.Length() && (limit == 0 || --limit > 0);++m)
	{
		offsets.Append(Array< Pair<int,int> >());
		for (int mm = 0;mm < regex.Matches[m].Length();++mm)
			offsets[m] << Pair<int,int>(regex.Matches[m][mm].Data-Data,regex.Matches[m][mm].Size);
	}

	regex.Matches.Release();

	int shift = 0;
	for (int o = 0;o < offsets.Length() && (limit == 0 || --limit > 0);++o)
	{
		Substring matched = Substring(Data+offsets[o][0].First()+shift,offsets[o][0].Second());

		String string;
		StringParser parser(data,size);
		parser.Mark();
		while (!parser.Eof())
		{
			if (parser.Is('\\'))
			{
				parser.Trap();
				string << parser.Token;

				parser.Next();
				parser.Mark();
				if (parser.SkipDecimal())
				{
					parser.Trap();

					int capture = parser.Token.Trim().Decimal();

					if (capture > 0 && capture < offsets[o].Length())
					{
						string << Substring(Data+offsets[o][capture].First()+shift,offsets[o][capture].Second());
					}
					else
					{
						string << parser.Token;
					}

					parser.Mark();
				}
			}
			else
			{
				parser.Next();
			}
		}

		parser.Trap();
		string << parser.Token;

		shift += string.Size-matched.Size;

		Replace(matched.Data-Data,(matched.Data-Data)+matched.Size,string);
	}

	return *this;
}

String & String::Translate(char * matches, int matchesSize, char *replacements, int replacementsSize)
{
	if (!Allocated) Construct();

	for (int m=0;m<matchesSize;++m)
	{
		for(int d=0;d<Size;++d)
		{
			if (Data[d] == matches[m])
			{

				if ( ! (m < replacementsSize) )
				{

					memmove(PointerAt(d),PointerAt(d+1),PointerAt(Size)-PointerAt(d+1));
					Right(-1);

					--d;
				}
				else
				{
					Data[d] = replacements[m];
				}
			}
		}
	}

	return *this;
}

String & String::Trim(int amount)
{
	LTrim(amount);
	RTrim(amount);
	return *this;
}

String & String::LTrim(int amount)
{	

	if (amount > 0)
		Left(-amount);
	return *this;
}

String & String::RTrim(int amount)
{

	if (amount > 0)
		Right(-amount);
	return *this;
}

String & String::Trim(char * matches, int matchesSize)
{
	LTrim(matches,matchesSize);
	RTrim(matches,matchesSize);
	return *this;
}

String & String::LTrim(char * matches, int matchesSize)
{

	Substring::LTrim(matches,matchesSize);
	return *this;
}

String & String::RTrim(char * matches, int matchesSize)
{

	Substring::RTrim(matches,matchesSize);
	return *this;
}

String & String::Upper()
{
	if (!Allocated) Construct();
	Characters::Uppercase(Data,Size);
	return *this;
}

String & String::Lower()
{
	if (!Allocated) Construct();
	Characters::Lowercase(Data,Size);
	return *this;
}

String & String::Propper()
{
	if (!Allocated) Construct();
	Characters::Proppercase(Data,Size);
	return *this;
}

String & String::Reverse()
{

	char * data = Data;
	int size = Size/2;
	int index = Size-1;
	char c = 0;
	while (data < Data+index)
	{
		c = *data;
		*data = Data[index];
		Data[index] = c;
		--index;
		++data;
	}

	return *this;
}

String & String::operator *= (int times)
{
	if (times > 0)
	{
		int pow = 0;
		int size = Size;
		Allocate(Size*times);
		while ((1 << (pow+1)) < times)
		{

			Append(*this);
			++pow;
		}

		times = times - (1<<pow);
		while (times-- > 0)
		{
			Append(Data,size);
		}
	}

	return *this;
}

String & String::FormatVa(const char * fmt, va_list va) 
{

	int amount = Length(fmt)*1.25;

	Reserve(amount);

	int count=0;
	while (true) 
	{

		amount = Allocated;
		count = Characters::FormatVa(Data,amount,fmt,va);

		if (count > -1 && count < amount)
			break;

		if (count > -1)		
		{

			Reserve(count+1+1);
		}
		else 
		{

			Reserve(0);
		}			
	}

	Size = count;

	Term();

	return *this;
}

String & String::FormatVa(int index, const char * fmt, va_list va) 
{

	int amount = String::Length(fmt)*1.25;
	Reserve(amount);

	int count=0;
	while (true) 
	{		
		amount = Allocated-Size;		
		count = Characters::FormatVa(Data+Size,amount,fmt,va);

		if (count > -1 && count < amount)
			break;		

		if (count > -1)		
		{
			Reserve(count+1+1);
		}
		else 
		{
			Reserve(0);
		}			
	}

	if (count > -1 && count < amount)
	{			
		int size = Size;
		Size += count;

		if (size > 0 && index < size)
		{

			Reserve(count);

			amount = size+count-index;
			memmove(Data+index+count,Data+index,amount);
			memmove(Data+index,Data+index+amount,count);		
		}

    }

	Term();
	return *this;	

}

String String::Formatted(const char * format, ...)
{
	va_list va;
	va_start(va, format);
	Superstring str = Hero::Formatter::FormatVa(format,va);
	va_end(va);
	return str;	
}

String String::Formatted(const char * format, va_list va)
{
	return Hero::Formatter::FormatVa(format,va);
}

String & String::Format(const char * format, ...) 
{

	va_list va;
	va_start(va, format);	
	FormatVa(format,va);
	va_end(va);

	return *this;
}

String & String::Format(int index, const char * format, ...) 
{

	va_list va;
	va_start(va, format);	
	FormatVa(index,format,va);
	va_end(va);

	return *this;
}

#ifdef HERO_USING_STD
String & String::operator << (ManipFunc func) 
{

	return *this;
}

int String::Read(std::istream & stream, int amount) 
{
    amount = (amount > (Allocated-1))?(Allocated-1):amount;

	if (amount == 0)
		amount = (Allocated-1);

	stream.read(Data,amount);

	Term();
    return amount;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Identity Superstring::Instance;

Superstring Superstring::Partial(const Superstring & superstring)
{

	if (superstring.Allocated) 
		return Superstring();

	return superstring;
}

Superstring Superstring::Partial(const String & string)
{

	return Superstring(Substring(string));
}

Superstring Superstring::Partial(char * data, int size)
{

	return Superstring(Substring(data,size));
}

Superstring Superstring::Literal(const Superstring & superstring)
{
	if (superstring.Allocated && superstring.Data[superstring.Size]==0)
		return superstring;
	else
		return String(superstring);
}

Superstring Superstring::Literal(const String & string)
{

	Superstring superstring;
	if (string.Allocated && string.Data[string.Size]==0)
		superstring.Assign(string);
	else
		superstring.Append(string);

	return superstring;
}

Superstring Superstring::Literal(const Substring & sequence)
{

	return Literal(sequence.Data,sequence.Size);
}

Superstring Superstring::Literal(char * data, int size)
{

	return Superstring(data,size);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern char const PrinterQueueTag[] = "PrinterQueue";
class PrinterQueue : public LeakDetector<PrinterQueue,PrinterQueueTag>
{	
public:

	ReadWriteLock Lock;

	static const int Size = 4096;
	char Data[4096];

	char * QueuePointer;
	int    QueueItemSize;

	PrinterQueue()
	{
		QueuePointer = Data;
		QueueItemSize = 0;	
	}

	~PrinterQueue()
	{

	}

	static PrinterQueue & Queue()
	{

		static PrinterQueue queue;
		return queue;

	}

	int Remaining()
	{
		return Size-(QueuePointer-Data);
	}

	void Initialise()
	{
		#ifdef HERO_PLATFORM_WINDOWS

		#endif
	}	

	char * Load(const char *data, const int size)
	{
		Initialise();

		if (data == 0)
			return (char *) data;

		if (QueuePointer==0)
			QueuePointer = Data;

		if (size > Size-2 ) 
		{
			Raise("PrinterQueue:Load - String is not large enough for output string.\n");
			return 0;
		}
		else
		{

			Lock.LockWrite();

			QueuePointer += QueueItemSize;

			if (size+1 > Remaining())
			{
				QueuePointer = Data;
			}

			memcpy(QueuePointer,data,size);			
			*(QueuePointer+size)=0;	

			QueueItemSize = size+1;

			Lock.UnlockWrite();

			return QueuePointer;
		}
	}	

};

Printer::operator char * (void) 
{
	return PrinterQueue::Queue().QueuePointer;
}

int Printer::Remaining()
{
	return PrinterQueue::Queue().Remaining();
}

char * Printer::Load(Substring &string)
{
	return Load(string.Data,string.Size);
}

char * Printer::Load(const char *data, const int size)
{
	return PrinterQueue::Queue().Load(data,size);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void HandleStdout(int level, const char *line)
{
	fprintf(HERO_HANDLE_STDOUT,line);
}

void HandleStderr(int level, const char *line)
{
	fprintf(HERO_HANDLE_STDERR,line);
}

void SetHandlerPrint(int level, void (*Handler)(int level, const char *line))
{
	HandlerPrint = Handler;
}

#undef Print

void Print(const char * format,...)
{
	va_list args;
	va_start(args,format);
	String line;
	Formatter::FormatVa(line,format,args);	
	va_end(args);	
	HandlerPrint(0,line.Data);
}

void PrintLn(const char * format,...)
{
	va_list args;
	va_start(args,format);
	String line;
	Formatter::FormatVa(line,format,args);		
	va_end(args);	
	line.Append("\n");
	HandlerPrint(0,line.Data);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef HERO_PLATFORM_POSIX
#pragma clang diagnostic pop

#endif