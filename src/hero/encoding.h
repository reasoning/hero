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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "hero/string.h"
#include "hero/binary.h"
#include "hero/stream.h"
#include "hero/structure.h"
#include "hero/buffer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Code;

class Encoder
{
public:

	virtual int Encode(Reader & reader);

	virtual int Encode(Array<int> & array, int index=0) {return Encode(array.Data,array.Size,index);}
	virtual int Encode(int array[],int length, int index=0);

	virtual int Encode(String & string) {return Encode(string.Data,string.Size);}
	virtual int Encode(Substring & sequence) {return Encode(sequence.Data,sequence.Size);}	
	virtual int Encode(char * data, int size);

	virtual int Encode() {return 0;}
	virtual int Encode(char data[], int size, int type)=0;

protected:

	virtual int Encode(int type, class Code * code)=0;
	virtual int Encode(char data[], int size, int type, class Code * code)=0;
};

class Decoder
{
public:

	virtual int Decode(Reader & reader);

	virtual int Decode(Array<int> & array, int index=0) {return Decode(array.Data,array.Size,index);}
	virtual int Decode(int array[],int length, int index=0);

	virtual int Decode(String & string) {return Decode(string.Data,string.Size);}
	virtual int Decode(Substring & sequence) {return Decode(sequence.Data,sequence.Size);}	
	virtual int Decode(char * data, int size);

	virtual int Decode() {return 0;}
	virtual int Decode(char data[], int size, int type)=0;

protected:

	virtual int Decode(int type, class Code * code)=0;
	virtual int Decode(char data[], int size, int type, class Code * code)=0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Code
{
public:

	enum CodeType
	{
		TYPE_8BIT	=(1),
		TYPE_16BIT	=(2),
		TYPE_32BIT	=(4),
	};

	Buffer Encoded;
	Buffer Decoded;

	int Type;

	Code():Type(0)
	{
	}

	virtual ~Code()
	{
	}

	virtual int Put(char * data, int size, int type)=0;

	virtual int Get(char *& data, char * end, int & bytes, int type)
	{
		bytes=0;
		char * byte = (char*)&bytes;
		int size = end-data;
		size = (size<type)?size:type;
		switch (size)
		{
			case 4: *byte++ = *data++;
			case 3:	*byte++ = *data++;
			case 2:	*byte++ = *data++;
			case 1:	*byte++ = *data++;
		}
		return size;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Codec: public Encoder, public Decoder
{
public:

	enum CodecType
	{
		TYPE_ENCODER,
		TYPE_DECODER,
	};

	class CodeEncoder : public Code
	{
	public:

		class Encoder & Encoder;
		CodeEncoder(class Encoder & encoder):Encoder(encoder)
		{
		}

		int Put(char * data, int size, int type)
		{
			return Encoder.Encode(data,size,type);
		}
	};

	class CodeDecoder : public Code
	{
	public:

		class Decoder & Decoder;
		CodeDecoder(class Decoder & decoder):Decoder(decoder)
		{
		}

		int Put(char * data, int size, int type)
		{
			return Decoder.Decode(data,size,type);
		}
	};

	class CodeWriter : public Code
	{
	public:

		class Writer & Writer;

		CodeWriter(class Writer & writer):Writer(writer)
		{
		}

		int Put(char * data, int size, int type)
		{			
			int put = Writer.Write(data,size);
			return put;
		}
	};

	class CodeArray : public Code
	{
	public:

		class Array<int> & Array;
		int Index;

		CodeArray(class Array<int> & array, int index=0):Array(array),Index(index)
		{	
		}

		int Put(char * data, int size, int type)
		{
			int put=0;
			int bytes=0;
			char * end = data+size;
			while (data != end)
			{
				bytes=0;
				Get(data,end,bytes,type);
				Array.Insert(bytes,Index++);
				++put;
			}	
			return put;
		}
	};

	class CodeString : public Code
	{
	public:

		class String & String;
		CodeString(class String & string):String(string)
		{
		}

		int Put(char * data, int size, int type)
		{
			String.Append(data,size);
			return size;
		}
	};

	class CodeChar : public Code
	{
	public:

		char * Data;
		int Size;
		int Index;

		CodeChar(char * data, int size, int index=0):Data(data),Size(size),Index(index)
		{
		}

		int Put(char * data, int size, int type)
		{
			int put=0;
			char * end = data+size;
			while (data != end && Index < Size)
			{
				Data[Index++] = *data++;
				++put;
			}
			return put;
		}
	};

	class CodeInt : public Code
	{
	public:
		int * Data;
		int Size;
		int Index;

		CodeInt(int * data, int size, int index=0):Data(data),Size(size),Index(index)
		{
		}

		int Put(char * data, int size, int type)
		{
			int put=0;
			char * end = data+size;
			int bytes=0;
			while (data != end && Index < Size)
			{
				bytes=0;
				Get(data,end,bytes,type);
				Data[Index++] = bytes;
				++put;
			}	
			return put;
		}
	};

public:

	class Code * Code;

	Codec():Code(0)
	{
	}

	Codec(Encoder & encoder):Code(0) {Coding(encoder);}
	Codec(Decoder & decoder):Code(0) {Coding(decoder);}
	Codec(Codec & codec, int type):Code(0) {Coding(codec,type);}
	Codec(Writer & writer):Code(0) {Coding(writer);}
	Codec(Array<int> & array, int index=0):Code(0) {Coding(array,index);}
	Codec(int *data, int size, int index=0):Code(0) {Coding(data,size,index);}
	Codec(char *data, int size, int index=0):Code(0) {Coding(data,size,index);}
	Codec(Substring & sequence):Code(0) {Coding(sequence);}
	Codec(String & string):Code(0) {Coding(string);}

	~Codec()
	{
		if (Code) delete Code;
	}

	void Coding(Encoder & encoder)
	{
		if (Code) delete Code;
		Code = new CodeEncoder(encoder);
	}

	void Coding(Decoder & decoder)
	{
		if (Code) delete Code;
		Code = new CodeDecoder(decoder);
	}

	void Coding(Codec & codec, int type)
	{
		if (Code) delete Code;
		switch(type)
		{
		case Codec::TYPE_ENCODER: Code = new CodeEncoder((Encoder&)codec);
		case Codec::TYPE_DECODER: Code = new CodeDecoder((Decoder&)codec);
		}	
	}

	void Coding(Writer & writer)
	{
		if (Code) delete Code;
		Code = new CodeWriter(writer);
	}

	void Coding(Array<int> & array, int index=0)
	{
		if (Code) delete Code;
		Code = new CodeArray(array,index);
	}

	void Coding(int *data, int size, int index=0)
	{
		if (Code) delete Code;
		Code = new CodeInt(data,size,index);
	}

	void Coding(char *data, int size, int index=0)
	{
		if (Code) delete Code;
		Code = new CodeChar(data,size);
	}

	void Coding(Substring & sequence)
	{
		if (Code) delete Code;
		Code = new CodeChar(sequence.Data,sequence.Size);
	}

	void Coding(String & string)
	{
		if (Code) delete Code;
		Code = new CodeString(string);
	}

	virtual bool IsEncoding() {return Code && !Code->Encoded.IsEmpty();}
	virtual bool IsDecoding() {return Code && !Code->Decoded.IsEmpty();}

	using Encoder::Encode;
	virtual int Encode(char data[], int size, int type) {return Encode(data,size,type,Code);}

	using Decoder::Decode;
	virtual int Decode(char data[], int size, int type) {return Decode(data,size,type,Code);}

protected:

	virtual int Encode(int type, class Code * code);
	virtual int Encode(char data[], int size, int type, class Code * code);

	virtual int Decode(int type, class Code * code);
	virtual int Decode(char data[], int size, int type, class Code * code);

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CodecStream : public StreamBuffer 
{
public:

	class Codec & Codec;

	CodecStream(class Stream & stream, class Codec & codec):
		StreamBuffer(stream),Codec(codec)
	{
	}

	~CodecStream()
	{
	}

	virtual int ReadDirect(char *data, int size)
	{

		return 0;
	}

	virtual int WriteDirect(char * data, int size)
	{

		return 0;
	}

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Ascii : public Codec
{
public:

	using Codec::Encode;
	using Codec::Decode;

	int Encode(char data[], int size, int type, class Code * code);
	int Decode(char data[], int size, int type, class Code * code);

};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Hex : public Codec
{
public:

	static const char Encoding[];
	static const char Decoding[];

	using Codec::Encode;
	using Codec::Decode;

	int Encode(char data[], int size, int type, class Code * code);
	int Decode(char data[], int size, int type, class Code * code);

	static int Encode(char & byte, char ascii[2]);
	static int Decode(char ascii[2], char & byte);

	static int Encode(char * bytes, int size, String & ascii);
	static int Encode(String & bytes, String & ascii) {return Encode(bytes.Data,bytes.Size,ascii);}

	static int Decode(char * ascii, int size, String & bytes);
	static int Decode(String & ascii, String & bytes) {return Decode(ascii.Data,ascii.Size,bytes);}
};

class Octal : public Codec
{
public:

	using Codec::Encode;
	using Codec::Decode;

	int Encode(char data[], int size, int type, class Code * code);
	int Decode(char data[], int size, int type, class Code * code);

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Base64 : public Codec
{
public:

	Base64()
	{
	}

	static const char Encoding[];
	static const char Decoding[];

	using Codec::Encode;
	using Codec::Decode;

	int Encode() 
	{

		return Encode(Code::TYPE_8BIT,Code);
	}

	int Encode(char data[], int size, int type, class Code * code);
	int Encode(int type, class Code * code);

	int Decode(char data[], int size, int type, class Code * code);

	static int Encode(char bytes[3], char ascii[4], int padded=0);
	static int Decode(char ascii[4], char bytes[3]);

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Unicode
{
public:

	enum Encoding
	{
		ENCODING_UTF8,
		ENCODING_UTF16,
		ENCODING_UTF32,
		ENCODING_UCS2,
	};

	bool IsUtf8() {};
	bool IsUtf16() {};
	bool IsUtf32() {};
	bool IsUcs2() {};

	static bool IsAscii(int code)
	{

		return (code >= 0x00 && code <= 0x7F);
	}

	static bool IsUnicode(int code)
	{

		return (code >= 0 && code <= 0x10FFFF);
	}

	static bool IsBom(int code)
	{

		return (code == 0xFEFF || code == 0xFFFE);
	}

	static bool IsLittleEndian(int code)
	{

		return (code == 0xFEFF && Binary::IsLittleEndian()) || (code == 0xFFFE && Binary::IsBigEndian());
	}

	static bool IsBigEndian(int code)
	{

		return (code == 0xFEFF && Binary::IsBigEndian()) || (code == 0xFFFE && Binary::IsLittleEndian());
	}

	static bool IsIdeographic(int code)
	{

		return (code >= 0x0800 && code <= 0xFFFF);
	}

	static bool IsSurrogate(int code)
	{

	}

	static int GuessEncoding(Substring & sequence)
	{

	}

	static int Directionality(int code)
	{
	}

	static int Plane(int code)
	{

		return code|0x00FF0000;
	}

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 class Utf : public Codec
{
public:

	int Bom;

	Utf():Bom(0)
	{
	}

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Utf8 : public Utf
{
public:

	struct FormednessStruct
	{
		unsigned int Code[2];
		unsigned char Byte[8];
	};

	static const FormednessStruct Formedness[];

	using Utf::Encode;
	int Encode(char data[], int size, int type, class Code * code);

	using Utf::Decode;
	int Decode(char data[], int size, int type, class Code * code);

	static bool IsWellFormed(int code, char byte[4]);

	static int Encode(int * code, char byte[4]);
	static int Decode(char byte[4], int * code);

	static int Count(int code);
	static int Count(char byte);
};

class Utf16 : public Utf
{
public:

	using Utf::Encode;
	int Encode(char data[], int size, int type, class Code * code);

	using Utf::Decode;
	int Decode(char data[], int size, int type, class Code * code);

	static bool IsWellFormed(int code, short word[2]);

	static int Encode(int * code, short word[2]);	
	static int Decode(short word[2], int * code);

	static int Count(int code);
	static int Count(short word);

	static void Swap(short word[2]);

	static int Reorder(short word[2], int bom);
};

class Utf32 : public Utf
{
public:

	using Utf::Encode;
	int Encode(char data[], int size, int type, class Code * code);

	using Utf::Decode;
	int Decode(char data[], int size, int type, class Code * code);

	static bool IsWellFormed(int code, int words[1]);

	static int Encode(int * code, int words[1]);	
	static int Decode(int words[1], int * code);

	static int Count(int code);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Utf7
{

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

