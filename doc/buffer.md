# Buffer


This contains the `Buffer` and `StreamBuffer` classes.

In Hero `Stream`'s are both read and write, but derive from a separate `Reader` and `Writer` interface.

`Buffer` derives from `String` and implements the `Reader` and `Writer` interfaces to provide a FIFO/LIFO string buffer abstraction.

Writing to the buffer appends data

Reading from the buffer deletes data the the mode determining from which end (left or right).

FIFO mode reads data from the left, LIFO mode reads data from the right.

# Types

- Buffer
- StreamBuffer


# Usage

An example of using the `Buffer`

```cpp
#include "hero/buffer.h"
#include "hero/stream.h"
#include "hero/string.h"
#include "hero/structure.h"

StringStream stream;
stream << "the quick brown fox jumps over the lazy dog";

// Let's index all the spaces, it's easy because 
// StringStream is also a String
Array<int> array;
for (int i : Range(0,stream.Size))
	if (stream.Data[i] == ' ') array.Append(i);

// Add a final eof index
array.Append(stream.Size);

// Must be cast to either String or Stream since it can be both.
Buffer buffer;
buffer.Write((String&)stream);


// Iterate the array and read words
String string;
int offset=0;
Iterand<int> it = array.Forward();
while(it)
{
	// The Reader/Writer interfaces always has an amount argument
	// Reading will be from index 0 in FIFO mode
	buffer.Read(string,it()-offset);
	offset += string.Size+1;
	
	// Read a single byte for the space or eof
	buffer.Read();

	PrintArgLn(string);

	++it;
}

```


The `Buffer` interface is simple.

```cpp

class Buffer : public String, public Reader, public Writer
{
public:

	enum Modes
	{
		MODE_FIFO,
		MODE_LIFO,
	};

	int Mode;

	Buffer(int mode=MODE_FIFO):Mode(mode)
	{
	}

	bool IsWriteable() {return true;}
	bool IsReadable() {return true;}

	using Writer::Write;
	int Write(char * data , int size);

	using Reader::Read;
	int Read(char * data, int size);

	int Erase(int amount=1)
	{
		static char erase=0;
		int read=0;
		for (;amount > 0;--amount)
			read += Read(&erase,1);	
		return read;
	}
};

```


# Implementation



Most of the behavior is in the `Read()` method which you can see just copies data from either end and then uses `LTrim()` or `RTrim()` to resize the base `String`.

```cpp
int Buffer::Write(char * data , int size)
{
	String::Append(data,size);
	return size;
}


int Buffer::Read(char * data, int size)
{
	Assert((data == 0 && size == 0) || (data != 0 && size >= 0)); 

	if (data == 0 || size < 0) return 0;

	switch(Mode)
	{
	case MODE_FIFO:
		{
			if (size <= Size)
			{
				memcpy(data,Data,size);
				LTrim(size);
				return size;
			}
			else
			{
				int remaining = Size;
				if (remaining > 0)
				{
					memcpy(data,Data,remaining);
					LTrim(remaining);
					return remaining;
				}
			}
		}
		break;
	case MODE_LIFO:
		{
			if (size <= Size)
			{
				memcpy(data,Data+Size-size,size);
				RTrim(size);
				return size;
			}
			else
			{
				int remaining = Size;
				if (remaining > 0)
				{
					memcpy(data,Data+Size-remaining,remaining);
					RTrim(remaining);
					return remaining;
				}
			}
		}
		break;
	}

	return 0;
}

```