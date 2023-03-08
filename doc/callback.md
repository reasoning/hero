# Callback

`Callback<>` is a generic function pointer.  Unlike std::function it represents functions in a "this" pointer independent way.  You can create a callback to a member function and optionally supply a this during creation, but then change the this when calling.

The std::function requires separate definitions to do this, thus makes it hard (like most things in the standard library) to write non templated generic code.

`Callback<>` accepts functors (struct's with overloaded operator ()) and lambdas.  For these two types it allocates a small struct to behave like the "this" pointer and facilicate type erasure. 

For all other types of function there is no allocation.

# Types
- Callback<>

# Usage

Presently `Callback<>` deliberately has no assignment operator (since that can't express the "this" pointer semantics) so you need to assign via constructor, this 
also make's things a little more explicit.

```cpp

    struct Helper
    {

        static void OnStatic(void * arg)
        {
            PrintLn("static: %p\n",arg);
        }

        void OnMember(void * arg)
        {
            PrintLn("this: %p member: %p\n",this,arg);
        }
    };

    Helper helper;

    // Run the callback with a pointer to itself
    Callback<void,void*> ca;
    Callback<void,void*> cb;
    void * thing = &cb;
    void * thang = &ca;

    PrintLn("thing: %p\n",thing);
    PrintLn("thang: %p\n",thang);
    
    // Static function
    cb = &Helper::OnStatic;        
    cb(thing);
    ca = cb;
    ca(thing);

    // Functor
    cb = helper;
    cb(thing);
    ca = cb;
    ca(thing);

    // Member function with this
    cb = Callback<void,void*>(&helper,&Helper::OnMember);
    cb(thing);
    ca = cb;
    ca(thing);

    // Member function with delayed this
    cb = &Helper::OnMember;
    cb(&helper,thing);
    ca = cb;

    // Can be called without valid this, provided it doesnt access any local 
    // variables it wont crash.
    ca(thing);        
    ca(&helper,thing);

```

# Implementation

`Callback<>` is based on th simplest possible representation of a function pointer in C++ which is a union of all the possible function pointer types and member function pointer types.

For instance, a member function pointer in a derived class with virtual inheritance "may" or "may not" be the same size as a member function pointer on a base class.  Using a union and the `sizeof` operator we can construct a universal poniter type.

```cpp
// Im not making any assumptions that the size of a member pointer in a class
// with multiple inheritance is the same as the size of a member pointer in a
// class with virtual inheritance.  Though according to many sources they are.
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

// The types are just the sizes so they can be identified easily, but in order to
// call them we need to atleast differentiate between a function pointer and a 
// member function pointer.  So we offset all of the member function pointers.
enum CallType
{
    TYPE_FUNCTION	= 1,
    TYPE_BASE		= 2,
    TYPE_DERIVED	= 3,
    TYPE_VIRTUAL	= 4,
    TYPE_UNKNOWN	= 5,
    TYPE_FUNCTOR    = 6,
};


// The same signature as a normal static function since we use a helper
typedef int (*CallFunctor)(void *, int);

// The largest type should always be the unknown pointer, so by setting that to
// null in the constructor the entire union should be null.
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

```

In addition to the union we define a this pointer, an args count (unused), and a type.

```cpp
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

```


Prior to C++11, this type required a number of duplicate copies with difference argument counts, but variadic template arguments
greatly simplifies the implementation.

For functors and lambdas we can use the rvalue reference syntax && and SFINAE (to prevent non functor/lambda types from resolving through that interface).  

```cpp
template<typename _Functor_>
Callback(_Functor_ && func, 
    typename Traits::Enable<!Traits::Castable<_Functor_,Call>::Value>::Type ** type=0):
    Call(6,-1,0)
{
    ...
};
```

SFNAE (Substitution Error Is Not a Failure) exploits the fact that failed template resolution is not treated as an error by the compiler, rather it simply removes that declaration if it can't be resolved.  By defining a pointer to a pointer to a type that is optionally defined in an Enable<> template we can disable a certain interface for particular types.

```cpp
template <bool _Bool_, class _Type_ = void>
struct Enable 
{
 	// SFINAE, there is no member "Type" when !_Bool_
};

template <class _Type_>
struct Enable<true, _Type_> 
{
	//using Type = _Type_;
	typedef _Type_ Type;
};
```

Normal function types can fit in the union, but lambda and functor need to allocate a struct (which we can point to using the existing this pointer).

The struct is used to implement type erasure using a templated static function which captures the type a    nd forwarding information for the arguments.

```cpp
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
// Increment the ref count
((Call::Counted*)That)->Increment();

Thunk = &ThunkFunctor;

```

Because we allocate for these two types we also ref count the struct so that copying/assigning callbacks shares resources.

In each constructor we use a similar local `Info {}` struct and memcpy trick to essentially cast from a specific function
pointer type to the generic union, bypassing the C++ type system.

```cpp
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
```


A simple `Sizeofcall` macro tries all combinations of function pointer size to set the correct type, which we then use to specify which `Thunk` to use for the call. 


```cpp
#define Sizeofcall(call) (sizeof(call)<=SizeofBase?2:(sizeof(call)<=SizeofDerived?3:(sizeof(call)<=SizeofVirtual?4:(sizeof(call)<=SizeofUnknown?5:0))))
```

 The `Thunk` avoids having to switch on type during call, by capturing the function pointer cast once on construction.

```cpp
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
```


