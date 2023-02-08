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
#include "hero/structure.h"
#include "hero/platform.h"
#include "hero/atomic.h"
#include "hero/sync.h"

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




class Error : public Object
{
public:

	
	
	enum ErrorType
	{
		TYPE_MESSAGE,
		TYPE_OBJECT,
	};

	int Type;

	Error(int type):Type(type) {}

	using Object::Print;
	void Print(String & string) 
	{
		switch (Type)
		{
		case TYPE_MESSAGE:string = "ErrorMessage";break;
		case TYPE_OBJECT:string = "ErrorMessage";break;
		}
	}
};

class ErrorMessage : public Error, public String
{
public:
	
	ErrorMessage(int type):Error(type) {}
	ErrorMessage(const Substring & msg):Error(TYPE_MESSAGE),String(msg) {}
	ErrorMessage(const char * msg):Error(TYPE_MESSAGE),String(msg) {}
	ErrorMessage(char * data, int size):Error(TYPE_MESSAGE),String(data,size) {}
	
};

class ErrorObject: public ErrorMessage
{
public:

	String Id;
	String Prefix;
	
	ErrorObject():ErrorMessage(TYPE_OBJECT) {}
	ErrorObject(const Substring& id, const Substring& prefix=""):ErrorMessage(TYPE_OBJECT),Id(id),Prefix(prefix) {}
	ErrorObject(const char * id, const char * prefix=""):ErrorMessage(TYPE_OBJECT),Id(id),Prefix(prefix) {}
};


void RaiseError(char * data, int size);
void RaiseError(Error * error);

template<typename _Error_>
void RaiseError(const char * msg)
{
	RaiseError(new _Error_(msg));
}


class Raise
{
public:

	template<typename _Error_>
	Raise() {Construct(new _Error_());}

	template<typename _Error_>
	Raise(const char * error)
	{
		RaiseError(new _Error_(error));
	}

	template<typename _Error_>
	Raise(char * data, int size)
	{
		RaiseError(new _Error_(data,size));
	}

	Raise(Error & error);
	Raise(Error * error);
	Raise(const Substring & error);
	
	Raise(char * data, int size);
	Raise(const char * format, ...);

private:

	void Construct(Error * error);

	
	
};



class Except {};
class StackTrace {};






class Level : Printable
{
public:
	
	
	
	
	

	Strong<class Error*> Error;
	Strong<StackTrace*> Stack;

	int Severity;

	Level(const class Error & error, int severity=0):Error(&error),Severity(severity) {}
	Level(const class Error * error, int severity=0):Error(error),Severity(severity) {}

	Level(const Substring & error, int severity=0):Error(new ErrorMessage(error)),Severity(severity) {}
	Level(const char * error, int severity=0):Error(new ErrorMessage(error)),Severity(severity) {}
	Level(char * data, int size, int severity=0):Error(new ErrorMessage(data,size)),Severity(severity) {}
	
	Level():Severity(0)
	{
	}

	using Printable::Print;
	virtual void Print(String & string)
	{
		Error->Print(string);
	}
};






class Try : public Stack<Level>
{
public:

	
	Try();

	
	
	
	
	

	~Try();
};





class Catch : public Stack<Level>
{
public:

	Catch();

	operator bool () 
	{
		return Length();
	}
};



class Unwind : public ThreadLocal<Stack<Try*>>
{
public:
	
	
	
	
	
	

	Unwind()
	{

	}

	~Unwind()
	{
		
		
		

	}

	
	using Result = typename Stack<Try*>::Result;
	
	
	
	

	Result Push(typename Template<Try*>::ConstReference kind) {return  Get().Push(kind);}	
	
	bool Pop(typename Template<Try*>::ConstReference kind) {return  Get().Pop(kind);}
	bool Pop(typename Template<Try*>::Reference kind) {return  Get().Pop(kind);}
	bool Pop() {return Get().Pop();}

	bool Pull(typename Template<Try*>::ConstReference kind) {return  Get().Pull(kind);}
	bool Pull(typename Template<Try*>::Reference kind) {return  Get().Pull(kind);}
	bool Pull() {return  Get().Pull();}

	Result Peek() {return  Get().Peek();}
	bool Peek(typename Template<Try*>::Reference kind) {return  Get().Peek(kind);}

	Try*& operator[] (int index) {return  Get()[index];}


	int Length() {return  Get().Length();}


	Result Remove(typename Template<Try*>::ConstReference kind) {return  Get().Remove(kind);}	
	Result Remove(typename Template<Try*>::Reference kind, int index) {return  Get().Remove(kind,index);}		
	Result RemoveAt(int index) {return  Get().RemoveAt(index);}	
	

	static Unwind & Singleton();

};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class BaseError : public ErrorObject 
{
public:

	BaseError() {}
	BaseError(const Substring& id, const Substring& prefix=""):ErrorObject(id,prefix) {}
	BaseError(const char * id, const char * prefix=""):ErrorObject(id,prefix) {}
};

#define ERROR_GENERATOR(Type) \
class Type : public BaseError \
{ \
public: \
	Type():BaseError(#Type,"Error") {} \
	Type(const Substring & msg):BaseError(#Type,"Error") {Construct(msg);} \
	Type(const char * msg):BaseError(#Type,"Error") {Construct(msg);} \
	Type(char * data, int size):BaseError(#Type,"Error") {Construct(data,size);} \
};


ERROR_GENERATOR(AssertError)

ERROR_GENERATOR(IndexError)
ERROR_GENERATOR(ArgumentError)

ERROR_GENERATOR(UnderflowError)
ERROR_GENERATOR(OverflowError)
ERROR_GENERATOR(SignError)

ERROR_GENERATOR(IOError)

ERROR_GENERATOR(KeyError)
ERROR_GENERATOR(ValueError)
ERROR_GENERATOR(TypeError)

ERROR_GENERATOR(ConversionError)
ERROR_GENERATOR(AssignmentError)

ERROR_GENERATOR(CallbackError)
ERROR_GENERATOR(NotFoundError)
ERROR_GENERATOR(NotImplementedError)
ERROR_GENERATOR(ReferenceError)
ERROR_GENERATOR(RuntimeError)
ERROR_GENERATOR(SyntaxError)
ERROR_GENERATOR(SystemError)
ERROR_GENERATOR(EnvironmentError)
ERROR_GENERATOR(TimeoutError)

ERROR_GENERATOR(SocketError)
ERROR_GENERATOR(StreamError)
ERROR_GENERATOR(FileError)
ERROR_GENERATOR(FolderError)
ERROR_GENERATOR(StorageError)
ERROR_GENERATOR(DataError)
ERROR_GENERATOR(StringError)

ERROR_GENERATOR(NumberError)
ERROR_GENERATOR(MemoryError)
ERROR_GENERATOR(ThreadError)
ERROR_GENERATOR(AsyncError)
ERROR_GENERATOR(SyncError)
ERROR_GENERATOR(FlagError)
ERROR_GENERATOR(StateError)


class Token;

class ParserError : public BaseError
{ 
public: 

	

	class ParserErrorToken : public String
	{
	public:

		int Position;	
		int Offset;		

		int Line;		
		int Column;		

		ParserErrorToken():Position(0),Offset(0),Line(0),Column(0) {};
		ParserErrorToken(const class Token & token);
	};
	
	ParserErrorToken Token;

	ParserError(const ParserErrorToken & token=ParserErrorToken()):
		BaseError("ParserError","Error"),Token(token) 
	{

	}	
		
	ParserError(const Substring & msg, const ParserErrorToken & token=ParserErrorToken())
		:BaseError("ParserError","Error"),Token(token) 
	{
		Construct(msg);
	}

	ParserError(const char * msg, const ParserErrorToken & token=ParserErrorToken())
		:BaseError("ParserError","Error"),Token(token) 
	{	
		Construct(msg);
	}

	ParserError(char * data, int size, const ParserErrorToken & token=ParserErrorToken())
		:BaseError("ParserError","Error"),Token(token) 
	{
		Construct(data,size);
	}
};




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define Verify(value) (new Try(), value)

inline bool Verified()
{
	Unwind & unwind = Unwind::Singleton();
	if (unwind.Length())
	{
		Unwind::Result res = unwind.Peek();

		
		Try * t = unwind[res.Index];
		Catch c;
		delete t;

		
		return !c;
	}

	return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
