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




#include "hero/encoding.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


namespace Hero {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int Encoder::Encode(Reader & reader)
{
	String string;
	string.Allocate(4096);

	
	

	int encoded=0;
	int encode = 0;

	reader.Read(string);

	while(string.Size > 0)
	{
		
		
		
		encode = Encode(string);
		string.LTrim(string.Size);

		encoded += encode;
		
		if (string.Size == 0)
			reader.Read(string);			
	}

	return encoded;
}

int Encoder::Encode(int array[],int length, int index)
{
	if (index < length)
		return Encode((char*)(array+index),(length-index)*4,Code::TYPE_32BIT);
	else
		return 0;
}

int Encoder::Encode(char * data, int size)
{
	return Encode(data,size,Code::TYPE_8BIT);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int Decoder::Decode(Reader & reader)
{
	String string;
	string.Allocate(4096);

	
	

	int decoded=0;
	int decode = 0;

	
	
	reader.Read(string);

	while(string.Size > 0)
	{
		decode = Decode(string);
		string.LTrim(string.Size);

		decoded += decode;
		
		if (string.Size == 0)
			reader.Read(string);			
	}

	return decoded;
}

int Decoder::Decode(int array[],int length, int index)
{
	if (index < length)
		return Decode((char*)(array+index),(length-index)*4,Code::TYPE_32BIT);
	else
		return 0;
}

int Decoder::Decode(char * data, int size)
{
	return Decode(data,size,Code::TYPE_8BIT);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Codec::Encode(int type, class Code * code)
{
	
	if (code && IsEncoding())
	{
		int coding = 0;
		coding += code->Put(code->Encoded.Data,code->Encoded.Size,type);
		code->Encoded.Release();
		
		return coding;
	}

	return 0;
}

int Codec::Encode(char data[], int size, int type, class Code * code)
{
	
	if (code) code->Put(data,size,type);
	return 0;
}

int Codec::Decode(int type, class Code * code)
{
	
	if (code && IsDecoding())
	{
		int coding = 0;
		coding += code->Put(code->Decoded.Data,code->Decoded.Size,type);
		code->Decoded.Release();

		
		return coding;
	}

	return 0;
}

int Codec::Decode(char data[], int size, int type, class Code * code)
{
	
	if (code) code->Put(data,size,type);
	return 0;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Ascii::Encode(char data[], int size, int type, class Code * code)
{
	
	
	
	
	
	
	
	int encoded=0;
	if (Code)
	{
		char bad = '?';
		int bytes=0;
		char * byte = (char*)&bytes;
		char * end = data+size;
		while(data != end)
		{
			Code->Get(data,end,bytes,type);
			encoded += Code->Put((bytes > 127)?&bad:byte,1,Code::TYPE_8BIT);
		}
	}
	return encoded;
}

int Ascii::Decode(char data[], int size, int type, class Code * code)
{
	if (type != Code::TYPE_8BIT) return 0;

	int decoded=0;
	if (Code) decoded = Code->Put(data,size,Code::TYPE_8BIT);
	return decoded;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char Hex::Encoding[] = {		
	'0','1','2','3','4','5','6','7','8','9', 
	'a','b','c','d','e','f'
};

const char Hex::Decoding[] = {
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	

	
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	
	 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0,	

	
	 0,10,11,12,13,14,15, 0, 0, 0, 0, 0, 0, 0, 0, 0,	
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	

	
	 0,10,11,12,13,14,15, 0, 0, 0, 0, 0, 0, 0, 0, 0,	
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	
};

int Hex::Encode(char data[], int size, int type, class Code * code)
{	
	
	
	
	
	
	
	
	int encoded=0;
	int encode=0;
	
	if (IsEncoding())
	{
		encoded = Codec::Encode(Code::TYPE_8BIT,code);
		if (!encoded) return 0;
	}


	char byte;
	char ascii[2];

	char * end = data+size;
	while(data != end)
	{
		code->Decoded.Write(data++,1);
		
		code->Decoded.Read(byte);
		Encode(byte,ascii);
		code->Encoded.Write(ascii,2);	
		
		if ((encode=Codec::Encode(Code::TYPE_8BIT,code)) < 2)
			break;

		encoded += encode;
	}
	
	return encoded;	
	
}

int Hex::Decode(char data[], int size, int type, class Code * code)
{
	if (type != Code::TYPE_8BIT) return 0;
	
	
	
	int decoded=0;
	int decode=0;
	
	if (IsDecoding())
	{
		decoded = Codec::Decode(Code::TYPE_8BIT,code);
		if (!decoded) return 0;
	}

	char byte;
	char ascii[2];	

	char * end = data+size;
	while(data != end)
	{
		while (code->Encoded.Size < 2 && data != end)	
			code->Encoded.Write(data++,1);

		if (code->Encoded.Size < 2)
			break;
		
		code->Encoded.Read(ascii,2);	
		int decode = Decode(ascii,byte);
		code->Decoded.Write(&byte,decode);
		
		if ((decode=Codec::Decode(Code::TYPE_8BIT,code)) < 1)
			break;

		decoded += decode;
	}
	
	return decoded;	
}

int Hex::Encode(char &byte, char ascii[2])
{
	ascii[0] = Encoding[(byte>>4)&0x0F];
	ascii[1] = Encoding[byte&0x0F];
	return 2;
}


int Hex::Decode(char ascii[2], char &byte)
{
	byte = (Decoding[ascii[0]]<<4)|(Decoding[ascii[1]]);
	return 1;
}

int Hex::Encode(char * bytes, int size, String & ascii)
{
	
	
	ascii.Reserve(size*2);
	int encoded = 0;
	
	char encode[2];
	char * end = bytes+size;
	while(bytes < end)
	{
		encode[0] = Encoding[(*bytes>>4)&0x0F];
		encode[1] = Encoding[*bytes&0x0F];
		
		ascii.Append(encode,2);	
		encoded += 2;
		++bytes;
	}
	
	return encoded;
}

int Hex::Decode(char * ascii, int size, String & bytes)
{
	bytes.Reserve(size/2);
	int decoded = 0;
	
	char decode[1];
	char * end = ascii+size-1;
	while(ascii < end)
	{
		
		
		
		
		
		
		
		
		
		
		
		decode[0] = ((Decoding[ascii[0]]<<4)|(Decoding[ascii[1]]));
		ascii += 2;
		
		bytes.Append(decode,1);		
		decoded += 1;
	}
	
	return decoded;
}
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Octal::Encode(char data[], int size, int type, class Code * code)
{
	
	
	int encoded=0;
	int encode=0;
	
	
	
	return encoded;
		
}

int Octal::Decode(char data[], int size, int type, class Code * code)
{
	if (type != Code::TYPE_8BIT) return 0;
	
	
	
	
	
	
	
	
	int decoded=0;
	int decode=0;

	
	
	return decoded;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char Base64::Encoding[] = {		
	
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',         
	
	
	'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z', 		
	
	
	'0','1','2','3','4','5','6','7','8','9', 
	
	
	'+','/' 
};

const char Base64::Decoding[] = {
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	

	
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,62, 0, 0, 0,63,	
	52,53,54,55,56,57,58,59,60,61, 0, 0, 0, 0, 0, 0,	

	
	 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,	
	15,16,17,18,19,20,21,22,23,24,25, 0, 0, 0, 0, 0,	

	
	 0,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,	
	41,42,43,44,45,46,47,48,49,50,51, 0, 0, 0, 0, 0,	
};

int Base64::Encode(int type, class Code * code)
{
	
	

	
	
		
	
	type = Code::TYPE_8BIT;


	int encoded=0;
	int encode=0;
	
	while(IsEncoding() || IsDecoding())
	{
		if (code->Encoded.Size == 0)
		{
			if (code->Decoded.Size > 0)
			{
				
				
				

				char bytes[3] = {0,0,0};
				char ascii[4] = {0,0,0,0};

				
				
				
				int encode = code->Decoded.Read(bytes,(code->Decoded.Size<3)?code->Decoded.Size:3);
				Encode(bytes,ascii,(encode<3)?3-encode:0);
				code->Encoded.Write(ascii,4);

				
				

				if (!(encode=Codec::Encode(Code::TYPE_8BIT,code)))
					break;

				encoded += encode;
			}
			else
			{
				
				break;
			}
		}
		else
		{
			
			if (!(encode=Codec::Encode(type,code)))
				break;

			encoded += encode;
		}
	}
	
	return encoded;
}


int Base64::Encode(char data[], int size, int type, class Code * code)
{
	
	
	
	
	
	

	
	type = Code::TYPE_8BIT;
	
	int encoded=0;
	int encode=0;
	
	if (IsEncoding())
	{
		encoded = Codec::Encode(Code::TYPE_8BIT,code);
		if (!encoded) return 0;
	}


	char bytes[3];
	char ascii[4];

	char * end = data+size;
	while(data != end)
	{
		while (code->Decoded.Size < 3 && data != end)
			code->Decoded.Write(data++,1);

		if (code->Decoded.Size < 3)
			break;

		code->Decoded.Read(bytes,3);
		Encode(bytes,ascii);
		code->Encoded.Write(ascii,4);	
		
		if ((encode=Codec::Encode(Code::TYPE_8BIT,code)) < 4)
			break;

		encoded += encode;
	}
	
	return encoded;
}

int Base64::Decode(char data[], int size, int type, class Code * code)
{


	
	
	
	

	
	type = Code::TYPE_8BIT;

	int decoded=0;
	int decode=0;
	
	if (IsDecoding())
	{
		decoded = Codec::Decode(Code::TYPE_8BIT,code);
		if (!decoded) return 0;
	}

	char bytes[3];
	char ascii[4];	

	char * end = data+size;
	while(data != end)
	{
		while (code->Encoded.Size < 4 && data != end)
			code->Encoded.Write(data++,1);

		if (code->Encoded.Size < 4)
			break;
		
		code->Encoded.Read(ascii,4);	
		int decode = Decode(ascii,bytes);
		code->Decoded.Write(bytes,decode);
		
		
		
		if ((decode=Codec::Decode(Code::TYPE_8BIT,code)) < 3)
			break;

		decoded += decode;
	}
	
	return decoded;

}


int Base64::Decode(char ascii[4], char bytes[3])
{
	

	
	
	
	
	
	bytes[0] = (Decoding[ascii[0]]<<2)|(Decoding[ascii[1]]>>4);
	bytes[1] = ((Decoding[ascii[1]]&0x0F)<<4)|(Decoding[ascii[2]]>>2);
	bytes[2] = ((Decoding[ascii[2]]&0x03)<<6)|(Decoding[ascii[3]]);
	
	
	int decoded=3;
	if (ascii[3] == '=') --decoded;
	if (ascii[2] == '=') --decoded;

	return decoded;
}

int Base64::Encode(char bytes[3], char ascii[4], int padded)
{	
	
	
	
	
	
	
	
	

	if (padded)
	{
		if (padded == 2)
		{
			
			ascii[0] = Encoding[(bytes[0]>>2)&0x3F];
			ascii[1] = Encoding[((bytes[0]&0x03)<<4)|0x00];
			ascii[2] = '=';
			ascii[3] = '=';
			return 4;
		}
		else
		if (padded == 1)
		{
			
			ascii[0] = Encoding[(bytes[0]>>2)&0x3F];
			ascii[1] = Encoding[((bytes[0]&0x03)<<4)|((bytes[1]>>4)&0x0F)];
			ascii[2] = Encoding[((bytes[1]&0x0F)<<2)|0x00];
			ascii[3] = '=';
			return 4;
		}

		
	}

	ascii[0] = Encoding[(bytes[0]>>2)&0x3F];
	ascii[1] = Encoding[((bytes[0]&0x03)<<4)|((bytes[1]>>4)&0x0F)];
	ascii[2] = Encoding[((bytes[1]&0x0F)<<2)|((bytes[2]>>6)&0x03)];
	ascii[3] = Encoding[bytes[2]&0x3F];
	return 4;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



const Utf8::FormednessStruct Utf8::Formedness[] = {
{{0x0000,0x007F},{0x00,0x7F,0x00,0x00,0x00,0x00,0x00,0x00}},
{{0x0080,0x07FF},{0xC2,0xDF,0x80,0xBF,0x00,0x00,0x00,0x00}},
{{0x0800,0x0FFF},{0xE0,0xE0,0xA0,0xBF,0x80,0xBF,0x00,0x00}},
{{0x1000,0xCFFF},{0xE1,0xEC,0x80,0xBF,0x80,0xBF,0x00,0x00}},
{{0xD000,0xD7FF},{0xED,0xED,0x80,0x9F,0x80,0xBF,0x00,0x00}},
{{0xE000,0xFFFF},{0xEE,0xEF,0x80,0xBF,0x80,0xBF,0x00,0x00}},
{{0x10000,0x3FFFF},{0xF0,0xF0,0x90,0xBF,0x80,0xBF,0x80,0xBF}},
{{0x40000,0xFFFFF},{0xF1,0xF3,0x80,0xBF,0x80,0xBF,0x80,0xBF}},
{{0x100000,0x10FFFF},{0xF4,0xF4,0x80,0x8F,0x80,0xBF,0x80,0xBF}}};

bool Utf8::IsWellFormed(int code, char byte[4])
{
	
	
	if (code >= 0xD800 && code <= 0xDFFF) return false;

	
	

	for (int f=0;f<9;++f)
	{
		if ((unsigned int)code >= Formedness[f].Code[0] && (unsigned int)code <= Formedness[f].Code[1])
		{
			if ((unsigned char)byte[0] < Formedness[f].Byte[0] || (unsigned char)byte[0] > Formedness[f].Byte[1]) return false;
			if ((unsigned char)byte[1] < Formedness[f].Byte[2] || (unsigned char)byte[1] > Formedness[f].Byte[3]) return false;
			if ((unsigned char)byte[2] < Formedness[f].Byte[4] || (unsigned char)byte[2] > Formedness[f].Byte[5]) return false;
			if ((unsigned char)byte[3] < Formedness[f].Byte[6] || (unsigned char)byte[3] > Formedness[f].Byte[7]) return false;

			return true;
		}
	}

	return false;
}


int Utf8::Encode(char data[], int size, int type, class Code * code)
{
	int bytes=0;
	char * byte = (char*)&bytes;

	int utf8s;
	char * utf8 = (char*)&utf8s;
	int count=0;

	int encoded=0;
	int encoding=0;
	
	if (IsEncoding())
	{
		encoded = Codec::Encode(Code::TYPE_8BIT,code);
		if (!encoded) return 0;
	}

	char * end = data+size;
	while(data != end)
	{
		Code->Get(data,end,bytes,type);
		count = Encode(&bytes,utf8);
		
		
		
		if (!count) continue;
		
		code->Encoded.Write(utf8,count);

		if ((encoding=Codec::Encode(Code::TYPE_8BIT,code)) < count)
			break;

		encoded += encoding;

		count = 0;
		utf8s = 0;		
	}
	
	return encoded;
}

int Utf8::Decode(char data[], int size, int type, class Code * code)
{
	
	
	
	

	if (type != Code::TYPE_8BIT) return 0;

	
	
	
	
	

	
	

	int count=0;

	int utf8s=0;
	char * utf8 = (char*)&utf8s;

	int bytes=0;
	char * byte = (char*)&bytes;

	int decoded=0;
	int decoding=0;
	
	if (IsDecoding())
	{
		decoded = Codec::Decode(Code::TYPE_32BIT,code);
		if (!decoded) return 0;
	}

	char * end = data+size;
	while(data != end || IsEncoding())
	{
		bytes = 0;
		count = 0;
		decoding = 0;

		if (!code->Encoded.IsEmpty())
		{
			decoding = code->Encoded.Read(byte,1);
		}
		else
		{
			byte[decoding++] = *data++;
		}

		count = Count(*byte);
		if (count)
		{
			
			
			if (!code->Encoded.IsEmpty())
				decoding += code->Encoded.Read(byte+1,count-1);

			while (data != end && decoding < count)
			{
				byte[decoding++] = *data++;
			}			

			if (decoding < count)
			{
				
				
				
				code->Encoded.Write(byte,decoding);

				count = 0;
				while (++count < decoding)
				{
					if (Count(byte[count]) > 0)
					{
						code->Encoded.Read(byte,count);
						break;
					}
				}

				if (count < decoding)
					continue;

				break;
			}
			else
			{
				count = Decode(byte,&utf8s);	
				if (count)
				{
					
					
					code->Decoded.Write(utf8,4);
					if (!(decoding=Codec::Decode(Code::TYPE_32BIT,code)))
						break;

					decoded += decoding;
					continue;
				}
			}				
		}

		
		code->Encoded.Write(byte+1,decoding-1);
	}
	
	return decoded;
}

int Utf8::Count(int code)
{
	

	
	
	
	
	

	if ((code&0xFFFFFF80) == 0) return 1;
	if ((code&0xFFFFF800) == 0) return 2;
	if ((code&0xFFFF0000) == 0) return 3;
	if (code <= 0x10FFFF) return 4;
	return 0;
}

int Utf8::Count(char byte)
{
	
	

	
	
	
	
	

	if ((byte&0x80) == 0) return 1;
	if ((byte&0xE0) == 0xC0) return 2;
	if ((byte&0xF0) == 0xE0) return 3;
	if ((byte&0xF8) == 0xF0) return 4;
	return 0;
}

int Utf8::Encode(int * code, char byte[4])
{

	
	

	
	
	
	
	

	int count=Count(*code);

	switch(count)
	{
	case 1:
		{
			
			byte[0] = ((*code)&0x0000007F)>>0;
		}
	break;
	case 2:
		{
			
			byte[0] = 0xC0 | ((*code)&0x000007C0)>>6;
			byte[1] = 0x80 | ((*code)&0x0000003F)>>0;
		}
	break;
	case 3:
		{
			
			byte[0] = 0xE0 | ((*code)&0x0000F000)>>12;
			byte[1] = 0x80 | ((*code)&0x00000FC0)>>6;
			byte[2] = 0x80 | ((*code)&0x0000003F)>>0;
		}
	break;
	case 4:
		{
			
			byte[0] = 0xF0 | ((*code)&0x001F0000)>>18;
			byte[1] = 0x80 | ((*code)&0x0003F000)>>12;
			byte[2] = 0x80 | ((*code)&0x00000FC0)>>6;
			byte[3] = 0x80 | ((*code)&0x0000003F)>>0;
		}
	break;
	default:
		{
			
		}
	}

	return count;
}

int Utf8::Decode(char byte[4], int * code)
{
	int count = Count(*byte);
	
	*code = 0;	
	unsigned char * data = (unsigned char*) byte;

	switch (count)
	{
	case 1:
		{
			
			
			
			
			
			
			
			

			
			
			
			(*code) = (int)*data;				
			++data;
		}
	break;
	case 2:
		{
			
			
			
			
			
			
			

			
			
			

			(*code) = ((*data)&0x1F)<<6;
			++data;
			(*code) |= ((*data)&0x3F);
			++data;
		}
	break;
	case 3:
		{
			
			
			
			

			(*code) = ((*data)&0x0F)<<12;
			++data;
			(*code) |= ((*data)&0x3F)<<6;
			++data;
			(*code) |= ((*data)&0x3F);
			++data;
		}
	break;
	case 4:
		{
			
			
			
			
			

			(*code) = ((*data)&0x07)<<18;
			++data;
			(*code) |= ((*data)&0x3F)<<12;
			++data;
			(*code) |= ((*data)&0x3F)<<6;
			++data;
			(*code) |= ((*data)&0x3F);
			++data;
		}
	break;
	default:
		{
			
		}
	}

	
	
	
	if (count)
	{
		if (!IsWellFormed((*code),byte))
		{
			count = 0;
		}
	}

	return count;	
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Utf16::IsWellFormed(int code, short word[2])
{
	
	
	

	if (code >= 0 && code <= 0x10FFFF)
	{
		if (word[2] == 0 && word[1] >= 0xD800 && word[1] <= 0xDFFF) return false;

		return true;
	}

	return false;
}

int Utf16::Encode(char data[], int size, int type, class Code * code)
{
	int count=0;

	int utf16s=0;
	short * utf16 = (short*)&utf16s;

	int bytes=0;
	char * byte = (char*)&bytes;

	int encoded=0;
	int encoding=0;
	
	if (IsEncoding())
	{
		encoded = Codec::Encode(type,code);
		if (!encoded) return 0;
	}

	char * end = data+size;
	while(data != end)
	{
		Code->Get(data,end,bytes,type);
		count = Encode(&bytes,utf16);
		
		
		
		if (!count) continue;
		
		code->Encoded.Write((char*)utf16,count*2);

		if ((encoding=Codec::Encode(Code::TYPE_16BIT,code)) < count*2)
			break;

		encoded += encoding;

		count = 0;
		utf16s = 0;		
	}
	
	return encoded;
}

int Utf16::Decode(char data[], int size, int type, class Code * code)
{
	
	
	
	
	
	
	

	

	
	
	
	
	

	int count=0;
	
	int utf16s=0;
	short * utf16 = (short*)&utf16s;

	int bytes=0;
	short * word = (short*)&bytes;
	char * byte = (char*)&bytes;

	int decoded=0;
	int decoding=0;
	
	if (IsDecoding())
	{
		decoded = Codec::Decode(Code::TYPE_32BIT,code);
		if (!decoded) return 0;
	}

	char * end = data+size;
	while(data != end || (IsEncoding() && code->Encoded.Size % 2 == 0))
	{
		bytes = 0;
		count = 0;
		decoding = 0;

		if (code->Encoded.Size == 2 || code->Encoded.Size == 4)
		{
			decoding = code->Encoded.Size;
			code->Encoded.Read(byte,2);
			count = Count(*word);
			if (count)
			{
				if (count*2 == decoding)
				{
					if (count == 2)
						code->Encoded.Read(byte+2,2);
					
					count = Decode(word,&utf16s);
					if (count)
					{
						
						code->Decoded.Write((char*)utf16,4);

						if (!(decoding=Codec::Decode(Code::TYPE_32BIT,code)))
							break;

						decoded += decoding;
					}
					else
					{
						code->Encoded.Write(byte+2,decoding-2);
					}

					continue;
				}
				else
				{
					
					code->Encoded.Write(byte,decoding);

					if (code->Encoded.Size == 2 && data == end)
						break;
				}

			}

			decoding = 0;
		}

		if (decoding == 0)
		{
			if (data != end)
			{
				code->Encoded.Write(data++,1);
			}
		}
	}
	
	return decoded;
}


int Utf16::Count(int code)
{
	
	if ((code >= 0x0000 && code <= 0xD7FF) || (code >= 0xE000 && code <= 0xFFFF)) return 1;
	if (code <= 0x10FFFF) return 2;
	return 0;
}

int Utf16::Count(short word)
{
	
	

	
	
	
	

	if ((word >= 0x0000 && word <= 0xD7FF) || (word >= 0xE000 && word <= 0xFFFF)) return 1;
	if ((word&0xFC00) == 0xD800) return 2;
	return 0;
}

void Utf16::Swap(short word[2])
{
	char byte=0;
	byte = ((char*)word)[0];
	((char*)word)[1] = ((char*)word)[0];
	((char*)word)[0] = byte;

	byte = ((char*)word)[2];
	((char*)word)[3] = ((char*)word)[2];
	((char*)word)[2] = byte;

	


	
}


int Utf16::Encode(int * code, short word[2])
{
	int count=Count(*code);

	
	
	
	

	
	
	
	

	switch(count)
	{	
	case 1:
		{
			
			

			
			
			
			
			
			word[0] = ((*code)&0x0000FFFF);
		}
	break;
	case 2:
		{
			
			
			
			
			

			
			
			
			
			
			

			word[0] = 0xD800 | ((((*code)&0x001F0000)>>16)-1)<<6 | ((*code)&0x0000FC00)>>10;
			word[1] = 0xDC00 | ((*code)&0x00000300)>>0 | ((*code)&0x000000FF)>>0;
		}
	break;
	default:
		{
			
		}
	}

	return count;
}

int Utf16::Decode(short word[2], int * code)
{
	*code = 0;
	int count=Count(*word);

	
	
	
	

	
	
	
	

	
	
	
	
	
	unsigned short * data = (unsigned short*)word;

	switch(count)
	{
	case 1:
		{
			
			
			
						
			(*code) = (*data);
			++data;
		}
	break;
	case 2:
		{
			(*code) = ((*data)&0x03C0)>>6;
			
			
			
			
			

			(*code) = ((*code)+1) << 16;
			
			(*code) |= ((*data)&0x003F)<<10;			
			++data;

			if (((*data)&0xFC00) == 0xDC00)
			{
				(*code) |= ((*data)&0x03FF);
				++data;
			}		
			else
			{
				
				count = 0;
			}
		}
	break;
	default:
		{
			
		}
	}

	
	

	if (count)
	{
		if (!IsWellFormed((*code),word))
		{
			count = 0;
		}
	}

	return count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int Utf32::Encode(char data[], int size, int type, class Code * code)
{
	int count=0;

	int utf32s=0;
	int * utf32 = (int*)&utf32s;

	int bytes=0;
	int * words = (int*)&bytes;
	char * byte = (char*)&bytes;

	int encoded=0;
	int encoding=0;
	
	if (IsEncoding())
	{
		encoded = Codec::Encode(Code::TYPE_32BIT,code);
		if (!encoded) return 0;
	}

	char * end = data+size;
	while(data != end)
	{
		Code->Get(data,end,bytes,type);
		count = Encode(&bytes,utf32);
		
		if (!count) continue;
		
		code->Encoded.Write((char*)utf32,count*4);

		if ((encoding=Codec::Encode(Code::TYPE_32BIT,code)) < count*4)
			break;

		encoded += encoding;

		count = 0;
		utf32s = 0;		
	}
	
	return encoded;
}

int Utf32::Decode(char data[], int size, int type, class Code * code)
{
	int count=0;
	
	int utf32s=0;
	int * utf32 = (int*)&utf32s;

	
	int bytes=0;
	int * words = (int*)&bytes;
	char * byte = (char*)&bytes;

	int decoded=0;
	int decoding=0;

	if (IsDecoding())
	{
		decoded = Codec::Decode(Code::TYPE_32BIT,code);
		if (!decoded) return 0;
	}
	
	char * end = data+size;
	while(data != end || (IsEncoding() && code->Encoded.Size % 2 == 0))
	{
		bytes = 0;
		count = 0;
		decoding = 0;

		if (code->Encoded.Size == 4)
		{
			decoding = code->Encoded.Size;
			code->Encoded.Read(byte,4);
			count = Decode(words,utf32);
			if (count)
			{
				code->Decoded.Write((char*)utf32,4);

				if (!(decoding=Codec::Decode(Code::TYPE_32BIT,code)))
					break;
				
				decoded += decoding;
			}
			else
			{
				
				continue;
			}

			decoding = 0;
		}

		if (decoding == 0)
		{
			if (data != end)
			{
				code->Encoded.Write(data++,1);
			}
		}
	}
	
	return decoded;
}


bool Utf32::IsWellFormed(int code, int words[1])
{
	if (code >= 0x0000D800 && code <= 0x0000DFFF) return false;
	if (code > 0x0010FFFF || code < 0x00000000) return false;

	if ((*words) >= 0x0000D800 && (*words) <= 0x0000DFFF) return false;
	if ((*words) > 0x0010FFFF || (*words) < 0x00000000) return false;		
	
	return true;
}

int Utf32::Encode(int * code, int words[1])
{
	(*words) = 0;
	(*words) = (*code);

	if (!IsWellFormed(*code,words))
		return 0;

	return 1;
}

int Utf32::Decode(int words[1], int * code)
{
	(*code) = 0;
	(*code) = (*words);

	if (!IsWellFormed(*code,words))
		return 0;

	return 1;
}

int Utf32::Count(int code)
{
	if (code >= 0x0000D800 && code <= 0x0000DFFF) return 0;
	if (code > 0x0010FFFF || code < 0x00000000) return 0;
	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

