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



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "hero/string.h"
#include "hero/number.h"
#include "hero/stream.h"
#include "hero/parser.h"
#include "hero/binary.h"
#include "hero/bit.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


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



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





class Packed
{

};

class Packing : public String
{
public:

	
	struct Specifier : public Flags<64>
	{
		enum SpecifierOptions
		{
			
			
			
			
			
			
			

			
			TYPE_FORMAT_a	=(1)<<0,
			
			
			TYPE_FORMAT_A	=(1)<<1,
			
			
			TYPE_FORMAT_Z	=(1)<<3,
			
			
			
			TYPE_FORMAT_b	=(1)<<4,
			
			
			TYPE_FORMAT_B	=(1)<<5,
			
			
			TYPE_FORMAT_h	=(1)<<6,
			
			
			TYPE_FORMAT_H	=(1)<<7,
			
			
			TYPE_FORMAT_c	=(1)<<8,
						
			
			TYPE_FORMAT_C	=(1)<<9,
			
			
			TYPE_FORMAT_W	=(1)<<10,
			
			
			TYPE_FORMAT_s	=(1)<<11,
			
			
			TYPE_FORMAT_S	=(1)<<12,
			
			
			TYPE_FORMAT_l	=(1)<<13,
			
			
			TYPE_FORMAT_L	=(1)<<14,
			
			
			TYPE_FORMAT_q	=(1)<<15,
			
			
			
			
			TYPE_FORMAT_Q	=(1)<<16,
				
			
			TYPE_FORMAT_i	=(1)<<17,
			
			
			
			
			TYPE_FORMAT_I	=(1)<<18,
			
			
			TYPE_FORMAT_j	=TYPE_FORMAT_i,
			
			TYPE_FORMAT_J	=TYPE_FORMAT_I,
    
			
			TYPE_FORMAT_n	=(1)<<19,
			
			
			TYPE_FORMAT_N	=(1)<<20,
			
			
			TYPE_FORMAT_v	=(1)<<21,
			
			
			TYPE_FORMAT_V	=(1)<<22,
			
			
			
			TYPE_FORMAT_f	=(1)<<23,
			
			TYPE_FORMAT_F	=TYPE_FORMAT_f,
			
			
			TYPE_FORMAT_d	=(1)<<24,
						
			
			
			
			TYPE_FORMAT_D	=TYPE_FORMAT_d,
			
			
			TYPE_FORMAT_p	=(1)<<25,
			
			
			TYPE_FORMAT_P	=(1)<<26,
			
			
			TYPE_FORMAT_u	=(1)<<27,
			
			
			
			TYPE_FORMAT_U	=(1)<<28,
			
			
			
			
			
			
			
			
			
			TYPE_FORMAT_x	=(1)<<30,
			
			
			
			
					

 		};
 		
 		enum SpecifierModifier
 		{
			
			
			
			
			
			 		
			
			
			
			
			
			
			
			
			
			
			
				
			
			
			
			

			INPUT_TEXT		=(1)<<0,
			
						
			OUTPUT_TEXT		=(1)<<1,
			

			
			
			

			
			
			
			
			
			

			MODIFIER_AT		=(1)<<2, 	
			MODIFIER_EQ		=(1)<<3,	
			MODIFIER_LT		=(1)<<4,	
			MODIFIER_GT		=(1)<<5,	
			MODIFIER_EX		=(1)<<6,		
			MODIFIER = MODIFIER_AT|MODIFIER_EQ|MODIFIER_LT|MODIFIER_GT|MODIFIER_EX
			
 		};

		int Count;
		int Modifier;
		Substring Format;

		Specifier():Count(0),Modifier(0) {}
		Specifier(int options):Flags<64>(options),Count(0),Modifier(0) {}
	};

	struct Value : public Flags<32>
	{
		enum ValueTypes
		{
			TYPE_NUMBER		=(1)<<0,
			TYPE_STRING		=(1)<<1,
			TYPE_POINTER	=(1)<<2,

			
			TYPE_TEXT		=(1)<<3,
			TYPE_BINARY		=(1)<<4,

			
			TYPE_REAL		=(1)<<5,
			TYPE_INTEGER	=(1)<<6,
		};

		String String;
		Number Number;
		void * Pointer;

		Value & operator = (int mask) {Flags<32>::operator =(mask);return *this;}
	};

	
	enum PackingModes
	{
		MODE_NONE	=(0),
		MODE_WRITE	=(1)<<0,		
		MODE_READ	=(1)<<1,		

		
		
		
		

		
		
		
		
		
		
		
		
		
		
		
		

		MODE_TEXT	=(1)<<2,		
		MODE_BINARY	=(1)<<3,		

		
		
		
		
		
		
		MODE_SPECIFIC	=(1)<<4,

		
		

		
		
		
		
		
		
		
		
		
		


		MODE_BYTES	=(1)<<5,
		MODE_ARRAY	=(1)<<6,
	};

	int Mode;
	Array<Specifier> Specifiers;

	
	
	
	
	Array<int> Counts;

	Packing():Mode(0)
	{

	}

	Packing(const Substring& sequence, int mode):Mode(0)
	{
		Construct(sequence,mode);
	}

	Packing(const char * data, int mode):Mode(0)
	{
		Construct(data,mode);
	}

	void Construct(const Substring & sequence, int mode) {Construct(sequence.Data,sequence.Size,mode);}
	void Construct(const char * data, int mode) {Construct((char*)data,String::Length(data),mode);}
	void Construct(char * data, int size, int mode);
	void Construct(int mode);

	void Reset(int mode)
	{
		
		
		
		
		
		

		
		

		
		

		if ((mode&(MODE_READ|MODE_WRITE)))
		{ 
			Mode &= ~(MODE_READ|MODE_WRITE);
			Mode |= mode;

			
			Counts.Size = 0;
			Counts.Reserve(Specifiers.Size);
			Counts.Size = Specifiers.Size;
			Array<int>::Set(Counts.Data,0,Counts.Size);
		}
		else
		{
			Mode |= mode;
		}
	}


	
	
	

	Substring Before(int index)
	{
		Substring token;
		if (index > Specifiers.Length()-1)
			return token;

		if (index > 0)
		{
			Specifier & specifier = Specifiers[index];
			token = Specifiers[index-1].Format;
			token.Data += token.Size;
			token.Size = specifier.Format.Data - token.Data;
		}
		else
		{
			Specifier & specifier = Specifiers[index];
			token.Data = Packing::Data;
			token.Size = specifier.Format.Data - token.Data;
		}

		return token;
	}

	Substring After(int index)
	{		
		Substring token;
		if (index > Specifiers.Length()-1)
			return token;
		
		if (index < Specifiers.Length()-1)
		{
			Specifier & specifier = Specifiers[index+1];
			token = Specifiers[index].Format;
			token.Data += token.Size;
			token.Size = specifier.Format.Data - token.Data;
		}
		else
		{
			token = Specifiers[Specifiers.Length()-1].Format;
			token.Data += token.Size;
			token.Size = (Packing::Data+Packing::Size) - token.Data;
		}

		return token;
	}


};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



class PackingStream : public Packing, public StreamFilter
{
public:

	
	int Index;

	
	
	
	
	
	
	int Width;
	
	
	Token Token;
	
	
	PackingStream(const Substring & sequence, int mode, int width=1);
	PackingStream(const char * data, int mode, int width=1);	
	~PackingStream();

	
	void Format(String & string, int width=1);

	
	void Format(Substring & string, int width=1);

	

	int Read(String & string);
	int Read(Value & value);

	int ReadInteger(char & value);
	int ReadInteger(short & value);
	int ReadInteger(int & value);
	int ReadInteger(long & value);
	int ReadInteger(long long & value);
	int ReadString(String & value);
	int ReadString(char * data, int size);
	int ReadDouble(double & value);
	

	
	

	int Write(String & string);
	int Write(Value & value);

	int WriteInteger(char & value);
	int WriteInteger(short & value);
	int WriteInteger(int & value);
	int WriteInteger(long & value);
	int WriteInteger(long long & value);
	int WriteString(String & value);
	int WriteString(char * data, int size);
	int WriteDouble(double & value);
	

	
	

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Pack
{
public:

	PackingStream Packing;

	
	
	
	
	
	
	
	

	
	
	

	
	Pack(const Substring& format,int mode=0, int width=1):Packing(format,mode,width)
	{

	}

	Pack(const char * format,int mode=0, int width=1):Packing(format,mode,width)
	{
	}

	~Pack()
	{

	}


	Pack & operator << (const String &s) {Packing.WriteString((String&)s);return *this;}
	
	Pack & operator << (const char &c) {Packing.WriteInteger((char&)c);return *this;}
	Pack & operator << (const short &s) {Packing.WriteInteger((short&)s);return *this;}
	Pack & operator << (const int &i) {Packing.WriteInteger((int&)i);return *this;}
	Pack & operator << (const long &i) {Packing.WriteInteger((long&)i);return *this;}
	Pack & operator << (const long long &l) {Packing.WriteInteger((long long&)l);return *this;}
	
	
	
	
	
	
	Pack & operator << (const double & d) {Packing.WriteDouble((double&)d);return *this;}
	
	Pack & operator >> (String & s) {Packing.ReadString(s);return *this;}
	Pack & operator >> (char &c) {Packing.ReadInteger(c);return *this;}
	Pack & operator >> (short &s) {Packing.ReadInteger(s);return *this;}
	Pack & operator >> (int &i) {Packing.ReadInteger(i);return *this;}
	Pack & operator >> (long &l) {Packing.ReadInteger(l);return *this;}
	Pack & operator >> (long long &ll) {Packing.ReadInteger(ll);return *this;}
	
	
	
	Pack & operator >> (double &d) {Packing.ReadDouble(d);return *this;}
	

	
	
	
	
	Pack & operator >> (const char * d) {Packing.ReadString((char*)d,String::Length(d));return *this;}
	Pack & operator << (const char * d) {Packing.WriteString((char*)d,String::Length(d));return *this;}
	

	
	
	template<typename _Kind_, int _Size_>
	Pack & operator << (const _Kind_(&data) [_Size_])
	{
		int size = _Size_;
		int index = 0;
		while(index < size)
			operator << ((_Kind_&)data[index++]);

		return *this;
	}

	template<char * _Kind_, int _Size_>
	Pack & operator << (const char * (&data)[_Size_])
	{
		int size = String::Length(data);
		int index = 0;
		while(index < size)
			operator << ((char)data[index++]);

		return *this;
	}

	template<typename _Kind_, int _Size_>
	Pack & operator >> (const _Kind_(&data) [_Size_])
	{
		int size = _Size_;
		int index = 0;
		while(index < size)
			operator >> ((_Kind_&)data[index++]);

		return *this;
	}

	
	
	
	Pack & operator << (const Pack & pack)
	{
		if (&pack == this)
			return *this;
		
		((Pack&)pack).Packing.Stream = Packing.Stream;
		Packing.Stream = 0;

		return (Pack&)pack;
	}


	Pack & operator >> (const Pack & pack)
	{
		if (&pack == this)
			return *this;
		
		((Pack&)pack).Packing.Stream = Packing.Stream;
		Packing.Stream = 0;

		return (Pack&)pack;		
	}

	
	
	
	
	
	class PackAdapter
	{
	public:	

		class Substring Bytes;

		
		
		const Pack & Packed;

		PackAdapter(const Pack & pack, char * data, int size, int width=1):Bytes(data,size),Packed(pack) 
		{
			
			
			

			((Pack&)Packed).Packing.Format(Bytes,width);
			
		}

		PackAdapter(const Pack & pack, Substring & bytes, int width=1):Bytes(bytes),Packed(pack)
		{
			((Pack&)Packed).Packing.Format(Bytes,width);
		}


		PackAdapter & operator << (const String & s) {((Pack&)Packed).operator << (s); return *this;}
		PackAdapter & operator << (const char &c) {((Pack&)Packed).operator << (c); return *this;}
		PackAdapter & operator << (const short &s) {((Pack&)Packed).operator << (s); return *this;}
		PackAdapter & operator << (const int &i) {((Pack&)Packed).operator << (i); return *this;}
		PackAdapter & operator << (const long &l) {((Pack&)Packed).operator << (l); return *this;}
		PackAdapter & operator << (const long long &ll) {((Pack&)Packed).operator << (ll); return *this;}
		PackAdapter & operator << (const double &d) {((Pack&)Packed).operator << (d); return *this;}

		PackAdapter & operator >> (String & s) {((Pack&)Packed).operator >> (s); return *this;}
		PackAdapter & operator >> (char &c) {((Pack&)Packed).operator >> (c); return *this;}
		PackAdapter & operator >> (short &s) {((Pack&)Packed).operator >> (s); return *this;}
		PackAdapter & operator >> (int &i) {((Pack&)Packed).operator >> (i); return *this;}
		PackAdapter & operator >> (long &l) {((Pack&)Packed).operator >> (l); return *this;}
		PackAdapter & operator >> (long long &ll) {((Pack&)Packed).operator >> (ll); return *this;}
		PackAdapter & operator >> (double &d) {((Pack&)Packed).operator >> (d); return *this;}


		
		
		template<typename _Kind_, int _Size_>
		PackAdapter & operator << (const _Kind_(&data) [_Size_])
		{
			int size = _Size_;
			int index = 0;
			while(index < size)
				operator << (data[index++]);

			return *this;
		}

		template<typename _Kind_, int _Size_>
		PackAdapter & operator >> (const _Kind_(&data) [_Size_])
		{
			int size = _Size_;
			int index = 0;
			while(index < size)
				operator >> ((_Kind_&)data[index++]);

			return *this;
		}


		
		
		
		
		

		PackAdapter operator >> (const Pack& pack)
		{
			return PackAdapter(pack,Bytes,Packed.Packing.Width);
		}

		PackAdapter operator << (const Pack& pack)
		{
			return PackAdapter(pack,Bytes,Packed.Packing.Width);
		}



	};

};


Pack & operator << (String & string, Pack & pack);
Pack & operator << (const String & string, const Pack & pack);

Pack & operator >> (String & string, Pack & pack);
Pack & operator >> (const String & string, const Pack & pack);







///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






class Unpack : public Pack
{
public:

	Unpack(const Substring & format,int mode=0):Pack(format,mode)
	{
	}
	
	Unpack(const char * format,int mode=0):Pack(format,mode)
	{
	}

	~Unpack()
	{

	}

	Unpack & operator >> (String & s) {return (Unpack&)Pack::operator >> (s);}
	Unpack & operator >> (char &c) {return (Unpack&)Pack::operator >> (c);}
	Unpack & operator >> (short &s) {return (Unpack&)Pack::operator >> (s);}
	Unpack & operator >> (int &i) {return (Unpack&)Pack::operator >> (i);}
	Unpack & operator >> (long &l) {return (Unpack&)Pack::operator >> (l);}
	Unpack & operator >> (long long &ll) {return (Unpack&)Pack::operator >> (ll);}	
	Unpack & operator >> (double &d) {return (Unpack&)Pack::operator >> (d);}


	
	

	template<typename _Kind_, int _Size_>
	Pack & operator >> (const _Kind_(&data) [_Size_])
	{
		int size = _Size_;
		int index = 0;
		while(index < size)
			operator >> ((_Kind_&)data[index++]);

		return *this;
	}
};


Unpack & operator >> (String & string, Unpack & unpack);
Unpack & operator >> (const String & string, const Unpack & unpack);
Unpack & operator >> (const Substring & string, const Unpack & unpack);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




template<typename _Kind_, int _Size_>
Pack::PackAdapter operator >> (const _Kind_(&data) [_Size_], const Unpack & unpack)
{
	int size = sizeof(_Kind_)*_Size_;
	return Pack::PackAdapter(unpack,(char*)&data[0],size,sizeof(_Kind_));
}

template<typename _Kind_, int _Size_>
Pack::PackAdapter operator >> (const _Kind_(&data) [_Size_], const Pack & pack)
{
	int size = sizeof(_Kind_)*_Size_;
	return Pack::PackAdapter(pack,(char*)&data[0],size,sizeof(_Kind_));
}

template<typename _Kind_, int _Size_>
Pack::PackAdapter operator << (const _Kind_(&data) [_Size_], const Pack & pack)
{
	int size = sizeof(_Kind_)*_Size_;
	return Pack::PackAdapter(pack,(char*)&data[0],size,sizeof(_Kind_));
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


