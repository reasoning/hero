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

#include "hero/path.h"
#include "hero/parser.h"
#include "hero/string.h"
#include "hero/text.h"
#include "hero/structure.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Arg
{
public:

	String Key;

	Array<String> Opts;

	Array<String> Values;

	enum Bound
	{
		BIND_NONE		= (0),
		BIND_BOOL		= (1)<<0,
		BIND_INT		= (1)<<1,
		BIND_LONGLONG	= (1)<<2,
		BIND_DOUBLE		= (1)<<3,
		BIND_STRING		= (1)<<4,
		BIND_MAP		= (1)<<5,
		BIND_SET		= (1)<<6,
		BIND_ARRAY		= (1)<<7,
		BIND_FUNC		= BIND_MAP|BIND_SET|BIND_ARRAY
	};

	struct Binder
	{
		int Type;
		int Index;
		void * Ptr;
		Callback<void> Func;

		Binder(int type, int index, void* ptr=0):
			Type(type),Index(index),Ptr(ptr)
		{
		}

		Binder(int type, int index, const Callback<void> & func):
			Type(type),Index(index),Ptr(0),Func(func)
		{
		}

		Binder():
			Type(0),Index(0),Ptr(0)
		{
		}

		bool Apply(Arg & arg)
		{
			if (arg.Values.Size > Index)
			{

				switch(Type)
				{
				case BIND_NONE: return false;	
				case BIND_BOOL: *((bool*)Ptr) = (bool)arg.Boolean(Index); break;
				case BIND_INT: *((int*)Ptr) = (int)arg.Integer(Index); break;
				case BIND_LONGLONG: *((long long*)Ptr) = (long long)arg.Integer(Index); break;
				case BIND_DOUBLE: *((double*)Ptr) = (double)arg.Real(Index); break;
				case BIND_STRING: *((String*)Ptr) = arg.Str(Index); break;

				case BIND_FUNC: Func(); break;
				}
			}

			return true;
		}
	};

	Array<Binder> Bindings;

	struct Base
	{
		Arg * Self;
		int Index;		
		Base(Arg * self,int index):Self(self),Index(index) {}
	};

	template<typename _Kind_>
	struct Getter : Base
	{
		Getter(Arg * self, int index):Base(self,index) {}
		String operator ()() {return Self->Str(Index);}
	};

	Arg(const String & key):Key(key)
	{
	}

	Arg()
	{
	}

	Arg(const Arg & arg):
		Key(arg.Key),Opts(arg.Opts),Values(arg.Values),Bindings(arg.Bindings)
	{
	}

	bool Apply()
	{
		Iterand<Binder> binding = Bindings.Forward();
		while (binding)
		{
			binding().Apply(*this);
			++binding;
		}

		return true;
	}

	bool Option(Substring opt)
	{

		opt.Trim().LTrim("-");		

		Opts.Append(opt);
		return true;
	}

	bool Match(Substring opt)
	{

		opt.Trim().LTrim("-");		

		Iterand<String> option = Opts.Forward();
		while (option)
		{

			if (option().Is(opt,true))
				return true;
			++option;
		}

		return false;	
	}

	Arg & Bind(bool & value, int index=0) {Bindings.Append(Binder(BIND_BOOL,index,(void*)&value)); return *this;}
	Arg & Bind(int & value, int index=0) {Bindings.Append(Binder(BIND_INT,index,(void*)&value)); return *this;}
	Arg & Bind(long long & value, int index=0) {Bindings.Append(Binder(BIND_LONGLONG,index,(void*)&value)); return *this;}
	Arg & Bind(double & value, int index=0) {Bindings.Append(Binder(BIND_DOUBLE,index,(void*)&value)); return *this;}
	Arg & Bind(String & value, int index=0) {Bindings.Append(Binder(BIND_STRING,index,(void*)&value)); return *this;}

	template< template<typename,typename> class _Map_, typename _Key_, typename _Value_>
	Arg & Bind(_Map_<_Key_,_Value_> & map, int index=0)
	{

		struct Inserter
		{
			_Map_<_Key_,_Value_> & Map;
			Getter<_Value_> Get;
			Inserter(Arg * self, int index, _Map_<_Key_,_Value_> & map):
				Get(self,index),Map(map) {}

			void operator() ()
			{	
				Map.Insert(Get.Self->Key, Get());

			}				
		};

		Bindings.Append(Binder(BIND_FUNC,index,Callback<void>(Inserter(this,index,map))));
		return *this;
	}

	template<typename _Value_>
	Arg & Bind(Callback<void,_Value_> & func, int index=0)
	{

		struct Inserter
		{
			Callback<void,_Value_> Func;
			Getter<_Value_> Get;
			Inserter(Arg * self, int index, Callback<void,_Value_> & func):
				Get(self,index),Func(func) {}

			void operator () ()
			{				
				Func(Get());				
			}
		};

		Bindings.Append(Binder(BIND_FUNC,index,Callback<void>(Inserter(this,index,func))));		
		return *this;
	}

	template<typename _Value_>
	Arg & Bind(Hero::Array<_Value_> & array, int index=0)
	{
		struct Inserter
		{

			Hero::Array<_Value_> & Array;
			Getter<_Value_> Get;

			Inserter(Arg * self, int index, Hero::Array<_Value_> & array):
				Get(self,index),Array(array) {}

			void operator () ()
			{

				Array.Resize(Get.Index+1);
				Array[Get.Index] = Get();
			}
		};

		Bindings.Append(Binder(BIND_FUNC,index,Callback<void>(Inserter(this,index,array))));	
		return *this;
	}	

	bool Boolean(int index=0)
	{
		if (index >= Values.Size) return false;
		return Values[index].Boolean();
	}

	long long Integer(int index=0)
	{
		if (index >= Values.Size) return 0;
		return Values[index].Integer();
	}	

	double Real(int index=0)
	{
		if (index >= Values.Size) return 0.0;
		return Values[index].Real();
	}	

	String Str(int index=0)
	{
		if (index >= Values.Size) return "";
		return Values[index];
	}			

};

template<>
struct Arg::Getter<bool> : Arg::Base
{
	Getter(Arg * self, int index):Base(self,index) {}
	bool operator ()() {return (bool)Self->Boolean(Index);}
};	

template<>
struct Arg::Getter<int> : Arg::Base
{
	Getter(Arg * self, int index):Base(self,index) {}
	int operator ()() {return (int)Self->Integer(Index);}
};	

template<>
struct Arg::Getter<long long> : Arg::Base
{
	Getter(Arg * self, int index):Base(self,index) {}
	long long operator ()() {return (long long)Self->Integer(Index);}
};	

template<>
struct Arg::Getter<double> : Arg::Base
{
	Getter(Arg * self, int index):Base(self,index) {}
	double operator ()() {return (double)Self->Real(Index);}
};	

class Options : public String
{
public:

	List<Arg*> Args;
	Arg Null;

	Options()
	{

	}

	~Options()
	{
		Args.Destroy();
	}

	Arg & Option(const Substring & key, const Substring & opt1)
	{
		Arg * arg = new Arg(key);
		arg->Option(opt1);
		Args.Append(arg);
		return *arg;
	}

	Arg & Option(const Substring & key, const Substring & opt1, const Substring & opt2)
	{
		Arg * arg = new Arg(key);
		arg->Option(opt1);
		arg->Option(opt2);

		Args.Append(arg);
		return *arg;
	}	

	Arg & Option(const Substring & key, const Substring & opt1, const Substring & opt2, const Substring & opt3)
	{
		Arg * arg = new Arg(key);
		arg->Option(opt1);
		arg->Option(opt2);
		arg->Option(opt3);

		Args.Append(arg);
		return *arg;
	}	

	bool Contains(const Substring & key)
	{
		Iterand<Arg*> args = Args.Forward();
		while (args)
		{
			if (args()->Key.Is(key))
				return true;				
			++args;
		}

		return false;
	}

	Arg & operator[] (const Substring & key)
	{
		Iterand<Arg*> args = Args.Forward();
		while (args)
		{
			if (args()->Key.Is(key))
				return *args();		
			++args;
		}

		return Null;	
	}

	bool Parse(const char * data) {return Parse((char*)data,String::Length(data));}
	bool Parse(const Substring & str) {return Parse(str.Data,str.Size);}
	bool Parse(char * data, int size);
	bool Parse(int argc, char * argv[]);
	bool Parse();

	bool Apply()
	{
		Iterand<Arg*> args = Args.Forward();
		while (args)
		{
			args()->Apply();
			++args;
		}

		return true;		
	}

	bool Match(const Substring & opt, Array<Substring> & values);

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Args : public Path
{
public:

	Args(){}
	Args(const char * data) {Construct(data);}
	Args(char * data, int size) {Construct(data,size);}
	Args(const Substring & str) {Construct(str);}
	Args(int argc, char * argv[]) {Construct(argc,argv);}

	bool Construct(const Substring & str) {return Construct(str.Data,str.Size);}
	bool Construct(const char * data) {return Construct((char*)data,String::Length(data));}
	bool Construct(char * data, int size);
	bool Construct(int argc, char * argv[]);

	Segment * Select(const Substring & name, bool caseless = false) {return Select(name.Data,name.Size,caseless);}
	Segment * Select(const char * name, bool caseless = false) {return Select((char*)name,String::Length(name),caseless);}
	Segment * Select(char * name, int size, bool caseless = false);

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Properties : public Multimap<String,String>
{
public:

	Properties():Multimap(Comparable::COMPARE_GENERAL)
	{

	}

	bool Contains(const char * data){return Contains((char*)data,String::Length(data));}
	bool Contains(const Substring & str){return Contains(str.Data,str.Size);}	
	bool Contains(char * data, int size)
	{

		return Multimap<String,String>::Select(String(data,size))!=0;
	}

	String & operator [] (const char * data)
	{
		Substring substring(data);
		return operator [] (substring);
	}

	String & operator [] (const Substring & str) 
	{
		Result res = Select(String(str));
		if (res)
		{

			return Values[res.Index];
		}
		else
		{
			res = Insert(str);
			Assert(res);
			return Values[res.Index];
		}
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Section
{
public:

	String Name;
	Hero::Properties Properties;

	Section()
	{
	}

	Section(const String & name):Name(name)
	{
	}

	bool Contains(const char * data){return Contains((char*)data,String::Length(data));}
	bool Contains(const Substring & str){return Contains(str.Data,str.Size);}	
	bool Contains(char * data, int size)
	{
		return Properties.Contains(data,size);
	}

	String & operator [] (const char * data) {return Properties[data];}

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Configuration
{
public:

	List<Hero::Section*> Sections;

	Configuration();
	~Configuration();

	void Insert(const String & name, const String & value){Insert("",name,value);}
	void Insert(const String & section, const String & name, const String & value)
	{
		for (int i=0;i<Sections.Count;++i)
		{
			if (Sections[i]->Name.Is(section))
			{
				Sections[i]->Properties.Insert(name,value);
				return;
			}
		}

		Hero::Section * sec = new Hero::Section(section);
		sec->Properties.Insert(name,value);
		Sections.Append(sec);
	}

	bool Load(const char * data){return Load((char*)data,String::Length(data));}
	bool Load(const Substring & str){return Load(str.Data,str.Size);}	
	bool Load(char * data, int size);

	bool Contains(const char * data){return Contains((char*)data,String::Length(data));}
	bool Contains(const Substring & str){return Contains(str.Data,str.Size);}	
	bool Contains(char * data, int size)
	{
		for (int i=0;i<Sections.Count;++i)
		{
			if (Sections[i]->Properties.Contains(data,size))
				return true;
		}

		return false;
	}

	Hero::Section * Section(const char * data){return Section((char*)data,String::Length(data));}
	Hero::Section * Section(const Substring & str){return Section(str.Data,str.Size);}	
	Hero::Section * Section(char * data, int size)
	{
		for (int i=0;i<Sections.Count;++i)
		{
			if (Sections[i]->Properties.Contains(data,size))
				return Sections[i];
		}

		return 0;
	}

	String * Property(const char * data){return Property((char*)data,String::Length(data));}
	String * Property(const Substring & str){return Property(str.Data,str.Size);}	
	String * Property(char * data, int size)
	{
		for (int i=0;i<Sections.Count;++i)
		{
			String string(data,size);
			if (Sections[i]->Properties.Contains(data,size))
				return &Sections[i]->Properties[string.Data];
		}

		return 0;
	}
};

class ConfigurationFile : public TextFile
{
public:

	ConfigurationFile(){}
	ConfigurationFile(const File & file):TextFile(file){}
	ConfigurationFile(const Substring & str):TextFile(str){}
	ConfigurationFile(const char * path):TextFile(path){}
	ConfigurationFile(char * path, int size):TextFile(path,size){}
	~ConfigurationFile(){}

	void Read(Properties & properties);
	void Write(Properties & properties);
};

class ConfigurationParser : public StringParser
{
public:

	ConfigurationParser(const Substring & str):StringParser(str) {}

	void ParseSection(Section & section);
	void ParseProperties(Properties & properties);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Settings
{
public:

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

