# Any

Any contains a simple policy based any class.

# Types
- Any

# Usage

You can set a type using the consstructor or the assignment operator.

You can get a type using the Cast<> template method ansd specifying the
type.  The Cast method attempts to verify that the types are the same
and will return a null value (zero memory but same type) as the type
requested if this fails.  

The Null<>::Value() that is returned is a reference to static memory
the size of type, so you can check if its null by comparing the pointer
address.

```cpp

Any any(String);
String res = any.Cast<String>();

// Check if the string returned from any is not the null type
if (!Null<String>::Equals(res))
    PrintArgLn(res);

```

Here you can see any used with int ans several string types.

```cpp

// Create any any for int, and then assign it a string literal which has a
// special overload to convert to String.

Any any;
any = 1;
any.Cast<int>();

any = "any";

// The expected return type will be String for literals
String str;
str = any.Cast<String>();
PrintArgLn(str);

// Likewise for char arrays
char buf[256] = {};
any = buf;
str = any.Cast<String>();
PrintArgLn(str);

char x[4] = {'a','b','c','d'};
any = x;
str = any.Cast<String>();
PrintArgLn(str);

// Any can store any type you want, including Fixed which is an infinite 
// precision floating point
any = Fixed("2e-30");
PrintArgLn(any.Cast<Fixed>().Str());

any = 10;
int a = any.Cast<int>();
PrintLn("%d",a);
```

# Implementation

It uses a pointer to the type being stored and a pointer to the current policy.

```cpp
class Any
{
public:

	AnyPolicy * Base;
	void * Kind;

	Any():
		Base(AnyPolicy::Empty()),Kind(0)
	{		
	}

	template <typename _Kind_>
	Any(const _Kind_& kind):
		Base(AnyPolicy::Empty()),Kind(0)
	{
		Assign(kind);
	}

    ...

	template<typename _Kind_>
	Any & Assign(const _Kind_& kind)
	{
		Reset();		
		Base = AnyPolicy::Policy<_Kind_>();
		Base->Set((void*)&kind,&Kind);
		return *this;
    }    
};
```
It is based on a form of type erasure, where the type is stored as (or in) a 
void * but the behavior for that type is captured by one of several policy 
implementations which derive from a common base class interface.

The policy interfaces pass mostly void ** pointers to pointers in order to
delete, copy, move, set and get the type.

The reason for the void** indirection is that some primitive types can be 
stored in the pointer value itself, saving allocation.

```cpp

class AnyPolicy
{
public:

	virtual void Del(void ** data) {}
	virtual void Copy(void ** data, void ** dest) {}
	virtual void Move(void ** data, void ** dest) {}
	virtual void Set(void * data, void ** dest) {}
	virtual void * Get(void ** data) {return 0;}
	
	virtual bool Cast(Types::Info & info) {return false;}
	
	template<typename _Kind_> 
	static AnyPolicy * Policy();
	
	static AnyPolicy * Empty();
};

```

The policy selector is a static template method on the Any class which 
constructs a function level static policy using a templated policy selector.

This is the key to almost all type erasure techniques, using multiple instances
of static template methods to capture behavior.

The policy is chosen based on some default type traits checks for class vs primtive
and template specialisations that overload this for particular cases.

```cpp
template<typename _Kind_>
struct PolicySelector 
{
	typedef typename Traits::If< Traits::IsClass<_Kind_>::Value, AnyPolicyNew<_Kind_>, AnyPolicyPlace<_Kind_> >::Type Type;		
};

template<>
struct PolicySelector<char[]> 
{
	typedef AnyPolicyNew<String> Type;
};

template<typename _Kind_> 
AnyPolicy * AnyPolicy::Policy()
{
	static typename PolicySelector<_Kind_>::Type policy;
	return &policy;
};

```