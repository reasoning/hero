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


#include "hero/generic.h"
#include "hero/traits.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Call
{
public:

	class Function
	{
	public:

	};


	
	
	

	class Base
	{
	public:
	};


	
	
	
	class Derived : public Base, public Function
	{
	public:
	};

	class Virtual : virtual Derived
	{
	public:		
	};


	class Unknown;

	typedef int (*CallFunction)(int);
	typedef int (Base::*CallBase)(int);
	typedef int (Derived::*CallDerived)(int);
	typedef int (Virtual::*CallVirtual)(int);
	typedef int (Unknown::*CallUnknown)(int);

	static const int SizeofFunction = sizeof(CallFunction);
	static const int SizeofBase = sizeof(CallBase);
	static const int SizeofDerived = sizeof(CallDerived);
	static const int SizeofVirtual = sizeof(CallVirtual);
	static const int SizeofUnknown = sizeof(CallUnknown);

	
	
	
	enum CallType
	{
		TYPE_FUNCTION	= 1,
		TYPE_BASE		= 2,
		TYPE_DERIVED	= 3,
		TYPE_VIRTUAL	= 4,
		TYPE_UNKNOWN	= 5,
        TYPE_FUNCTOR    = 6,
	};


    
    typedef int (*CallFunctor)(void *, int);
    
	
	
	union Calls
	{
        CallFunctor Functor;    
		CallFunction Function;
		CallBase Base;
		CallDerived Derived;
		CallVirtual Virtual;
		CallUnknown Unknown;

		Calls():Unknown(0) {}
	};

public:

	short Type;
	short Args;
	void * That;
	Calls Func;

	Call():
		Type(0),Args(0),That(0)
	{
	}

	Call(const Call & call):
		Type(call.Type),Args(call.Args),That(call.That),Func(call.Func)
	{
	}

	Call(short type, short args, void * that):
		Type(type),Args(args),That(that)
	{
	}

	operator bool ()
	{
		return Type != 0 && Func.Unknown != 0;
	}
	
    Call & operator = (const Call& call)
    {
        Type = call.Type;
        Args = call.Args;
        That = call.That;
        Func = call.Func;
        return *this;
    }

    struct Counted
    {
        int References;

        Counted():References(0) {}
        virtual ~Counted() {}
        
        void Increment()
        {
            ++References;
        }

        void Decrement()
        {
            --References;
            if (References <= 0)
                delete this;
        }
    };


    
    
    

    template<typename _Functor_,typename _Return_, typename... _Args_>
    struct Functor : Counted
    {
    public:

        _Functor_ Func;

        Functor(_Functor_ && func):Func(func) 
        {

        }

        ~Functor() 
        {

        }

        
		_Return_ operator () (_Args_&&... args)
        {
            return Func((_Args_&&)(args)...);
			
        }

    };

};




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



template <typename _Return_, typename... _Args_>
class Callback : public Call
{
public:

	#define Sizeofcall(call) (sizeof(call)<=SizeofBase?2:(sizeof(call)<=SizeofDerived?3:(sizeof(call)<=SizeofVirtual?4:(sizeof(call)<=SizeofUnknown?5:0))))


    typedef _Return_ (*ThunkType)(Calls & func, void * that, _Args_...args);

    ThunkType Thunk;
    
    static _Return_ ThunkFunctor(Calls & func, void * that,_Args_...args)
    {
        typedef _Return_ (*FunctorInvoke)(void *, _Args_...);
        return ((FunctorInvoke)func.Functor)(that,(_Args_&&)args...); 
    }
    
    static _Return_ ThunkFunction(Calls & func, void * that,_Args_...args)
    { 
        typedef _Return_ (*FunctionPrototype)(_Args_...); 
        return ((FunctionPrototype)func.Function)((_Args_&&)args...); 
    }

	static _Return_ ThunkBase(Calls & func, void * that,_Args_...args)
    { 
        typedef _Return_ (Base::*BasePrototype)(_Args_...); 
        return (((Base*)that)->*((BasePrototype)func.Base))((_Args_&&)(args)...);
    }
    
    static _Return_ ThunkDerived(Calls & func, void * that,_Args_...args)  
    { 
        typedef _Return_ (Derived::*DerivedPrototype)(_Args_...); 
        return (((Derived*)that)->*((DerivedPrototype)func.Derived))((_Args_&&)(args)...);
    }

    static _Return_ ThunkVirtual(Calls & func, void * that,_Args_...args)   
    { 
        typedef _Return_ (Virtual::*VirtualPrototype)(_Args_...); 
        return (((Virtual*)that)->*((VirtualPrototype)func.Virtual))((_Args_&&)(args)...);
    }
    
    static _Return_ ThunkUnknown(Calls & func, void * that,_Args_...args)   
    { 
        typedef _Return_ (Unknown::*UnknownPrototype)(_Args_...); 
        return (((Unknown*)that)->*((UnknownPrototype)func.Unknown))((_Args_&&)(args)...); 
    }

	Callback() 
	{
        Thunk = 0;
	}

    ~Callback()
    {   
        if (Type == TYPE_FUNCTOR)
        {
            ((Call::Counted*)That)->Decrement();
        }
    }

    
    
    
    
    
    
    
    
    
    
    template<typename _R_,typename... _A_>    
    Callback(const Callback<_R_, _A_...> & callback):Call((Call&)callback)
    
    
    {        
        Thunk = callback.Thunk;
        if (Type == TYPE_FUNCTOR)
        {
            ((Call::Counted*)That)->Increment();
        }
        
    }
    
    Callback & operator = (const Callback & callback)
    {
        Call::operator = ((Call&)callback);
        
        Thunk = callback.Thunk;
        if (Type == TYPE_FUNCTOR)
        {
            ((Call::Counted*)That)->Increment();
        }

        return *this;
    }

    Callback(int null):Call()
    {
        
        Thunk = 0;
    }

    template<typename _Functor_>
    Callback(_Functor_ && func, 
        typename Traits::Enable<!Traits::Castable<_Functor_,Call>::Value>::Type ** type=0):
        Call(6,-1,0)
    {
        
        typedef Functor<_Functor_,_Return_,_Args_...> FunctorType;
        struct FunctorInvoke
        {            
            
            static _Return_ Invoke(void * that,_Args_...args)
            {   
                return ((FunctorType*)that)->operator()((_Args_&&)args...);
            }
        };

        
        
        
        That = (void*)new FunctorType((_Functor_&&)func);
        
        
        
        
        struct Info {_Return_ (*func)(void*,_Args_...);} info = {&FunctorInvoke::Invoke};
        memcpy(&Func,&info,sizeof(info));

        Type = Call::TYPE_FUNCTOR;

        
        ((Call::Counted*)That)->Increment();

        Thunk = &ThunkFunctor;

    }

	template<typename _Class_>
	Callback(_Return_ (_Class_::*call)(_Args_...)):Call(Sizeofcall(call),-1,0)
	{
		struct Info {_Return_ (_Class_::*func)(_Args_...);} info = {call};        
		memcpy(&Func,&info,sizeof(info));

        switch(Type)
        {
        case TYPE_BASE: Thunk = &ThunkBase;break;
        case TYPE_DERIVED: Thunk = &ThunkDerived;break;
        case TYPE_VIRTUAL: Thunk = &ThunkVirtual;break;
        default: Thunk = &ThunkUnknown;
        }
	}

	template<typename _Class_>
	Callback(_Class_ * that, _Return_ (_Class_::*call)(_Args_...)):Call(Sizeofcall(call),-1,that)
	{
		struct Info {_Return_ (_Class_::*func)(_Args_...);} info = {call};
		memcpy(&Func,&info,sizeof(info));

        switch(Type)
        {
        case TYPE_BASE: Thunk = &ThunkBase;break;
        case TYPE_DERIVED: Thunk = &ThunkDerived;break;
        case TYPE_VIRTUAL: Thunk = &ThunkVirtual;break;
        default: Thunk = &ThunkUnknown;
        }
	}

	Callback(_Return_ (*call)(_Args_...)):Call(1,-1,0)
	{
		struct Info {_Return_ (*func)(_Args_...);} info = {call};
		memcpy(&Func,&info,sizeof(info));
        
        Thunk = &ThunkFunction;
	}

    _Return_ operator() (_Args_...args) const
	{        

		#ifdef HERO_USING_TRACY
		
        ZoneScoped;
		#endif

        return Thunk((Calls&)Func,That,(_Args_&&)(args)...);

        
        
	}	

    
    template<typename _Class_>
    _Return_ operator() (_Class_ * that, _Args_... args) const
	{
		#ifdef HERO_USING_TRACY
		
        ZoneScoped;
		#endif

        return Thunk((Calls&)Func,that,(_Args_&&)(args)...);

        
       
	}


};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////