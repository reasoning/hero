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

#include "hero/pack.h"

#include "hero/indices.h"
#include "hero/binary.h"
#include "hero/string.h"
#include "hero/parser.h"
#include "hero/number.h"
#include "hero/encoding.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





#ifdef HERO_PLATFORM_POSIX

#include <arpa/inet.h>
#endif



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Packing::Construct(char * data, int size, int mode)
{
	String::Construct(data,size);
	Construct(mode);
}

void Packing::Construct(int mode)
{
	Specifiers.Release();
	Mode = mode;
	
	
	
	
	

	StringParser parser(*this);

	
	int index=0;

	
	
	int modifier=0;

	
	
	
	const char * modifiers = "@=<>!";
	if (!parser.Eof() && parser.IsAny(modifiers))
	{
		switch (*parser.At())
		{
			case '@': modifier |= Specifier::MODIFIER_AT; break;
			case '=': modifier |= Specifier::MODIFIER_EQ; break;
			case '<': modifier |= Specifier::MODIFIER_LT; break;
			case '>': modifier |= Specifier::MODIFIER_GT; break;
			case '!': modifier |= Specifier::MODIFIER_EX; break;
		}

		parser.Next();
	}

	const char * specifiers = "aAZbBhH@xXPcCsSiIlLnNvVqQjJfdFDp";

	while (!parser.Eof())
	{
		if (parser.Skip("%") && parser.IsAny(specifiers))
			modifier |= Specifier::INPUT_TEXT;

		char at = *parser.At();
		if (parser.IsAny("aAZbBhH@xXP"))
		{
			Specifier specifier;		
					
			parser.Mark(specifier.Format);
					
			switch (*parser.At())
			{
				case 'a': specifier.On(Specifier::TYPE_FORMAT_a); break;
				case 'A': specifier.On(Specifier::TYPE_FORMAT_A); break;
				case 'Z': specifier.On(Specifier::TYPE_FORMAT_Z); break;
				case 'b': specifier.On(Specifier::TYPE_FORMAT_b); break;
				case 'B': specifier.On(Specifier::TYPE_FORMAT_B); break;
				case 'h': specifier.On(Specifier::TYPE_FORMAT_h); break;
				case 'H': specifier.On(Specifier::TYPE_FORMAT_H); break;
				
				case 'x': specifier.On(Specifier::TYPE_FORMAT_x); break;
				
				case 'P': specifier.On(Specifier::TYPE_FORMAT_P); break;								
			}
								
			parser.Next();	
			if (parser.Skip("%"))
				modifier |= Specifier::OUTPUT_TEXT;				
											
			int count = 1;
			if (parser.ParseNumber())
				count = ((count = parser.Token.Integer())>0)?count:1;
			if (parser.Parse("*"))
				count = -1;
				
			
			
			
							
			parser.Trap(specifier.Format);
				
			
			specifier.Modifier = modifier;

			specifier.Count = count;
			Specifiers.Append(specifier);		
		}
		else
		if (parser.IsAny("cCsSiIlLnNvVqQjJ") || parser.IsAny("fdFDp"))
		{		
			Specifier specifier;
		
			
			
			parser.Mark(specifier.Format);	

			switch (*parser.At())
			{
				case 'c': specifier.On(Specifier::TYPE_FORMAT_c); break;
				case 'C': specifier.On(Specifier::TYPE_FORMAT_C); break;
				case 'W': specifier.On(Specifier::TYPE_FORMAT_W); break;
				case 's': specifier.On(Specifier::TYPE_FORMAT_s); break;
				case 'S': specifier.On(Specifier::TYPE_FORMAT_S); break;
				case 'i': specifier.On(Specifier::TYPE_FORMAT_i); break;
				case 'I': specifier.On(Specifier::TYPE_FORMAT_I); break;
				case 'l': specifier.On(Specifier::TYPE_FORMAT_l); break;
				case 'L': specifier.On(Specifier::TYPE_FORMAT_L); break;
				case 'n': specifier.On(Specifier::TYPE_FORMAT_n); break;
				case 'N': specifier.On(Specifier::TYPE_FORMAT_N); break;
				case 'v': specifier.On(Specifier::TYPE_FORMAT_v); break;
				case 'V': specifier.On(Specifier::TYPE_FORMAT_V); break;
				case 'q': specifier.On(Specifier::TYPE_FORMAT_q); break;
				case 'Q': specifier.On(Specifier::TYPE_FORMAT_Q); break;
				
				
				
				
				case 'j': specifier.On(Specifier::TYPE_FORMAT_j); break;
				case 'J': specifier.On(Specifier::TYPE_FORMAT_J); break;
				
				case 'f': specifier.On(Specifier::TYPE_FORMAT_f); break;
				case 'd': specifier.On(Specifier::TYPE_FORMAT_d); break;
				case 'F': specifier.On(Specifier::TYPE_FORMAT_F); break;
				case 'D': specifier.On(Specifier::TYPE_FORMAT_D); break;
				
				
				
				
				
				
				
				
				case 'p': specifier.On(Specifier::TYPE_FORMAT_p); break;				
			}
					
			parser.Next();
			if (parser.Skip("%"))
				modifier |= Specifier::OUTPUT_TEXT;	
			
			int count=1;
			if (parser.ParseNumber())
				count = ((count = parser.Token.Integer())>0)?count:1;
			else
			if (parser.Parse("*"))
				count = -1;

			parser.Trap(specifier.Format);		

			
			specifier.Modifier = modifier;

			specifier.Count = count;
			Specifiers.Append(specifier);				
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
	


	
	Reset(MODE_NONE);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PackingStream::PackingStream(const Substring & sequence, int mode, int width):
	Packing(sequence,mode),Index(0),Width(width)
{
}

PackingStream::PackingStream(const char * data, int mode, int width):
	Packing(data,mode),Index(0), Width(width)
{
}

PackingStream::~PackingStream()
{

	if (Mode == Packing::MODE_WRITE)
	{
		
		
		
	}

	
	
	
	if (Stream)
		delete Stream;
}

void PackingStream::Format(String & string, int width)
{
	if (Stream)
		delete Stream;

	Stream = new StringStream::StreamAdapter(string);

	
	
	
	Width = width;
}

void PackingStream::Format(Substring & string, int width)
{
	if (Stream)
		delete Stream;

	
	
	
	
	Stream = new SubstringStream::StreamAdapter(string);
	
	
	Width = width;
}

int PackingStream::Read(String & string)
{
	

	StreamParser parser(Stream);
	parser.Scanner->Token = Token;

	int position = parser.Position();
	if (parser.Parse(string))
	{
		return parser.Position()-position;
	}
	
	return 0;
}

int PackingStream::Read(Value & value)
{
	Reset(Packing::MODE_READ);

	
	
	
	if (Index < 0 || Index > Specifiers.Length()-1) return 0;
	
	
	
	String unpack;

	int position = Stream->Position;
	BinaryStream packedStream(Stream);

	
	
	
	
	
	
	
	
	

	Specifier & specifier = Specifiers[Index];

	
	while (specifier.Is(Specifier::TYPE_FORMAT_x))
	{		
		Counts[Index] = 0;
		++Index;
		
		
		
		unsigned char null;
		packedStream.Read(null);
		
		if (Index < 0 || Index > Specifiers.Length()-1) return 0;
		specifier = Specifiers[Index];
	}






	
	
	
	
	
	
	
	int modifier = Specifiers[0].Modifier&Specifier::MODIFIER;
	if ((Mode&Packing::MODE_SPECIFIC) != 0)
		modifier = specifier.Modifier&Specifier::MODIFIER;
	
	
	if ((modifier&(Specifier::MODIFIER_EQ|Specifier::MODIFIER_AT)) == 0)
		modifier |= Specifier::MODIFIER_EQ;
	
	
	int mode = Binary::ENDIAN_NONE;
	if ((modifier&Specifier::MODIFIER_LT) != 0)
		mode = Binary::ENDIAN_LITTLE;
	else
	if ((modifier&Specifier::MODIFIER_GT) != 0)
		mode = Binary::ENDIAN_BIG;



	
	
	if (Counts[Index] == 0)
		Counts[Index] = specifier.Count;

	int count = Counts[Index];

	
	if (count <= 0)
		
		count = -1;
	
	if (specifier.Is(Specifier::TYPE_FORMAT_a|Specifier::TYPE_FORMAT_A|Specifier::TYPE_FORMAT_Z))
	{
		value.On(Value::TYPE_STRING);
		
		char data;

		
		
		while (count != 0 && (count == -1 || count-- > 0))
		{
			if (!packedStream.Read(data)) break;
			unpack.Append(data);			
		}
		
		if (specifier.Is(Specifier::TYPE_FORMAT_a|Specifier::TYPE_FORMAT_A))
		{
			
			unpack.RTrim((char*)"\0 ",2);
		}
		else
		if (specifier.Is(Specifier::TYPE_FORMAT_Z))
		{
			
			unpack.RTrim((char*)"\0",1);
		}

	}
	else
	if (specifier.Is(Specifier::TYPE_FORMAT_h|Specifier::TYPE_FORMAT_H))
	{
		
		
		

		char byte = 0;
		char hexed[2] = {0};
		int nibble = 0;

		

		
		
		
		
		

		unsigned long long hex = 0;
		char * data = (char*)&hex;

		
		
		
		int size = value.Number.Size*2;
		if (value.Is(Value::TYPE_STRING))
			size = -1;

		int read = 0;		
		int shift = 0;

		while (count != 0 && (count == -1 || count > 0))
		{	
			
			

			read = packedStream.Read(byte);			
			if (read > 0)
			{	
				
				hex <<= 8;

				
				
				
				
				

				Hex::Encode(byte,hexed);	

				
				
				
				
				
				
				
				while (count == -1 || count > 0)
				{	
					if (specifier.Is(Specifier::TYPE_FORMAT_H))
					{
						
						
						
						
						
						
						
						
						
						nibble = byte&0xF0;
						byte <<=4;
						hex |= nibble>>(shift*4);

						unpack.Append(hexed[shift]);					
					}
					else
					{
						
						
						
						

						
						
						
						
						nibble = (byte&0x0F)<<4;
						byte >>=4;
						hex |= nibble>>(shift*4);

						unpack.Append(hexed[1-shift]);
					}										


					if (size != -1) --size;		
					if (count != -1) --count;

					if(++shift == 2)
						break;	

				}
					
				
				
				

				shift = 0;

			}
			else
			if (count > 0)
			{
				
				

				hex <<= 4;
				unpack.Append(0);
				
				if (size != -1) --size;
				if (count != -1) --count;
			}
			else
			{
				break;
			}			
			
			if (size == 0)
				break;


		}	

		value.Number = hex;

	}
	else
	if (specifier.Is(Specifier::TYPE_FORMAT_b|Specifier::TYPE_FORMAT_B))
	{
		
		
		
		
		

		
		
		

		char byte = 0;
		char binary[8] = {0};
		int bit = 0;

		unsigned long long bits = 0;
		char * data = (char*)&bits;

		
		int size = value.Number.Size*8;
		if (value.Is(Value::TYPE_STRING))
			size = -1;

		int read = 0;		
		int shift = 0;

		while (count != 0 && (count == -1 || count > 0))
		{					

			read = packedStream.Read(byte);			
			if (read > 0)
			{	
			
				
				

				while (count == -1 || count > 0)
				{
					
					char bit = (byte>>(specifier.Is(Specifier::TYPE_FORMAT_B)?7-shift:shift))&0x01;
					bits = (bits<<1)|bit;
					unpack.Append((bit)?'1':'0');		

					if (size != -1) --size;
					if (count != -1) --count;

					if(++shift == 8)
						break;		
				}
					
				
				
				

				shift = 0;

			}
			else
			if (count > 0)
			{
				

				
				

				bits <<=1;
				unpack.Append(0);

				
				

				if (size != -1) --size;
				if (count != -1) --count;
			}
			else
			{
				break;
			}
			

			if (size == 0)
				break;

			
			
		}

		value.Number = bits;
	}
	else
	if (specifier.Is(
		Specifier::TYPE_FORMAT_c|Specifier::TYPE_FORMAT_C|
		Specifier::TYPE_FORMAT_s|Specifier::TYPE_FORMAT_S|
		Specifier::TYPE_FORMAT_i|Specifier::TYPE_FORMAT_I|
		
		Specifier::TYPE_FORMAT_l|Specifier::TYPE_FORMAT_L|
		Specifier::TYPE_FORMAT_n|Specifier::TYPE_FORMAT_N|
		Specifier::TYPE_FORMAT_v|Specifier::TYPE_FORMAT_V|
		Specifier::TYPE_FORMAT_q|Specifier::TYPE_FORMAT_Q	
		) || specifier.Is(
		
		
		Specifier::TYPE_FORMAT_f|Specifier::TYPE_FORMAT_d|
		Specifier::TYPE_FORMAT_p|Specifier::TYPE_FORMAT_P))
	
	{
		

		value.On(Value::TYPE_NUMBER);
	
		
		
		
		if (count != 0 && (count == -1 || count-- > 0))
		{
			switch (specifier.Bits)
			{
			
			case Specifier::TYPE_FORMAT_c:
			case Specifier::TYPE_FORMAT_C:
				{
					unsigned char bits;
					if (!packedStream.Read(bits)) break;
					unpack.Append((char)bits);
					value.Number = bits;
				}
				break;
			case Specifier::TYPE_FORMAT_s:
			case Specifier::TYPE_FORMAT_S:
				{
					unsigned short bits;
					if (!packedStream.Read(bits)) break;
					unpack.Append((short)bits);
					value.Number = bits;
				}
				break;	
			case Specifier::TYPE_FORMAT_i:
			case Specifier::TYPE_FORMAT_I:
			
			
			
				{
					unsigned int bits;
					if (!packedStream.Read(bits)) break;
					unpack.Append((int)bits);
					value.Number = bits;
				}
				break;	
			case Specifier::TYPE_FORMAT_l:
			case Specifier::TYPE_FORMAT_L:			
			case 'l':case 'L':
			
				{
					
					

					
					
					

					
					
					
					
					
					if ((modifier&Specifier::MODIFIER_EQ) == 0)
					{
						
						
						
						
						
						
						

						unsigned long bits;
						if (!packedStream.Read(bits)) break;
						unpack.Append((long)bits);
						value.Number = bits;
					}
					else
					{
						
						
						
						
						
						
						
						
						

						unsigned int bits;
						if (!packedStream.Read(bits)) break;
						unpack.Append((int)bits);
						value.Number = bits;					
					}
				}
				break;	
			case Specifier::TYPE_FORMAT_q:
			case Specifier::TYPE_FORMAT_Q:	
			case 'q':case 'Q':
				{
					unsigned long long bits;
					if (!packedStream.Read(bits)) break;
					unpack.Append((long long)bits);
					value.Number = bits;
				}
				break;	

			
			
			

			
			case Specifier::TYPE_FORMAT_n:
				{
					unsigned short bits;
					if (!packedStream.Read(bits)) break;
					unpack.Append((short)ntohs(bits));
					value.Number = bits;
				}
				break;	
			case Specifier::TYPE_FORMAT_N:
				{
					unsigned long bits;
					if (!packedStream.Read(bits)) break;
					unpack.Append((long)ntohl(bits));
					value.Number = bits;
				}
				break;	
			
			case Specifier::TYPE_FORMAT_v:
				{
					unsigned short bits;
					if (!packedStream.Read(bits)) break;
					unpack.Append((short)bits);
					value.Number = bits;
				}
				break;
			case Specifier::TYPE_FORMAT_V:				
				{
					unsigned long bits;
					if (!packedStream.Read(bits)) break;
					unpack.Append((long)bits);
					value.Number = bits;
				}
				break;
			
			case Specifier::TYPE_FORMAT_f:
			
				{
					float bits;
					if (!packedStream.Read(bits)) break;
					unpack.Append(bits);
					value.Number = bits;
				}
				break;
			
			case Specifier::TYPE_FORMAT_d:
			
				{
					double bits;
					if (!packedStream.Read(bits)) break;
					unpack.Append(bits);
					value.Number = bits;
				}
				break;
			
			case Specifier::TYPE_FORMAT_p:
			case Specifier::TYPE_FORMAT_P:
				{
					int bits;
					if (!packedStream.Read(bits)) break;
					unpack.Append(bits);
					value.Number = bits;
				}
				break;
			}
		}
	}
	
		
	
	
	value.String = unpack;


	if ((Mode&Packing::MODE_ARRAY))
	{
		int bytes = Stream->Position-position;

		while (bytes < Width)
		{		
			
			
			unsigned char null=0;
			packedStream.Read(null);
			++bytes;
		}
	}


	
	Counts[Index] = count;
	if (count == 0)
		++Index;

	
	

	return Stream->Position-position;
}


int PackingStream::ReadInteger(char & value) 
{
	
	
	
	
	
	int read = 0;

	Value val;
	
	
	val.On(Value::TYPE_NUMBER);
	
	
	
	
	
	
	
	val.Number = (signed char)value;
	
	
	
	
	
	
	
	
	

	read += Read(val);

	if (val.Is(Value::TYPE_STRING))
		value = val.String.Integer();
	else
	if (val.Is(Value::TYPE_POINTER))
		#ifdef HERO_PLATFORM_64BIT
		value = (char)(long long)val.Pointer;
		#else
		value = (char)(int)val.Pointer;
		#endif
	else
		value = (char)val.Number;
	
	return read;
}

int PackingStream::ReadInteger(short & value) 
{
	
	
	
	

	int read = 0;

	Value val;
	
	
	val.On(Value::TYPE_NUMBER);
	val.Number = value;

	read += Read(val);

	if (val.Is(Value::TYPE_STRING))
		value = val.String.Integer();
	else
	if (val.Is(Value::TYPE_POINTER))
		#ifdef HERO_PLATFORM_64BIT
		value = (short)(long long)val.Pointer;
		#else
		value = (short)(int)val.Pointer;
		#endif
	else
		value = (short)val.Number;
	
	return read;
}
	


int PackingStream::ReadInteger(int & value)
{
	int read = 0;

	
	
	Value val;
	val.On(Value::TYPE_NUMBER);	
	val.Number = value; 

	read += Read(val);

	if (val.Is(Value::TYPE_STRING))
	{
		value = val.String.Integer();
	}
	else
	{
		if (val.Is(Value::TYPE_POINTER))
			#ifdef HERO_PLATFORM_64BIT
			value = (int)(long long)val.Pointer;
			#else
			value = (int)(int)val.Pointer;
			#endif
		else
			value = (int)val.Number;
	}

	return read;
}


int PackingStream::ReadInteger(long & value)
{
	int read = 0;

	
	
	Value val;
	val.On(Value::TYPE_NUMBER);
	val.Number = value;	

	read += Read(val);

	if (val.Is(Value::TYPE_STRING))
	{
		value = val.String.Integer();
	}
	else
	{
		if (val.Is(Value::TYPE_POINTER))
			#ifdef HERO_PLATFORM_64BIT
			value = (long)(long long)val.Pointer;
			#else
			value = (long)(int)val.Pointer;
			#endif
		else
			value = (long)val.Number;
	}

	return read;
}

int PackingStream::ReadInteger(long long & value)
{
	int read = 0;

	
	
	Value val;
	val.On(Value::TYPE_NUMBER);
	val.Number = value; 

	read += Read(val);

	if (val.Is(Value::TYPE_STRING))
	{
		value = val.String.Integer();
	}
	else
	{
		if (val.Is(Value::TYPE_POINTER))
			value = (long long)val.Pointer;
		else
			value = (long long)val.Number;
	}

	return read;
}

int PackingStream::ReadDouble(double & value)
{
	int read = 0;

	Value val;
	val.On(Value::TYPE_NUMBER);
	val.Number = value; 

	read += Read(val);

	if (val.Is(Value::TYPE_STRING))
	{
		value = (double)val.String.Real();
	}
	else
	{
		if (val.Is(Value::TYPE_POINTER))
			#ifdef HERO_PLATFORM_64BIT
			value = (double)(long long)val.Pointer;
			#else
			value = (double)(int)val.Pointer;
			#endif
		else
			value = (double)val.Number;
	}

	return read;
}

int PackingStream::ReadString(char * data, int size)
{
	String value;
	int read = ReadString(value);

	
	
	
	
	memcpy(data,value.Data,Number::Min(value.Size,size));
	if (value.Size < size)
		memset(data+value.Size,0,size-value.Size);

	return read;
}


int PackingStream::ReadString(String & value)
{
	int read = 0;

	Value val;
	val.On(Value::TYPE_STRING);

	read += Read(val);

	if (val.Is(Value::TYPE_STRING))
	{
		value = val.String;
	}
	else
	{
		if (val.Is(Value::TYPE_POINTER))
			#ifdef HERO_PLATFORM_64BIT
			value = (long long)val.Pointer;
			#else
			value = (int)val.Pointer;
			#endif
		else
			value = (double)val.Number;
	}

	return read;
}



int PackingStream::Write(String & string)
{
	return Stream->Write(string);
}

int PackingStream::Write(Value & value)
{
	Reset(Packing::MODE_WRITE);

	
	
	

	

	String pack;
	

	int position = Stream->Position;

	
	
	

	if (Index < 0 || Index > Specifiers.Length()-1)
	{
		
		if(value.Is(Value::TYPE_NUMBER))
		{
			value.Number.Print(pack);
			Stream->Write(pack);
		}
		else
		if (value.Is(Value::TYPE_POINTER))
		{
			pack.Format("%08LX",value.Pointer);
			Stream->Write(pack);
		}
		else
		if(value.Is(Value::TYPE_STRING))
		{
			Stream->Write(value.String);
		}

		return Stream->Position-position;
	}


	BinaryStream packedStream(Stream);

	
	
	
	Specifier & specifier = Specifiers[Index];

	
	
	
	
	
	
	
	
	

	
	
	
	
	
	
	int modifier = Specifiers[0].Modifier&Specifier::MODIFIER;
	if ((Mode&Packing::MODE_SPECIFIC) != 0)
		modifier = specifier.Modifier&Specifier::MODIFIER;
	
	
	
	
	if ((modifier&(Specifier::MODIFIER_EQ|Specifier::MODIFIER_AT)) == 0)
		
		
		
		
		modifier |= Specifier::MODIFIER_EQ;

	
	
	
	
	int mode = Binary::ENDIAN_NONE;
	if ((modifier&Specifier::MODIFIER_LT) != 0)
		mode = Binary::ENDIAN_LITTLE;
	else
	if ((modifier&Specifier::MODIFIER_GT) != 0)
		mode = Binary::ENDIAN_BIG;


	
	
	if (Counts[Index] == 0)
		Counts[Index] = specifier.Count;

	int count = Counts[Index];
	

	
	if (count <= 0)
		
		count = -1;
		
	
	
	
	
	
	
		

	
	
	
	
	
	
	
	
		
	
	
			
	

	if (specifier.Is(Specifier::TYPE_FORMAT_a|Specifier::TYPE_FORMAT_A|Specifier::TYPE_FORMAT_Z))
	{
		char * data = value.String.Data;
		int size = value.String.Size;

		
		
		
		
		
					
		while (count != 0 && (count == -1 || count-- > 0))
		{
			if (size-- > 0)
			{
				if (!packedStream.Write(*data)) break;
				++data;
			}
			else
			if (count >= 0)
			{
				
				if (!packedStream.Write(specifier.Is(Specifier::TYPE_FORMAT_A)?' ':'\0')) break;
			}
			else
			{
				break;
			}
		}
	}
	else
	if (specifier.Is(Specifier::TYPE_FORMAT_b) || specifier.Is(Specifier::TYPE_FORMAT_B))
	{

		unsigned long long hex = 0;
		char * data = 0;
		int size = 0;


		
		
		
		struct Bits
		{
			int Index;
			int Size;
				
			Bits(Value & value, int count)
			{
				Index = 0;
				if (value.Is(Value::TYPE_NUMBER))
				{						
					Size = value.Number.Size*8;	
					
				}
				else
				if (value.Is(Value::TYPE_STRING))
				{
					Size = value.String.Size;
				}
			}
			
			int Read(Value & value, int count, unsigned long long & hex)
			{
				if (Size == 0)
					return (int)(hex = 0);
		
				int shift=0;

				if (value.Is(Value::TYPE_NUMBER))
				{
					hex = value.Number.UnsignedLongLong();
						
					
					int offset = ((Size-1)-Index);
					int mask = 0x1<<Index;
					hex = (hex&mask)>>Index;
				}
				else
				if (value.Is(Value::TYPE_STRING))
				{
					char bit=0;
					bit = value.String.Data[Index];

					
					
					
					bit = (bit==0 || bit==0x30)?0:1;
					hex = (unsigned long long)bit;
				}

				--Size;
				++Index;
				++shift;

				return shift;
			}
		};
						

		Bits bits(value,count);

		

		char byte = 0;
		int bit = 0;

		int read = 0;
		int shift = 0;
		while (count != 0 && (count == -1 || count-- > 0))
		{

			read = bits.Read(value,count,hex);
			if (read > 0)
			{
				
				
				

				
				
				bit = hex&0x01;					
				byte |= ((specifier.Is(Specifier::TYPE_FORMAT_b))?bit<<shift:bit<<(7-shift));
				++shift;
			}
			else
			if (count >= 0)
			{
				bit = 0;
				byte |= ((specifier.Is(Specifier::TYPE_FORMAT_b))?bit<<shift:bit<<(7-shift));
				++shift;					
			}
			else
			{
				break;
			}


			if (shift == 8)
			{
				if (!packedStream.Write(byte)) break;
				shift=0;
				byte=0;
			}

		}

		if (shift)
			packedStream.Write(byte);
	}
	else	
	if (specifier.Is(Specifier::TYPE_FORMAT_h) || specifier.Is(Specifier::TYPE_FORMAT_H))
	{
		
		
		
		

		
		
		

		
		
			
		
		

		unsigned long long hex = 0;
		char * data = 0;
		int size = 0;


		
		
		
		struct Nibbles
		{
			int Index;
			int Size;
				
			Nibbles(Value & value, int count)
			{
				Index = 0;
				if (value.Is(Value::TYPE_NUMBER))
				{						
					
					
					
					Size = value.Number.Size*2;	
					
				}
				else
				if (value.Is(Value::TYPE_STRING))
				{
					Size = value.String.Size;
				}
			}
			
			int Read(Value & value, int count, unsigned long long & hex)
			{
				if (Size == 0)
					return (int)(hex = 0);

		
				int shift=0;

				if (value.Is(Value::TYPE_NUMBER))
				{
					hex = value.Number.UnsignedLongLong();
						
					
					
					
					
					

					int offset = ((Size-2)-Index*2)*4;
					int mask = 0xFF<<offset;
					hex = (hex&mask)>>offset;
					++Index;
					shift += 2;

					
				}
				else
				if (value.Is(Value::TYPE_STRING))
				{
					char hexed[2]= {0};
					while (Index < Size && shift < 2)
					{
						hexed[shift] = value.String.Data[Index];
						++Index;
						++shift;
					}
						
					char byte = 0;
					Hex::Decode(hexed,byte);
					hex = (unsigned long long)byte;
				}

				return shift;
			}
		};
						

		Nibbles nibbles(value,count);

		char byte = 0;
		char hexed[2]= {0};
		int nibble = 0;

		int read = 0;
		int shift = 0;
		while (count != 0 && (count == -1 || count-- > 0))
		{
			if (shift == 0)
				read = nibbles.Read(value,count,hex);

			if (read > 0)
			{
				
				
				
				
				if (specifier.Is(Specifier::TYPE_FORMAT_H))
				{
					

					
					
					

					nibble = hex&0xF0;
					hex <<=4;
					byte |= nibble>>(shift*4);

				}
				else
				{
					
						
					
					
					

					nibble = (hex&0x0F)<<4;
					hex >>=4;
					byte |= nibble>>(shift*4);

				}

				++shift;
				--read;
			}
			else
			if (count >= 0)
			{
				nibble = 0;
				byte |= ((specifier.Is(Specifier::TYPE_FORMAT_H))?nibble<<shift*4:nibble<<(4-shift*4));
				++shift;	
			}
			else
			{
				break;
			}

			
			if (shift == 2)
			{
				if (!packedStream.Write(byte)) break;
				shift=0;
				byte=0;
			}

		}

		if (shift)
			packedStream.Write(byte);
		
	}
	else
	if (count != 0 && (count == -1 || count-- > 0))
	{
		
		

		switch (specifier.Bits)
		{			
				
		
		case Specifier::TYPE_FORMAT_c:
		case Specifier::TYPE_FORMAT_C:		
		{		
			
			packedStream.Write((unsigned char)value.Number.UnsignedChar()); break;
		}
		break;
		case Specifier::TYPE_FORMAT_s:
		case Specifier::TYPE_FORMAT_S:								
		{
			unsigned short data = value.Number.UnsignedShort();
			packedStream.Write((char*)&data,sizeof(unsigned short),mode);
		}
		break;				
		case Specifier::TYPE_FORMAT_i:
		case Specifier::TYPE_FORMAT_I:
		
		
		{
			unsigned int data = value.Number.UnsignedInt();
			packedStream.Write((char*)&data,sizeof(unsigned int),mode);
		}
		break;		
		case Specifier::TYPE_FORMAT_l:
		case Specifier::TYPE_FORMAT_L:			
		{
			
			

			
			
			
			
			
			
			
			
			
			
			
			
			
			
		
			if ((modifier&Specifier::MODIFIER_EQ) != 0)
			{
				
				
				
				unsigned int data = value.Number.UnsignedInt();
				packedStream.Write((char*)&data,sizeof(unsigned int),mode);
			}
			else	
			{
				
				unsigned long data = value.Number.UnsignedLong();
				packedStream.Write((char*)&data,sizeof(unsigned long),mode);
			}
						
		}
		break;
		case Specifier::TYPE_FORMAT_q:
		case Specifier::TYPE_FORMAT_Q:
		{
			unsigned long long data = value.Number.UnsignedLongLong();
			packedStream.Write((char*)&data, sizeof(unsigned long long),mode);
		}
		break;
		
		case Specifier::TYPE_FORMAT_n:	
		{		
			
			unsigned short data = value.Number.UnsignedShort();
			packedStream.Write((char*)&data,sizeof(unsigned short),Binary::ENDIAN_BIG);
		}
		break;
		case Specifier::TYPE_FORMAT_N:
		{
			
			
			
			
			

			
			
			unsigned int data = value.Number.UnsignedInt();
			packedStream.Write((char*)&data,sizeof(unsigned int),Binary::ENDIAN_BIG);
		}
		break;
		
		case Specifier::TYPE_FORMAT_v:
		{
			
			unsigned short data = value.Number.UnsignedShort();
			packedStream.Write((char*)&data,sizeof(unsigned short),Binary::ENDIAN_LITTLE);
		}
		break;
		case Specifier::TYPE_FORMAT_V:
		{
			
			unsigned int data = value.Number.UnsignedInt();
			packedStream.Write((char*)&data,sizeof(unsigned int),Binary::ENDIAN_LITTLE);
		}			
		break;
		
		case Specifier::TYPE_FORMAT_f:
		
		packedStream.Write((float)value.Number.Float()); break;

		
		case Specifier::TYPE_FORMAT_d:
		
		packedStream.Write((double)value.Number.Double()); break;
			
		

		
		
				
		
		
		
		
				
		case Specifier::TYPE_FORMAT_p:
		case Specifier::TYPE_FORMAT_P:
		#ifdef HERO_PLATFORM_I386
		packedStream.Write((int)(char*)value.Pointer); break;
		#else
		
		
		packedStream.Write((long long)(char*)value.Pointer); break;
		#endif				

		}
				
		
		
		
		
	}

		


	
	
	
	
	if ((Mode&Packing::MODE_BYTES))
	{
		int bytes = Stream->Position-position;

		while (bytes < Width)
		{		
			
			unsigned char null=0;
			packedStream.Write(null);
			++bytes;
		}
	}


	
	Counts[Index] = count;
	if (count == 0)
		++Index;
		

	
	
	
	
	
	

	return Stream->Position-position;
}


int PackingStream::WriteInteger(char & value) 
{
	Value val;
	val = Value::TYPE_NUMBER;
	val.Number = value;
	return Write(val);
}

int PackingStream::WriteInteger(short & value) 
{
	Value val;
	val = Value::TYPE_NUMBER;
	val.Number = value;
	return Write(val);
}
	

int PackingStream::WriteInteger(int & value)
{
	Value val;
	val = Value::TYPE_NUMBER;
	val.Number = value;
	return Write(val);
}

int PackingStream::WriteInteger(long & value)
{
	Value val;
	val = Value::TYPE_NUMBER;
	val.Number = value;
	return Write(val);
}

int PackingStream::WriteInteger(long long & value)
{
	Value val;
	val = Value::TYPE_NUMBER;
	val.Number = value;
	return Write(val);
}

int PackingStream::WriteString(char * data, int size)
{
	
	String string(data,size,0);
	return WriteString(string);
}

int PackingStream::WriteString(String & value)
{
	Value val;
	val = Value::TYPE_STRING;

	
	
	val.String.Construct(value.Data,value.Size,value.Allocated);
	return Write(val);
}

int PackingStream::WriteDouble(double & value)
{
	Value val;
	val = Value::TYPE_NUMBER;
	val.Number = value;
	return Write(val);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Pack & operator << (String & string, Pack & pack)
{
	pack.Packing.Format(string);
	
	return pack;
}

Pack & operator << (const String & string, const Pack & pack)
{
	((Pack&)pack).Packing.Format((String &)string);
	
	
	
	
	
	((Pack&)pack).Packing.Stream->Seek(0,1);

	
	return (Pack&)pack;
}



Pack & operator >> (String & string, Pack & pack)
{
	pack.Packing.Format(string);

	
	

	
	return pack;
}


Pack & operator >> (const String & string, const Pack & pack)
{
	((Pack&)pack).Packing.Format((String &)string);
	
	
	

	
	return (Pack&)pack;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Unpack & operator >> (String & string, Unpack & unpack)
{
	unpack.Packing.Format(string);
	
	return unpack;
}


Unpack & operator >> (const String & string, const Unpack & unpack)
{
	((Unpack&)unpack).Packing.Format((String &)string);
	
	return (Unpack&)unpack;
}

Unpack & operator >> (const Substring & string, const Unpack & unpack)
{
	((Unpack&)unpack).Packing.Format((Substring &)string);
	
	return (Unpack&)unpack;
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

