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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "hero/indices.h"
#include "hero/binary.h"
#include "hero/string.h"
#include "hero/parser.h"
#include "hero/number.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






#ifdef HERO_PLATFORM_POSIX

#include <arpa/inet.h>
#endif

#ifdef HERO_PLATFORM_WINDOWS

#include <winsock2.h>
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Bits::Append(int & to, int from, int bits, int toIndex, int fromIndex, int fromScheme, int size)
{
	return Insert(to,from,bits,toIndex,fromIndex,SCHEME_NONE,fromScheme,size);
}

int Bits::Prepend(int & to, int from, int bits, int toIndex, int fromIndex, int fromScheme, int size)
{
	return Insert(to,from,bits,toIndex-bits,fromIndex,SCHEME_NONE,fromScheme,size);
}

int Bits::Insert(int & to, int from, int bits, int toIndex, int fromIndex, int toScheme, int fromScheme, int size)
{
	
	
	

	

	
	
	
	
	
    

	
	
	


	
	size = size<<3;

	
	if (!Indices::Normalise(toIndex,size) || !Indices::Normalise(fromIndex,size))
		return 0;

	
	
	
	


	if (toIndex < 0 || toIndex >= size || fromIndex < 0 || fromIndex >= size)
		return 0;

	if ((fromScheme&SCHEME_BITS) == (toScheme&SCHEME_BITS))
	{
		
		
		
		
		

		
		
		
		
		
		
		fromIndex = (fromScheme&SCHEME_INDEX_RIGHT)?(size-1)-fromIndex:fromIndex;
		toIndex = (toScheme&SCHEME_INDEX_RIGHT)?(size-1)-toIndex:toIndex;

		
		int diff = fromIndex - toIndex;
		from = (diff>0)?from<<diff:from>>abs(diff);
		
		

		
		int mask = Mask(bits,(toScheme&SCHEME_BITS_LEFT)?toIndex-(bits-1):toIndex,size);
		from &= mask; 
		to &= ~mask;		
		to |= from;	
	}
	else
	{
		
		fromIndex = (fromScheme&SCHEME_INDEX_RIGHT)?(size-1)-fromIndex:fromIndex;
		toIndex = (toScheme&SCHEME_INDEX_RIGHT)?(size-1)-toIndex:toIndex;

		
		
		int fromMask = Mask(1,fromIndex,size);
		int toMask = Mask(1,toIndex,size);
		int bit = 1<<toIndex;

		int count = bits;
		while (count)
		{
			
			
			to &= ~toMask;
			if (from&fromMask)
				to |= toMask;
			
			
			
			
			fromMask = (fromScheme&SCHEME_BITS_LEFT)?fromMask<<1:fromMask>>1;
			toMask = (toScheme&SCHEME_BITS_LEFT)?toMask<<1:toMask>>1;
			
			--count;		
		}
	
	}

	return bits;
	
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const int Hashing::Djb2Hash::Ascii[] = { 
			 65,192,206,110,232, 83,143,223, 92,106,216,149,162, 58, 54, 26,
			235,218, 47,108, 89, 30,135, 39,194,116, 46,200, 88,156,150,209,
			161, 77, 12, 20, 56, 99,165, 38,182,  6,237,253,159,183, 33,155,
			107,180, 37,181, 69,186,157,  9,188,198,222,100,213, 48,212,242,
			169,119, 97, 43,112,  4, 11,  0,178, 81,132, 72, 45,  1, 66,140,
			147, 76,  7,233,115,111, 90, 53, 57,117,228,114,120, 15,234,196,
			131, 13,134,141,109,177,225,230,163, 40, 19,250,160,190,172,197,
			 41, 79, 98, 18, 32,123, 71,241,245, 84, 44,247,185,252,154,243,
			 95,170,179, 94,254,187,118, 17,145,130,164,184,208,  5,203, 62,
			125,238, 59,124, 36, 68, 28, 23, 49,215,219, 87, 14,148,121,136,
			 25,204, 73, 52,195,202,137, 74,231,214,171, 22,174,103,193,113,
			 61,244,104, 34, 63,142, 27,101,151, 55,129,236,189,246, 80,217,
			152,249, 91,201,146,199, 78,207, 75,122,227, 24, 29,158,128, 96,
			166,226,255,229, 50, 51, 21,105, 93,221, 85, 82, 31, 35,224,168,
			239, 86,251, 10,139,175,126,144,176,133,240,  8,210,138, 64, 60,
			  2,248,205,153, 42,167,191,127, 67,211,173,  3, 70, 16,102,220};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Binary & Binary::Allocate(int bits)
{
	if (bits == 0)
	{
		if (!Size && Count > 0)
			bits = Count;
		else
			bits = (Count>0)?Count+1:1;
	}
	else
	if (bits > 0)
	{
		if (bits < Count)
			return *this;
	}
	else
	if (bits < 0)
	{
		return *this;
	
	}
	
	int size = Bits::Count(bits,1);
		
	unsigned char * data = new unsigned char[size];
	
	if (Count > 0) 
		memcpy(data,Data,(Count+7)/8);

	if (Size)
		delete [] Data;

	Data = data;
	Size = size;
			
	return *this;
}

Binary & Binary::Resize(int bits)
{
	if (!Size)
	{
		Allocate(bits);
		return *this;
	
	}
	
	if (bits > 0)
	{
		if (Count == 0) 
		{
			Allocate(bits);
		}
		else
		if (Remaining()-bits < 0)
		{
			int size = Bits::Count(Count+bits);
			unsigned char * data = new unsigned char[size];
			memcpy(data,Data,(Count+7)/8);
			
			if (Size)
				delete [] Data;	
			
			Data = data;
			Size = size;	
		}
	
		Count += bits;
	}
	else
	if (bits < 0)
	{
		Count += bits;
	}
	
	return *this;
}

Binary & Binary::Reserve(int bits)
{
	if (bits > 0)
	{
		if (Count == 0) 
		{
			Allocate(bits);
		}
		else
		if (Remaining()-bits < 0)
		{
			int size = Bits::Count(Count+bits);
			unsigned char * data = new unsigned char[size];
			memcpy(data,Data,(Count+7)/8);
			
			if (Size)
				delete [] Data;	
			
			Data = data;
			Size = size;
		}
	}
	else
	if (bits < 0)
	{
		
		bits = Count;
		Allocate(bits);
	}
	
	return *this;
}



Binary & Binary::Left(int bits)
{
	Assert((Size > 0 && Data != 0) || (Size == 0));

	if (bits >= 0)
	{
		Reserve(bits);
		
		
		
		
		if (bits%8 == 0)
		{
			
			memmove(Data+((bits+7)/8),Data,Size);
		
		}
		else
		{
			int bit = Size;

			int toByte = Bits::Index(bit+bits,1);
			int fromByte = Bits::Index(bit,1);

			int toIndex = (bit+bits)%8;		
			int toCount = toIndex;			
			
			int fromIndex = bit%8;
			int fromCount = fromIndex;
			
			while (bit > 0)
			{
				int to = Bits::Index(toByte,1);
				int from = Bits::Index(fromByte,1);
								
				
				
				
				
				
				
				
				
				
				bits = (toCount<fromCount)?toCount:fromCount; 
				
				Bits::Insert(to,from,bits,toIndex-bits,fromIndex-bit,Bits::DIRECTION_LEFT,Bits::DIRECTION_LEFT,sizeof(char));
				
				
				
				fromIndex -= bits;
				if (fromIndex < 0)
					fromByte--;
					
				
				
				
				Data[toByte] = to;
				
				bit -= 8;
			}
		}
	}
	else
	if (bits < 0)
	{
		memmove(Data,Data-bits,Size+bits);
	}

	Size += bits; 
	Terminate();
	return *this;
}

Binary & Binary::Right(int bits)
{
	Assert((Size > 0 && Data != 0) || (Size == 0));

	if (bits >= 0)
	{
		Reserve(bits);
	}
	else
	{
		
		
	}


	Size += bits;
	Terminate();
	return *this;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Binary::PackVa(String & packed, const char * format, va_list va)
{
	

	
	

	
	

	
	
	


	
	

	
	
	
	
	

	

	#ifdef HERO_PLATFORM_64BIT
	typedef long long word;
	#else
	typedef int word;
	#endif

	String pack;

	StringStream::StreamAdapter packedAdapter(packed);
	BinaryStream packedStream(packedAdapter);

	StringParser parser(format);
	while (!parser.Eof())
	{
		char at = *parser.At();
		if (parser.IsAny("aAZbBhH@xXP"))
		{
			parser.Next();

			int count = 1;
			if (parser.ParseNumber())
				count = ((count = parser.Token.Integer())>0)?count:1;

			switch (at)
			{
			case 'a':case 'A':case 'Z':
				{					
					char * data = (char*)va_arg(va,word);
					int size = String::Length(data);

					while(count-- > 0)
					{
						if (size-- > 0)
						{
							packedStream.Write(*data);
							++data;
						}
						else
						{
							
							packedStream.Write((at=='A')?' ':'\0');
						}
					}
				}
				break;
			case 'h': case 'H':
				{
					char * data = (char*)va_arg(va,word);
					int size = String::Length(data);

					
					int byte = 0;
					int bits = 0;
					while (count-- > 0)
					{
						if (size-- > 0)
						{
							char hex = 0;
							if ((*data >= 'a' && *data <= 'z') || (*data >= 'A' && *data <= 'Z') || (*data == '0' || *data == '1'))
								hex = Characters::Hex(data,1);
							else
							if (*data == 0 || *data == 1)
								hex = *data;
							else
								hex = *data&0x0F;

							
							
							AssertBreak();

							
							
							
							
							
							Bits::Insert(byte,bits,hex,4,4,(at=='H')?Bits::DIRECTION_LEFT:Bits::DIRECTION_RIGHT);
							++data;
						}
						else
						{
							
							

							Bits::Insert(byte,bits,'\0',4,4,(at=='H')?Bits::DIRECTION_LEFT:Bits::DIRECTION_RIGHT);
						}

						if (bits%8 == 0)
						{
							packedStream.Write(byte);
							bits=0;
						}
					}

					if (bits)
						packedStream.Write(byte);
				}
				break;
			case 'b': case 'B':
				{

					char * data = (char*)va_arg(va,word);
					int size = String::Length(data);

					

					char byte = 0;
					int bit = 0;

					int shift = 0;
					while (count-- > 0)
					{
						if (size-- > 0)
						{
							
							
							
							AssertBreak();

							
							
							bit = (int)(*data&0xFE);


							
							
							

							
							

							byte |= ((at=='b')?bit<<shift:bit<<(7-shift));
							++shift;
							++data;
						}


						if (shift == 8)
						{
							packedStream.Write(byte);
							shift=0;
						}

					}

					if (shift)
						packedStream.Write(byte);
				}
				break;
			}
		}
		else
		if (parser.IsAny("cCsSiIlLnNvVqQjJ") || parser.IsAny("fdFDp"))
		{
			
			parser.Next();

			int count=1;
			if (parser.ParseNumber())
				count = ((count = parser.Token.Integer())>0)?count:1;

			while (count-- > 0)
			{
				switch (at)
				{
				
				case 'c':case 'C': packedStream.Write((unsigned char)va_arg(va,int)); break;
				case 's':case 'S': packedStream.Write((unsigned short)va_arg(va,int)); break;
				case 'i':case 'I':
				case 'j':case 'J': packedStream.Write((unsigned int)va_arg(va,int)); break;
				case 'l':case 'L': packedStream.Write((unsigned long)va_arg(va,unsigned long)); break;
				case 'q':case 'Q': packedStream.Write((unsigned long long)va_arg(va,unsigned long long)); break;

				
				case 'n': packedStream.Write(htons((unsigned short)va_arg(va,int))); break;
				case 'N': packedStream.Write(htonl((unsigned long)va_arg(va,unsigned long))); break;

				
				case 'v': packedStream.Write((unsigned short)va_arg(va,int)); break;
				case 'V': packedStream.Write((unsigned long)va_arg(va,unsigned long)); break;			

				
				case 'f':case 'F': packedStream.Write((float)va_arg(va,double)); break;

				
				case 'd':case 'D': packedStream.Write((double)va_arg(va,double)); break;
				
				

				
				
				
				
				
				
				
				
				#ifdef HERO_PLATFORM_I386
				case 'p': packedStream.Write((int)(char*)va_arg(va,int)); break;
				#else
				
				
				case 'p': packedStream.Write((long long)(char*)va_arg(va,long long)); break;
				#endif

				}
			}
		}
		else
		if (parser.Is("uUw("))
		{
			
			
			

			Assert(false);
			parser.Next();
		}
		else
		{
			parser.Next();
		}
	}

}


void Binary::UnpackVa(Substring & packed, const char * format, va_list va)
{
	
	
	
}

void Binary::Unpack(Substring & packed, const char * format, String & unpacked)
{
	String unpack;

	Substring packedSubstring(packed);
	SubstringStream::StreamAdapter packedAdapter(packedSubstring);
	BinaryStream packedStream(packedAdapter);

	StringStream::StreamAdapter unpackedStream(unpacked);

	StringParser parser(format);
	while (!parser.Eof())
	{
		unpack.Release();

		char at = *parser.At();
		if (parser.IsAny("aAZbBhH@xXP"))
		{
			parser.Next();

			int count = 1;
			if (parser.ParseNumber())
				count = ((count = parser.Token.Integer())>0)?count:1;

			switch (at)
			{
			case 'a':case 'A':case 'Z':
				{
					char data;

					
					
					while(count-- > 0)
					{
						packedStream.Read(data);
						unpack.Append(data);
					}
					
					if (at == 'A')
					{
						
						unpack.RTrim("\0 ");
					}
					else
					if (at == 'Z')
					{
						
						unpack.RTrim("\0");
						unpack.Append('\0');
					}
					else
					if (at == 'a')
					{
						
					}	
				}
				break;
			case 'h': case 'H':
				{
					
					
					

					char byte = 0;
					int bits = 0;					

					while (count > 0)
					{					
						packedStream.Read(byte);

						
						
						
						unsigned int hex = (unsigned char)((at=='H')?byte:((byte&0x0F)<<4)|((byte&0xF0)>>4));
						String hexed = Numbers::Hex(hex);
					
						unpack.Append(hexed.Data[0]);
						--count;

						
						if (count > 0)
						{
							unpack.Append(hexed.Data[1]);
							--count;
						}
					}					
				}
				break;
			case 'b': case 'B':
				{
					
					
					

					
					
					

					char byte = 0;
					int bits = 0;					

					while (count > 0)
					{					
						packedStream.Read(byte);
						bits = (count < 8)?count:8;

						
						
						
						int shift=0;
						while (bits > 0)
						{
							char bit = byte&((at=='B')?1<<(8-shift):1<<shift);
							unpack.Append((bit)?'1':'0');
							++shift;
							--bits;
						}

						count -= 8;
					}
				}
				break;
			}
		}
		else
		if (parser.IsAny("cCsSiIlLnNvVqQjJ") || parser.IsAny("fdFDp"))
		{
			
			parser.Next();

			int count=1;
			if (parser.ParseNumber())
				count = ((count = parser.Token.Integer())>0)?count:1;

			while (count-- > 0)
			{
				switch (at)
				{
				
				case 'c':case 'C': 
					{
						unsigned char bits;
						packedStream.Read(bits);
						unpack.Append((char)bits);
					}
					break;
				case 's':case 'S':
					{
						unsigned short bits;
						packedStream.Read(bits);
						unpack.Append((short)bits);
					}
					break;	
				case 'i':case 'I':
				case 'j':case 'J': 
					{
						unsigned int bits;
						packedStream.Read(bits);
						unpack.Append((int)bits);
					}
					break;	
				case 'l':case 'L':
					{
						unsigned long bits;
						packedStream.Read(bits);
						unpack.Append((long)bits);
					}
					break;	
				case 'q':case 'Q':
					{
						unsigned long long bits;
						packedStream.Read(bits);
						unpack.Append((long long)bits);
					}
					break;	

				
				
				

				
				case 'n': 
					{
						unsigned short bits;
						packedStream.Read(bits);
						unpack.Append((short)ntohs(bits));
					}
					break;	
				case 'N': 
					{
						unsigned long bits;
						packedStream.Read(bits);
						unpack.Append((long)ntohl(bits));
					}
					break;	
				
				case 'v': 
					{
						unsigned short bits;
						packedStream.Read(bits);
						unpack.Append((short)bits);
					}
					break;
				case 'V': 
					{
						unsigned long bits;
						packedStream.Read(bits);
						unpack.Append((long)bits);
					}
					break;
				
				case 'f':case 'F':
					{
						float bits;
						packedStream.Read(bits);
						unpack.Append(bits);
					}
					break;
				
				case 'd':case 'D': 
					{
						double bits;
						packedStream.Read(bits);
						unpack.Append(bits);
					}
					break;
				
				case 'p': 
					{
						int bits;
						packedStream.Read(bits);
						unpack.Append(bits);
					}
					break;
				}
			}
		}
		else
		if (parser.Is("uUw("))
		{
			
			
			

			Assert(false);
			parser.Next();
		}
		else
		{
			parser.Next();
		}

		unpackedStream.Write(unpack);
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

int BinaryStream::Write(char * data, int size, int mode)
{
	int endian = Binary::ENDIAN_NONE;
	
	#if defined(HERO_PLATFORM_X86_64) || defined(HERO_PLATFORM_I386)
	
	
	endian = Binary::ENDIAN_LITTLE;
	#else
	
	endian = Binary::Endian();
	#endif

	if (!mode)
		mode = endian;		

	if (endian == mode)
	{
		
		return Write(data,size);
	}
	else
	{
		
		

		
		
		

		if (size == 8)
		{
			#ifdef HERO_PLATFORM_WINDOWS
			unsigned long long value = _byteswap_uint64(*(unsigned long long*)data);			
			#else
			unsigned long long value = __builtin_bswap64(*(unsigned long long*)data);
			#endif
			
			return Write((char*)&value,size);
		}
		else
		if (size == 4)
		{
			#ifdef HERO_PLATFORM_WINDOWS
			unsigned int value = _byteswap_ulong(*(unsigned int*)data);			
			#else
			unsigned int value = __builtin_bswap32(*(unsigned int*)data);
			#endif
			
			return Write((char*)&value,size);
		}
		else
		if (size == 2)
		{
			#ifdef HERO_PLATFORM_WINDOWS
			unsigned short value = _byteswap_ushort(*(unsigned short*)data);			
			#else
			unsigned short value = __builtin_bswap16(*(unsigned short*)data);
			#endif

			return Write((char*)&value,size);
		}
		else
		{
			
			
			int write = 0;
			char * value = data+size;				
			while (value-- >= data)
				write += Write(value,1);
			return write;
		}

	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
