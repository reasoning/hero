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

#include "hero/unsigned.h"
#include "hero/integer.h"
#include "hero/error.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






Unsigned::Unsigned(const Integer& integer)
{
	
	
	Unsigned res(0), base(integer.Base), tmp;
	int digit = Size;
	while (digit > 0) 
	{
		digit--;
		tmp.Multiply(res, base);
		res.Add(tmp, Unsigned(integer.Data[digit]));
	}

	Move(res);
}

Unsigned::Unsigned(const Substring& str)
{
	
	Unsigned res(Integer(str, 10));
	Move(res);
}




Unsigned::Unsigned(unsigned long long x)	{InitFromUnsignedPrimitive(x);}
Unsigned::Unsigned(unsigned long  x)		{InitFromUnsignedPrimitive(x);}
Unsigned::Unsigned(unsigned int   x)		{InitFromUnsignedPrimitive(x);}
Unsigned::Unsigned(unsigned short x)		{InitFromUnsignedPrimitive(x);}

Unsigned::Unsigned(signed long long  x)	{InitFromSignedPrimitive(x);}
Unsigned::Unsigned(signed long  x)		{InitFromSignedPrimitive(x);}
Unsigned::Unsigned(signed int   x)		{InitFromSignedPrimitive(x);}
Unsigned::Unsigned(signed short x)		{InitFromSignedPrimitive(x);}

Unsigned::Unsigned(float x)				{InitFromSignedPrimitive(x);}
Unsigned::Unsigned(double x)			{InitFromSignedPrimitive(x);}


unsigned long long  Unsigned::UnsignedLongLong()	{return ConvertToUnsignedPrimitive<unsigned long long>();}
unsigned long		Unsigned::UnsignedLong()		{return ConvertToUnsignedPrimitive<unsigned long>();}
unsigned int		Unsigned::UnsignedInt()			{return ConvertToUnsignedPrimitive<unsigned int>();}
unsigned short		Unsigned::UnsignedShort()		{return ConvertToUnsignedPrimitive<unsigned short>();}

signed long long	Unsigned::SignedLongLong()		{return ConvertToSignedPrimitive<signed long long>();}
long				Unsigned::SignedLong()			{return ConvertToSignedPrimitive<signed long>();}
int					Unsigned::SignedInt()			{return ConvertToSignedPrimitive<signed int>();}
short				Unsigned::SignedShort()			{return ConvertToSignedPrimitive<signed short>();}

String Unsigned::Str()
{
	return Integer::UnsignedToString(*this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




Unsigned Unsigned::operator +(const Unsigned &x) 
{
	Unsigned ans;
	ans.Add(*this, x);
	return ans;
}

Unsigned Unsigned::operator -(const Unsigned &x) 
{
	Unsigned ans;
	ans.Subtract(*this, x);
	return ans;
}

Unsigned Unsigned::operator *(const Unsigned &x) 
{
	Unsigned ans;
	ans.Multiply(*this, x);
	return ans;
}

Unsigned Unsigned::operator /(const Unsigned &x)
{
	if (((Unsigned&)x).IsZero())
	{
		Raise("Unsigned::operator /: division by zero");
		return Unsigned();
	}

	Unsigned q, r;
	r = *this;
	r.DivideWithRemainder(x, q);

	return q;
}

Unsigned Unsigned::operator %(const Unsigned &x) 
{
	if (((Unsigned&)x).IsZero())
	{	
		Raise("Unsigned::operator %: division by zero");
		return Unsigned();
	}

	Unsigned q, r;
	r = *this;
	r.DivideWithRemainder(x, q);
	return r;
}


Unsigned Unsigned::operator &(const Unsigned &x)
{
	Unsigned ans;
	ans.BitAnd(*this, x);
	return ans;
}

Unsigned Unsigned::operator |(const Unsigned &x) 
{
	Unsigned ans;
	ans.BitOr(*this, x);
	return ans;
}

Unsigned Unsigned::operator ^(const Unsigned &x) 
{
	Unsigned ans;
	ans.BitXor(*this, x);
	return ans;
}

Unsigned Unsigned::operator <<(int b) 
{
	Unsigned ans;
	ans.BitShiftLeft(*this, b);
	return ans;
}

Unsigned Unsigned::operator >>(int b) 
{
	Unsigned ans;
	ans.BitShiftRight(*this, b);
	return ans;
}

Unsigned & Unsigned::operator +=(const Unsigned &x) 
{
	Add(*this, x);
	return *this;
}

Unsigned & Unsigned::operator -=(const Unsigned &x) 
{
	Subtract(*this, x);
	return *this;
}

Unsigned & Unsigned::operator *=(const Unsigned &x) 
{
	Multiply(*this, x);
	return *this;
}

Unsigned & Unsigned::operator /=(const Unsigned &x) 
{
	if (((Unsigned&)x).IsZero()) 
	{
		Raise("Unsigned::operator /=: division by zero");
		return *this;		
	}

	
	
	Unsigned q;
	DivideWithRemainder(x, q);
	
	Move(q);	

	return *this;
}

Unsigned & Unsigned::operator %=(const Unsigned &x) 
{
	if (((Unsigned&)x).IsZero()) 
	{
		Raise("Unsigned::operator %=: division by zero");
		return *this;
	}
	
	Unsigned q;
	
	DivideWithRemainder(x, q);

	return *this;
}

Unsigned & Unsigned::operator &=(const Unsigned &x)
{
	BitAnd(*this, x);
	return *this;
}

Unsigned & Unsigned::operator |=(const Unsigned &x) 
{
	BitOr(*this, x);
	return *this;
}

Unsigned & Unsigned::operator ^=(const Unsigned &x) 
{
	BitXor(*this, x);
	return *this;
}

Unsigned & Unsigned::operator <<=(int b) 
{
	BitShiftLeft(*this, b);
	return *this;
}

Unsigned & Unsigned::operator >>=(int b) 
{
	BitShiftRight(*this, b);
	return *this;
}





void Unsigned::Set(int index, Block block) {
	if (block == 0) 
	{
		if (index < Size) 
		{
			Data[index] = 0;
			Trim();
		}
		

	} 
	else 
	{
		
		if (index >= Size) 
		{
			
			Allocate(index+1);
			
			for (int j = Size; j < index; j++)
				Data[j] = 0;
			Size = index+1;
		}

		Data[index] = block;
	}
}




int Unsigned::BitLength() 
{
	if (IsZero())
	{
		return 0;
	}
	else 
	{
		Block leftmostBlock = Get(Size - 1);

		
		

		int leftmostBlockLen = 0;
		while (leftmostBlock != 0) 
		{
			leftmostBlock >>= 1;
			leftmostBlockLen++;
		}
		return leftmostBlockLen + (Size - 1) * N;
	}
}

void Unsigned::Bit(int bi, bool newBit) 
{
	int blockI = bi / N;
	Block block = Get(blockI), mask = Block(1) << (bi % N);
	block = newBit ? (block | mask) : (block & ~mask);
	Set(blockI, block);
}

int Unsigned::Compare(const Unsigned &x) 
{
	
	if (Size < x.Size)
	{
		return -1;
	}
	else 
	if (Size > x.Size)
	{
		return 1;
	}
	else 
	{
		
		int i = Size;
		while (i > 0) {
			i--;
			if (Data[i] == x.Data[i])
				continue;
			else if (Data[i] > x.Data[i])
				return 1;
			else
				return -1;
		}
		
		return 0;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





Unsigned & Unsigned::Add(const Unsigned &a, const Unsigned &b) 
{
	if (this == &a || this == &b)
	{
		Move(Unsigned().Add(a, b));
		return *this;
	}

	
	if (a.Size == 0) 
	{
		operator =(b);
		return *this;
	} 
	else 
	if (b.Size == 0) 
	{
		operator =(a);
		return *this;
	}

	
	
	bool carryIn, carryOut;
	Block temp;
	int i;
	
	const Unsigned *a2, *b2;
	if (a.Size >= b.Size) 
	{
		a2 = &a;
		b2 = &b;
	} 
	else 
	{
		a2 = &b;
		b2 = &a;
	}

	
	int amount = a2->Size + 1;
	Allocate(amount);
	Size = amount;

	
	for (i = 0, carryIn = false; i < b2->Size; i++) 
	{
		
		temp = a2->Data[i] + b2->Data[i];
		
		
		carryOut = (temp < a2->Data[i]);
		
		if (carryIn) 
		{
			temp++;
			carryOut |= (temp == 0);
		}
		Data[i] = temp; 
		carryIn = carryOut; 
	}
	
	
	
	for (; i < a2->Size && carryIn; i++) 
	{
		temp = a2->Data[i] + 1;
		carryIn = (temp == 0);
		Data[i] = temp;
	}
	
	
	
	for (; i < a2->Size; i++)
		Data[i] = a2->Data[i];

	
	if (carryIn)
		Data[i] = 1;
	else
		Size--;

	return *this;
}

Unsigned & Unsigned::Subtract(const Unsigned &a, const Unsigned &b) 
{
	if (this == &a || this == &b)
	{
		Move(Unsigned().Subtract(a, b));
		return *this;
	}

	if (b.Size == 0) 
	{
		
		operator =(a);
		return *this;
	} 
	else 
	if (a.Size < b.Size)
	{
		
		Raise("Unsigned::subtract: Negative result in unsigned calculation");
		return *this;
	}

	
	bool borrowIn, borrowOut;
	Block temp;
	int i;

	
	int amount = a.Size;
	Allocate(amount);
	Size = amount;

	

	for (i = 0, borrowIn = false; i < b.Size; i++) 
	{
		temp = a.Data[i] - b.Data[i];
		
		
		borrowOut = (temp > a.Data[i]);
		
		if (borrowIn) 
		{
			borrowOut |= (temp == 0);
			temp--;
		}
		Data[i] = temp; 
		borrowIn = borrowOut; 
	}
	
	
	for (; i < a.Size && borrowIn; i++) 
	{
		borrowIn = (a.Data[i] == 0);
		Data[i] = a.Data[i] - 1;
	}

	
	
	
	if (borrowIn) 
	{
		Size = 0;
		Raise("Unsigned::subtract: Negative result in unsigned calculation");
		return *this;
	}
	else
	{
		
		for (; i < a.Size; i++)
			Data[i] = a.Data[i];
	}

	Trim();

	return *this;
}





Block GetShiftedBlock(const Unsigned &num, int x, unsigned int y) 
{
	Block part1 = (x == 0 || y == 0) ? 0 : (num.Data[x - 1] >> (Unsigned::N - y));
	Block part2 = (x == num.Size) ? 0 : (num.Data[x] << y);
	return part1 | part2;
}

Unsigned & Unsigned::Multiply(const Unsigned &a, const Unsigned &b) 
{
	if (this == &a || this == &b)
	{
		Move(Unsigned().Multiply(a, b));
		return *this;
	}

	
	if (a.Size == 0 || b.Size == 0) {
		Size = 0;
		return *this;
	}

	

	
	
	

	
	int i, j, k;
	unsigned int i2;
	Block temp;
	bool carryIn, carryOut;
	
	
	int amount = a.Size + b.Size;
	Allocate(amount);
	Size = amount;

	
	for (i = 0; i < Size; i++)
		Data[i] = 0;

	
	for (i = 0; i < a.Size; i++) 
	{
		
		for (i2 = 0; i2 < N; i2++) 
		{
			if ((a.Data[i] & (Block(1) << i2)) == 0)
				continue;

			
			

			
			
			
			
			
			
			
			

			for (j = 0, k = i, carryIn = false; j <= b.Size; j++, k++) {
				
				

				temp = Data[k] + GetShiftedBlock(b, j, i2);
				carryOut = (temp < Data[k]);
				if (carryIn) {
					temp++;
					carryOut |= (temp == 0);
				}
				Data[k] = temp;
				carryIn = carryOut;
			}
			
			
			for (; carryIn; k++) {
				Data[k]++;
				carryIn = (Data[k] == 0);
			}
		}
	}
	
	if (Data[Size - 1] == 0)
		Size--;

	return *this;
}


Unsigned & Unsigned::Divide(const Unsigned &a, const Unsigned &b) 
{
	if (this == &a || this == &b)
	{
		Move(Unsigned().Divide(a, b));
		return *this;
	}

	
	if (a.Size == 0 || b.Size == 0) {
		Size = 0;
		return *this;
	}

	
	
	*this = a;

	Unsigned q;
	DivideWithRemainder(b, q);
	
	Move(q);

	return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Unsigned & Unsigned::Pow(const Unsigned &x)
{
	Unsigned & _x = (Unsigned&)x;

	
	if (x == 1) return *this;

	if (x == 0)
	{
		
		operator = (1);
	}
	else
	{
		
		Unsigned pow(*this);
		while(_x > 1)
		{
			pow *= *this;
			--_x;
		}

		Move(pow);
	}

	return *this;
}

Unsigned & Unsigned::Mod(const Unsigned &x)
{
	Unsigned q;
	DivideWithRemainder(x,q);
	return *this;
}



void Unsigned::DivideWithRemainder(const Unsigned &b, Unsigned &q) {
	
	
	
	
	

	if (this == &q)
		Raise("Unsigned::DivideWithRemainder: Cannot write quotient and remainder into the same variable");


	
	
	if (this == &b || &q == &b) {
		Unsigned tmpB(b);
		DivideWithRemainder(tmpB, q);
		return;
	}

	
	
	
	
	
	
	
	
	
	if (b.Size == 0) {
		q.Size = 0;
		return;
	}

	
	
	
	
	if (Size < b.Size) {
		q.Size = 0;
		return;
	}

	

	
	

	
	
	
	
	
	
	

	
	

	
	
	
	
	
	

	
	int i, j, k;
	unsigned int i2;
	Block temp;
	bool borrowIn, borrowOut;

	
	
	
	
	
	
	
	
	
	
	int length = Size; 
	
	Allocate(Size+1);
	Data[Size] = 0; 
	++Size;

	
	Block *subtractBuf = new Block[Size];

	
	int amount = length - b.Size + 1;
	q.Allocate(amount);
	q.Size = amount;

	
	for (i = 0; i < q.Size; i++)
		q.Data[i] = 0;

	
	i = q.Size;
	while (i > 0) {
		i--;
		
		
		q.Data[i] = 0;
		i2 = N;
		while (i2 > 0) 
		{
			i2--;
			
			
			
			
			
			
			
			
			for (j = 0, k = i, borrowIn = false; j <= b.Size; j++, k++) 
			{
				temp = Data[k] - GetShiftedBlock(b, j, i2);
				borrowOut = (temp > Data[k]);
				if (borrowIn) 
				{
					borrowOut |= (temp == 0);
					temp--;
				}
				
				subtractBuf[k] = temp; 
				borrowIn = borrowOut;
			}
			
			
			for (; k < length && borrowIn; k++) 
			{
				borrowIn = (Data[k] == 0);
				subtractBuf[k] = Data[k] - 1;
			}
			
			
			
			
			
			
			
			
			
			
			if (!borrowIn) {
				q.Data[i] |= (Block(1) << i2);
				while (k > i) {
					k--;
					Data[k] = subtractBuf[k];
				}
			} 
		}
	}
	
	if (q.Data[q.Size - 1] == 0)
		q.Size--;
	
	Trim();
	
	
	delete [] subtractBuf;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





Unsigned & Unsigned::BitAnd(const Unsigned &a, const Unsigned &b) 
{
	if (this == &a || this == &b)
	{
		Move(Unsigned().BitAnd(a, b));
		return *this;
	}
	
	
	int amount = (a.Size >= b.Size) ? b.Size : a.Size;
	Allocate(amount);
	Size = amount;

	int i;
	for (i = 0; i < Size; i++)
		Data[i] = a.Data[i] & b.Data[i];

	Trim();

	return *this;
}

Unsigned & Unsigned::BitOr(const Unsigned &a, const Unsigned &b) 
{
	if (this == &a || this == &b)
	{
		Move(Unsigned().BitOr(a, b));
		return *this;
	}


	int i;
	const Unsigned *a2, *b2;
	if (a.Size >= b.Size) {
		a2 = &a;
		b2 = &b;
	} else {
		a2 = &b;
		b2 = &a;
	}
	
	Allocate(a2->Size);
	
	for (i = 0; i < b2->Size; i++)
		Data[i] = a2->Data[i] | b2->Data[i];

	for (; i < a2->Size; i++)
		Data[i] = a2->Data[i];
	Size = a2->Size;
	

	return *this;
}

Unsigned & Unsigned::BitXor(const Unsigned &a, const Unsigned &b) 
{
	if (this == &a || this == &b)
	{
		Move(Unsigned().BitXor(a, b));
		return *this;
	}

	int i;
	const Unsigned *a2, *b2;
	
	if (a.Size >= b.Size) 
	{
		a2 = &a;
		b2 = &b;
	}
	else 
	{
		a2 = &b;
		b2 = &a;
	}
	
	Allocate(a2->Size);
	
	for (i = 0; i < b2->Size; i++)
		Data[i] = a2->Data[i] ^ b2->Data[i];
	
	for (; i < a2->Size; i++)
		Data[i] = a2->Data[i];

	Size = a2->Size;
	Trim();
	
	return *this;
}

Unsigned & Unsigned::BitShiftLeft(const Unsigned &a, int b) 
{
	if (this == &a)
	{
		Move(Unsigned().BitShiftLeft(a, b));
		return *this;
	}

	if (b < 0) 
	{
		if (b << 1 == 0)
		{
			Raise("Unsigned::BitShiftLeft: Pathological shift amount not implemented");
			return *this;
		}
		else 
		{
			BitShiftRight(a, -b);
			return *this;
		}
	}

	int shiftBlocks = b / N;
	unsigned int shiftBits = b % N;

	
	int amount = a.Size + shiftBlocks + 1;
	Allocate(amount);
	Size = amount;

	int i, j;
	for (i = 0; i < shiftBlocks; i++)
		Data[i] = 0;

	for (j = 0, i = shiftBlocks; j <= a.Size; j++, i++)
		Data[i] = GetShiftedBlock(a, j, shiftBits);


	
	if (Data[Size - 1] == 0)
		Size--;
	
	return *this;
}

Unsigned & Unsigned::BitShiftRight(const Unsigned &a, int b) 
{
	if (this == &a)
	{
		Move(Unsigned().BitShiftRight(a, b));
		return *this;
	}

	if (b < 0) 
	{
		if (b << 1 == 0)
		{
			Raise("Unsigned::BitShiftRight: Pathological shift amount not implemented");
			return *this;
		}
		else 
		{
			BitShiftLeft(a, -b);
			return *this;
		}
	}

	
	
	int rightShiftBlocks = (b + N - 1) / N;
	unsigned int leftShiftBits = N * rightShiftBlocks - b;


	
	
	if (rightShiftBlocks >= a.Size + 1) {
		
		
		Size = 0;
		return *this;
	}

	
	
	int amount = a.Size + 1 - rightShiftBlocks;
	Allocate(amount);
	Size = amount;

	int i, j;
	for (j = rightShiftBlocks, i = 0; j <= a.Size; j++, i++)
		Data[i] = GetShiftedBlock(a, j, leftShiftBits);
	
	if (Data[Size - 1] == 0)
		Size--;

	return *this;
}


Unsigned & Unsigned::operator ++() 
{
	
	int i;
	bool carry = true;
	for (i = 0; i < Size && carry; i++) 
	{
		Data[i]++;
		carry = (Data[i] == 0);
	}

	if (carry) 
	{
		
		Allocate(Size + 1);
		Size++;
		Data[i] = 1;
	}

	return *this;
}

Unsigned Unsigned::operator ++(int) 
{
	
	Unsigned prev = *this;
	operator ++();
	return prev;
}



Unsigned & Unsigned::operator --() 
{
	
	if (Size == 0)
	{
		Raise(new UnderflowError("Unsigned::operator --(): Cannot decrement an unsigned zero"));
		return *this;
	}

	int i;
	bool borrow = true;
	for (i = 0; borrow; i++) 
	{
		borrow = (Data[i] == 0);
		Data[i]--;
	}

	
	if (Data[Size - 1] == 0)
		Size--;

	return *this;
}


Unsigned Unsigned::operator --(int) 
{
	
	Unsigned prev = *this;
	operator --();
	return prev;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




