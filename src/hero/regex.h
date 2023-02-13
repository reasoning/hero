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

#include <memory.h>

#include "hero/hero.h"
#include "hero/parser.h"
#include "hero/structure.h"

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

class RegexIdentifier: public Flags<32>
{
public:

	enum IdentifierTypes
	{
		IDENTIFIER_ANY					=(1),		

		IDENTIFIER_BACKSPACE			=(1)<<1,	
		IDENTIFIER_TAB					=(1)<<2,	
		IDENTIFIER_CARRIAGE_RETURN		=(1)<<3,	
		IDENTIFIER_VERTICAL_TAB			=(1)<<4,	
		IDENTIFIER_FORM_FEED			=(1)<<5,	
		IDENTIFIER_NEW_LINE				=(1)<<6,	

		IDENTIFIER_WORD					=(1)<<7,	
		IDENTIFIER_NON_WORD				=(1)<<8,	
		IDENTIFIER_WHITESPACE			=(1)<<9,	
		IDENTIFIER_NON_WHITESPACE		=(1)<<10,	
		IDENTIFIER_DIGIT				=(1)<<11,	
		IDENTIFIER_NON_DIGIT			=(1)<<12,	

		IDENTIFIER_SYMBOL				=(1)<<13,		
	};

	static Flags<32> Classify(char data)
	{
		Flags<32> token;
		token.On(IDENTIFIER_ANY);

		switch (data)
		{
		case Characters::Backspace: token.On(IDENTIFIER_BACKSPACE); break;
		case Characters::Tab: token.On(IDENTIFIER_TAB); break;
		case Characters::CarriageReturn: token.On(IDENTIFIER_CARRIAGE_RETURN); break;
		case Characters::VerticalTab: token.On(IDENTIFIER_VERTICAL_TAB); break;
		case Characters::FormFeed: token.On(IDENTIFIER_FORM_FEED); break;
		case Characters::NewLine: token.On(IDENTIFIER_NEW_LINE); break;
		}						

		if (Characters::IsAlphanumeric(data) || data == '_')
		{
			token.On(IDENTIFIER_WORD);
			token.On(IDENTIFIER_NON_WHITESPACE);
			token.On(Characters::IsNumeric(data)?IDENTIFIER_DIGIT:IDENTIFIER_NON_DIGIT);
		}
		else
		{
			token.On(IDENTIFIER_NON_WORD);
			token.On(IDENTIFIER_NON_DIGIT);
			token.On(Characters::IsWhitespace(data)?IDENTIFIER_WHITESPACE:IDENTIFIER_NON_WHITESPACE);
		}

		return token;
	}

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class RegexRange : public Flags<8>
{
public:

	enum RangeTypes
	{
		RANGE_ALPHA_UPPERCASE		=(1),		
		RANGE_ALPHA_LOWERCASE		=(1)<<1,	
		RANGE_ALPHA					=RANGE_ALPHA_UPPERCASE|RANGE_ALPHA_LOWERCASE,

		RANGE_NUMERIC				=(1)<<2,	
		RANGE_ALPHANUMERIC			=RANGE_NUMERIC|RANGE_ALPHA,
	};

	static Flags<8> Classify(char data)
	{
		Flags<8> range;

		if (Characters::IsNumeric(data))
			range.On(RANGE_NUMERIC);
		else
		if (Characters::IsUppercase(data))
			range.On(RANGE_ALPHA_UPPERCASE);
		else
		if (Characters::IsLowercase(data))
			range.On(RANGE_ALPHA_LOWERCASE);

		return range;
	}

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class RegexQuantifier
{
public:

	enum QuantifierTypes
	{
		QUANTIFIER_ZERO_OR_MORE		=(1),		
		QUANTIFIER_ZERO_OR_ONE		=(1)<<1,	
		QUANTIFIER_ONE_OR_MORE		=(1)<<2,	
		QUANTIFIER_COUNT			=(1)<<3,	
		QUANTIFIER_RANGE			=(1)<<4,	
	};

	enum QuantifierModes
	{
		QUANTIFIER_GREEDY,		
		QUANTIFIER_LAZY,		
		QUANTIFIER_POSSESSIVE,	
	};

	Flags<8> Type;
	Flags<8> Mode;
	int Minimum;
	int Maximum;

	RegexQuantifier(int count):
		Type(QUANTIFIER_COUNT),Mode(QUANTIFIER_GREEDY),Minimum(count),Maximum(count)
	{

	}

	RegexQuantifier(int minimum, int maximum):
		Type(0),Mode(QUANTIFIER_GREEDY),Minimum(minimum),Maximum(maximum)
	{
		Quantify();
	}

private:

	void Quantify()
	{
		if (Maximum==0 && Minimum==0)
		{
			Type = QUANTIFIER_ZERO_OR_MORE;
		}
		else
		if (Maximum==0 && Minimum>=1)
		{
			Type = QUANTIFIER_ONE_OR_MORE;
		}
		else
		if (Maximum==1 && Minimum==0)
		{
			Type = QUANTIFIER_ZERO_OR_ONE;
		}
		else
		if (Minimum == Maximum)
		{
			Type = QUANTIFIER_COUNT;
		}
		else
		{
			Type = QUANTIFIER_RANGE;
		}
	}

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class RegexFunction
{
public:

	enum FunctionTypes
	{
		FUNCTION_SEQUENCE,		
		FUNCTION_GROUP,			
		FUNCTION_CHOICE,		
		FUNCTION_SET,			
		FUNCTION_LITERAL,		
		FUNCTION_IDENTIFIER,	
		FUNCTION_SCOPE,			
		FUNCTION_ANCHOR,		
		FUNCTION_REFERENCE,		

	};

	Flags<8> Type;
	Substring Token;

	RegexFunction(int type):Type(type)
	{
	}

	virtual ~RegexFunction()
	{
	}
};

class RegexFunctionScope : public RegexFunction
{
public:

	RegexFunctionScope():RegexFunction(FUNCTION_SCOPE)
	{
	}

};

class RegexFunctionGroup : public RegexFunction
{
public:

	enum GroupTypes
	{
		GROUP_CAPTURE				=(0),		
		GROUP_PASSIVE				=(1)<<2,	
		GROUP_LOOKAHEAD_EQUALS		=(1)<<3,	
		GROUP_LOOKAHEAD_NOT_EQUALS	=(1)<<4,	
	};

	Flags<8> Group;

	RegexFunctionGroup():
		RegexFunction(FUNCTION_GROUP),Group(GROUP_CAPTURE)
	{
	}

	RegexFunctionGroup(int group):
		RegexFunction(FUNCTION_GROUP),Group(group)
	{
	}

	~RegexFunctionGroup()
	{
	}

};

class RegexFunctionLiteral : public RegexFunction
{
public:

	RegexFunctionLiteral(const Substring &literal): RegexFunction(FUNCTION_LITERAL)
	{
		Token = literal;
	}

	~RegexFunctionLiteral()
	{
	}

	bool Match(char * data, int size)
	{
		return Token.Is(data,size);
	}
};

class RegexFunctionReference : public RegexFunction
{
public:
	int Index;

	RegexFunctionReference(int index):RegexFunction(FUNCTION_REFERENCE),Index(index)
	{
	}
};

class RegexFunctionAnchor : public RegexFunction
{
public:

	enum AchnorTypes
	{
		ANCHOR_START,			
		ANCHOR_END,				
		ANCHOR_STRING_START,	
		ANCHOR_STRING_END,		
		ANCHOR_WORD,			
		ANCHOR_NON_WORD,		
	};

	Flags<8> Anchor;

	RegexFunctionAnchor(int anchor):RegexFunction(FUNCTION_ANCHOR),Anchor(anchor)
	{
	}
};

class RegexFunctionIdentifier : public RegexFunction
{
public:

	RegexIdentifier Identifier;
	String Symbol;

	RegexFunctionIdentifier():RegexFunction(FUNCTION_IDENTIFIER)
	{
	}

	RegexFunctionIdentifier(int type):RegexFunction(FUNCTION_IDENTIFIER)
	{
		Identifier.Assign(type);
	}

	virtual ~RegexFunctionIdentifier()
	{
	}

	bool Match(char data)
	{
		if (Identifier != 0)
		{
			if (Identifier.Is(RegexIdentifier::IDENTIFIER_ANY))
				return true;

			RegexIdentifier token;
			token.Assign(RegexIdentifier::Classify(data));
			if (token.Is(Identifier))
				return true;
		}

		if (Symbol.Is(&data,1))
			return true;

		return false;
	}

};

class RegexFunctionSet : public RegexFunctionIdentifier
{
public:

	enum SetTypes
	{
		SET_INCLUSION		=(1),		
		SET_EXCLUSION		=(1)<<1,	
	};

	Flags<8> Set;
	RegexRange Range;

	RegexFunctionSet():
		Set(SET_INCLUSION)
	{
		Type = FUNCTION_SET;
	}

	RegexFunctionSet(int set):
		Set(set)
	{
		Type = FUNCTION_SET;
	}

	RegexFunctionSet(int set, int range, int token, String & symbol):
		Set(set)
	{
		Range.Assign(range);
		Type = FUNCTION_SET;
		Identifier.Assign(token);
		Symbol = symbol;
	}

	~RegexFunctionSet()
	{
	}

	bool Match(char data)
	{
		if (Symbol.Contains(data))
			return Set.Is(SET_INCLUSION)?true:false;

		if (Range != 0)
		{
			RegexRange range;
			range.Assign(RegexRange::Classify(data));
			if (range != 0 && range.Is(Range))
				return Set.Is(SET_INCLUSION)?true:false;
		}

		if (Identifier != 0)
		{
			RegexIdentifier token;
			token.Assign(RegexIdentifier::Classify(data));
			if (token != 0 && token.Is(Identifier))
				return Set.Is(SET_INCLUSION)?true:false;
		}

		return Set.Is(SET_EXCLUSION)?true:false;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class RegexState
{
public:

	RegexFunction * Function;
	RegexState * Parent;
	Array<RegexState*> Children;
	RegexQuantifier * Quantifier;
	RegexState * Next;

	int Index;

	Substring Match;

	int Base;

	int Offset;

	int Switch;
	int Quantity;
	int Maximum;
	int Minimum;
	bool Lazy;
	bool Possesive;

	RegexState(RegexFunction * function,RegexState * parent=0, RegexQuantifier * quantifier=0):
		Function(function),Parent(parent),Quantifier(quantifier),Next(0),
		Offset(0),Index(0),Quantity(0),Maximum(0),Minimum(0),Switch(0),Base(0),Lazy(false),Possesive(false)
	{

	}

	~RegexState()
	{
		if (Function)
		{
			delete Function;
			Function=0;
		}

		if (Quantifier)
		{
			delete Quantifier;
			Quantifier=0;
		}

		Children.Destroy();

		Parent = 0;
	}

	void Clear()
	{
		Base = 0;
		Index = 0;
		Offset = 0;
		Switch = 0;
		Quantity = 0;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class RegexFrame
{
public:

	enum FrameType
	{
		FRAME_NONE,
		FRAME_ENTER,
		FRAME_LEAVE,
	};

	Flags<8> Frame;

	RegexState * State;
	int Switch;
	int Offset;

	RegexFrame(RegexState * state, int frame=0):State(state),Frame(frame),Switch(0),Offset(0) {}

	static RegexFrame Enter(RegexState * state) {return RegexFrame(state,FRAME_ENTER);}
	static RegexFrame Leave(RegexState * state) {return RegexFrame(state,FRAME_LEAVE);}
	static RegexFrame None(RegexState * state) {return RegexFrame(state,FRAME_NONE);}

};

class RegexScope : public Map<RegexState*,RegexState*>
{
public:

	RegexScope()
	{
	}

	using Map<RegexState*,RegexState*>::operator [];
};

template <typename _Kind_>
class RegexArray : public Array<_Kind_>
{
public:

	RegexArray() 
	{

	}

	~RegexArray()
	{

	}

	_Kind_ Pop()
	{

		this->Size--;
		return this->Data[this->Size];
	}

	_Kind_ Pop(int index)
	{

		return (this->Size > 0 && index >= 0 && index < this->Size)?RemoveAt(this->Size-(1+index)):0;

	}

	_Kind_ Peek(int index = 0)
	{

		return (this->Size>0 && index >= 0 && index < this->Size)?this->Data[this->Size-(1+index)]:0;

	}

	void Push(_Kind_ kind)
	{

		if (this->Size == this->Allocated)
			this->Allocate(0);

		this->Data[this->Size] = kind;
		++this->Size;
	}

	void Push(RegexArray & stack)
	{
		Assert(&stack != this);

		int amount = stack.Length();
		if (amount > 0)
		{
			if ((this->Allocated-this->Size) <= amount)
				this->Allocate(this->Size + (amount+1));

			Assert(this->Allocated-this->Size > amount);

			for (int a = 0;a < amount;++a)
				this->Data[this->Size+a] = stack.Data[a];
			this->Size += amount;

			Assert(this->Size <= this->Allocated);
		}

	}
};

template <typename _Kind_>
class RegexStack
{
public:

	_Kind_ Heap[8];
	_Kind_ * Data;
	int Size;
	int Allocated;

	RegexStack(const RegexStack & stack):Data(Heap),Size(0),Allocated(8)
	{
		Push((RegexStack&)stack);
	}

	RegexStack():Data(Heap),Size(0),Allocated(8)
	{

	}

	~RegexStack()
	{
		if (Data != Heap)
		{
			delete [] Data;
		}

	}

	RegexStack & operator = (const RegexStack & stack)
	{
		Push((RegexStack&)stack);
		return *this;
	}

	int Length() 
	{
		return Size;
	}

	_Kind_ & operator [] (int index)
	{
		Assert(index > -1 && index < Size);
		return Data[index];
	}

	_Kind_ Pop()
	{
		Assert(Size > 0);
		Size--;
		return Data[Size];
	}

	_Kind_ Peek()
	{
		Assert(Size > 0);
		return Data[Size-1];
	}

	void Push(_Kind_ kind)
	{

		if (Size >= Allocated)
		{
			Allocated = ((Size+1)*1.25);
			_Kind_ * array = new _Kind_[Allocated];			
			Copy(array,Data,Size);
			if (Data != Heap)
			{				
				delete [] Data;
			}
			Data = array;
		}

		Data[Size] = kind;
		++Size;
	}

	void Push(RegexStack & stack)
	{
		if (stack.Size == 0) return;

		Assert(&stack != this);

		if (Size+stack.Size >= Allocated)
		{
			Allocated = ((Size+stack.Size+1)*1.25);
			_Kind_ * array = new _Kind_[Allocated];			
			Copy(array,Data,Size);
			if (Data != Heap)
			{				
				delete [] Data;
			}
			Data = array;
		}

		Copy(Data+Size,stack.Data,stack.Size);
		Size += stack.Size;

		Assert(Size <= Allocated);		
	}

	void Swap(RegexStack & stack)
	{
		int size = stack.Size;
		int allocated = stack.Allocated;
		_Kind_ * data = stack.Data;

		if (stack.Heap == stack.Data)
		{
			_Kind_ heap[8];
			Copy(heap,stack.Heap,stack.Size);
			if (Heap == Data)
			{
				Copy(stack.Heap,Heap,Size);

				Copy(Heap,heap,size);

			}
			else
			{
				stack.Data = Data;
				Copy(Heap,heap,size);
				Data = Heap;				
			}
		}
		else
		if (Heap == Data)
		{
			Copy(stack.Heap,Heap,Size);
			stack.Data = stack.Heap;
			Data = data;
		}
		else
		{
			stack.Data = Data;
			Data = data;
		}

		stack.Size = Size;
		Size = size;

		stack.Allocated = Allocated;
		Allocated = allocated;	
	}

	void Copy(_Kind_ * to, _Kind_ * from, int size)
	{

		if (size > 0)
		{
			int loop = (size+7)/8;
			switch(size%8)
			{
				do { 
				case 0:	*to++ = *from++;
				case 7: *to++ = *from++;
				case 6: *to++ = *from++;
				case 5: *to++ = *from++;
				case 4: *to++ = *from++;
				case 3: *to++ = *from++;
				case 2: *to++ = *from++;
				case 1: *to++ = *from++;
				} while(--loop > 0);
			}
		}

	}
};

class RegexConstants
{
public:

	enum RegexOptions
	{
		OPTION_MULTILINE	=(1)<<0,	
		OPTION_INSENSITIVE	=(1)<<1,	
		OPTION_GLOBAL		=(1)<<2,	
	};

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class RegexInterpreter
{
public:

	RegexState * State;
	Array< Array<Substring> > * Matches;
	int Options;

	RegexInterpreter(RegexState * state, Array< Array<Substring> > * matches):
		State(state),Matches(matches),Options(0)
	{

	}

	RegexInterpreter():State(0),Matches(0),Options(0)
	{
	}

	int Match(Substring & sequence) {return Match(1,sequence.Data,sequence.Size);}
	int Match(const char * data) {return Match(1,(char*)data,String::Length(data));}
	int Match(char * data, int size) {return Match(1,data,size);}
	int Match(int limit, Substring & sequence) {return Match(limit,sequence.Data,sequence.Size);}
	int Match(int limit, const char * data) {return Match(limit,(char*)data,String::Length(data));}
	int Match(int limit, char * data, int size)
	{

		if (Matches)
			Matches->Release();

		int base=0;
		int index=0;
		int matches=0;

		int offset=-1;

		RegexStack<RegexState*> next;
		RegexStack<Substring> capture;
		RegexState * state = State;

		Substring input(data,size);

		#define HERO_REGEX_RECURSION

		while (index <= input.Size && (matches < limit || limit == 0))
		{

			capture.Size = 0;
			next.Size = 0;
			offset = -1;
			base = index;

			#ifdef HERO_REGEX_RECURSION

			if (ProcessBacktrack(input,state,next,capture,base,offset) > -1)
			#else

			if ((offset = ProcessBacktrack((Substring&)input,state,base)) > -1)
			#endif
			{
				#ifdef HERO_REGEX_RECURSION
				Substring match(input.Data+base,((offset>base)?offset:base)-base);
				#else
				Substring match(input.Data+base,offset-base);
				#endif
				if (match.Size >= 0)
				{

					index += (match.Size>0)?match.Size:1;

					if (Matches)
					{
						(*Matches).Append(Array<Substring>());
						(*Matches)[matches].Append(match);

						for (int c=0;c<capture.Length();++c)
							(*Matches)[matches].Append(capture[c]);
					}

					++matches;
				}
				else
				{
					++index;
				}
			}
			else
			{
				++index;
			}

		}

		return matches;
	}

	static int Minimum(RegexState * state)
	{
		return state->Quantifier?state->Quantifier->Minimum:1;
	}

	static int Maximum(RegexState * state)
	{
		return state->Quantifier?(state->Quantifier->Maximum?state->Quantifier->Maximum:0x7FFFFFFF):1;
	}

	enum MatchTypes
	{
		MATCH_NONE=0,					
		MATCH_PRECEDING,			
		MATCH_FOLLOWING,			
		MATCH_FOLLOWING_SCOPED,		
	};

	enum ModeTypes
	{
		MODE_NEXT,
		MODE_PREV,
		MODE_THIS,
	};

	void ProcessNext(RegexState * state, RegexStack<RegexState*> & next)
	{

		if (state == 0)
			return;

		if (next.Size > 0)
			state->Next = next.Peek();

		int q = 0;
		RegexState * s = 0;		

		switch (state->Function->Type)
		{
		case RegexFunction::FUNCTION_SCOPE:
			{		
				if (next.Size > 0)
				{
					s = next.Pop();
					ProcessNext(s,next);
					next.Push(s);
				}
			}
			break;
		case RegexFunction::FUNCTION_SEQUENCE:
			{
				int len = next.Size;
				for (q = state->Children.Size-1;q > 0;--q)
					next.Push(state->Children[q]);

				s = state->Children[0];
				ProcessNext(s,next);

				next.Size = len;
			}
			break;
		case RegexFunction::FUNCTION_GROUP:
			{	
				next.Push(state->Children[0]);

				state->Next = next.Peek();

				s = state->Children[1];
				ProcessNext(s,next);
				next.Pop();
			}
			break;
		case RegexFunction::FUNCTION_CHOICE:
			{
				for (q = 0;q < state->Children.Size;++q)
				{
					s = state->Children[q];
					ProcessNext(s,next);
				}
			}
			break;
		case RegexFunction::FUNCTION_SET:
		case RegexFunction::FUNCTION_IDENTIFIER:
		case RegexFunction::FUNCTION_ANCHOR:
		case RegexFunction::FUNCTION_REFERENCE:
		case RegexFunction::FUNCTION_LITERAL:
			{				
				if (next.Size > 0)
				{
					s = next.Pop();
					ProcessNext(s,next);
					next.Push(s);
				}
			}
			break;

		}
	}

	int Process(Substring & input, RegexState * state, RegexStack<RegexState*> & next, RegexStack<Substring> & capture, int & base, int & offset)
	{
		if (state == 0 || base > input.Size)
			return -1;

		int q = 0;				
		int b = base;			
		int o = -1;				
		RegexState * s = 0;		

		switch (state->Function->Type)
		{
		case RegexFunction::FUNCTION_SCOPE:
			{

				state->Parent->Base = base;

				int len = capture.Size;

				if (((RegexFunctionGroup*)state->Parent->Function)->Group != RegexFunctionGroup::GROUP_PASSIVE)

					capture[state->Parent->Offset] = Substring(input.Data+state->Parent->Index,base-state->Parent->Index);

				if (next.Size > 0)

				{
					s = next.Pop();

					o = Process(input,s,next,capture,b,o);
					next.Push(s);
				}
				else
				{
					o = base;
				}

				if (o > offset)
				{
					offset = o;

				}	
				else
				{
					capture.Size = len;
					capture[state->Parent->Offset].Data = 0;
					capture[state->Parent->Offset].Size = 0;
				}

			}
			break;
		case RegexFunction::FUNCTION_SEQUENCE:
			{
				int len = next.Size;
				for (q = state->Children.Size-1;q > 0;--q)
					next.Push(state->Children.Data[q]);

				s = state->Children.Data[0];
				o = Process(input,s,next,capture,b,o);
				if (o > offset)
				{
					offset = o;
				}

				next.Size = len;

			}
			break;
		case RegexFunction::FUNCTION_GROUP:
			{	

				state->Offset = capture.Size;
				if (((RegexFunctionGroup*)state->Function)->Group != RegexFunctionGroup::GROUP_PASSIVE)
					capture.Push(Substring());

				int len = capture.Size;				
				RegexStack<Substring> cap;
				cap.Push(capture);

				next.Push(state->Children.Data[0]);

				int g = base;
				q = state->Minimum;
				do 
				{

					if ((state->Lazy && offset > -1) )
						break;

					state->Index = g;

					if (next.Size > 0)

					{

						cap.Size = len;

						s = (q == 0)?next.Pop():state->Children.Data[1];

						b = g;
						o = -1;
						o = Process(input,s,next,cap,b,o);

						if (q == 0) next.Push(s);						

						if (o >= offset && o > -1)
						{
							offset = o;

							capture.Size = 0;
							capture.Push(cap);

							g = state->Base;
						}	
						else
						if (state->Base > g)
						{

							g = state->Base;
						}
						else
						if (q > 0)
						{
							break;
						}
					}
					else
					{
						offset = base;
					}

				} 
				while (q++ < state->Maximum);

				--q;

				if (q < state->Minimum || q > state->Maximum)
				{
					offset = -1;
				}

				next.Pop();
			}
			break;
		case RegexFunction::FUNCTION_CHOICE:
			{

				int len = capture.Size;
				RegexStack<Substring> cap;
				cap.Push(capture);

				int cb = base;
				int co = -1;

				for (q = 0;q < state->Children.Size;++q)
				{
					s = state->Children.Data[q];
					b = cb;
					o = co;

					cap.Size = len;

					o = Process(input,s,next,cap,b,o);
					if(o > offset)
					{

						offset = o;

						capture.Size = 0;
						capture.Push(cap);

					}
				}
			}
			break;
		case RegexFunction::FUNCTION_SET:
		case RegexFunction::FUNCTION_IDENTIFIER:
		case RegexFunction::FUNCTION_ANCHOR:
		case RegexFunction::FUNCTION_REFERENCE:
		case RegexFunction::FUNCTION_LITERAL:
			{
				int len = capture.Size;
				RegexStack<Substring> cap;
				cap.Push(capture);

				int index = base;

				s = (next.Size>0)?next.Pop():0;

				q = state->Minimum;
				do
				{

					if (state->Lazy && offset > -1)
						break;

					if (q > 0 && !ProcessTerminal(input,state->Function,index,capture))
						break;

					if (s != 0)
					{				

						cap.Size = len;								

						b = index;
						o = -1;
						o = Process(input,s,next,cap,b,o);
						if (o >= offset && o > -1)
						{

							offset = o;
							capture.Size = 0;
							capture.Push(cap);

						}
					}
					else
					{
						offset = index;
					}

				}
				while(q++ < state->Maximum);

				--q;

				if (s != 0) next.Push(s);

				if (q < state->Minimum || q > state->Maximum)
				{
					offset = -1;
				}

			}
			break;

		}

		return offset;
	}

	int ProcessBacktrack(Substring & input, RegexState * state, RegexStack<RegexState*> & next, RegexStack<Substring> & capture, int & base, int & offset)
	{
		if (state == 0 || base > input.Size)
			return -1;

		int q = 0;				
		int b = base;			
		int o = -1;				
		RegexState * s = 0;		

		switch (state->Function->Type)
		{
		case RegexFunction::FUNCTION_SCOPE:
			{
				RegexState * group = state->Parent;
				group->Base = base;

				int index = group->Index;

				int len = capture.Size;
				RegexStack<Substring> cap;
				cap.Push(capture);

				cap.Size = group->Offset;

				++group->Quantity;

				if (!group->Lazy)
				{
					if (group->Quantity < group->Maximum)
					{

						s = group;						
						b = base;
						o = -1;
						o = ProcessBacktrack(input,s,next,cap,b,o);							

						group->Index = index;
						group->Base = base;
					}

				}

				if (o <= offset)
				{

					cap.Size = 0;
					cap.Push(capture);
					if (((RegexFunctionGroup*)group->Function)->Group != RegexFunctionGroup::GROUP_PASSIVE)
						cap[group->Offset] = Substring(input.Data+group->Index,base-group->Index);

					if (next.Length() > 0)
					{
						s = next.Pop();
						b = base;
						o = -1;
						o = ProcessBacktrack(input,s,next,cap,b,o);
						next.Push(s);
					}
					else
					{
						o = base;
					}

					if (o > offset)
					{
						offset = o;
						capture.Size = 0;
						capture.Push(cap);
					}	
				}
				else
				{
					offset = o;
					capture.Size = 0;
					capture.Push(cap);
				}

			}
			break;
		case RegexFunction::FUNCTION_SEQUENCE:
			{

				int len = next.Size;
				for (q = state->Children.Size-1;q > 0;--q)
					next.Push(state->Children.Data[q]);

				s = state->Children.Data[0];
				o = ProcessBacktrack(input,s,next,capture,b,o);
				if (o > offset)
				{
					offset = o;
				}

				next.Size = len;

			}
			break;
		case RegexFunction::FUNCTION_GROUP:
			{

				state->Offset = capture.Size;

				if (((RegexFunctionGroup*)state->Function)->Group != RegexFunctionGroup::GROUP_PASSIVE)
					capture.Push(Substring());

				state->Index = base;

				next.Push(state->Children.Data[0]);
				s = state->Children.Data[1];

				o = ProcessBacktrack(input,s,next,capture,b,o);
				next.Pop();

				if (o <= offset && state->Quantity == 0 && state->Minimum == 0)
				{

					if (next.Length() > 0)
					{
						s = next.Pop();
						o = ProcessBacktrack(input,s,next,capture,b,o);
						next.Push(s);
					}
				}

				if (o >= offset && o > -1)
				{
					offset = o;

				}

			}
			break;
		case RegexFunction::FUNCTION_CHOICE:
			{				
				int len = capture.Size;
				RegexStack<Substring> cap;
				cap.Push(capture);

				int cb = base;
				int co = -1;

				for (q = 0;q < state->Children.Size;++q)
				{
					s = state->Children.Data[q];
					b = cb;
					o = co;

					cap.Size = len;

					o = ProcessBacktrack(input,s,next,cap,b,o);
					if(o > offset)
					{
						offset = o;
						capture.Size = 0;
						capture.Push(cap);

						break;
					}
				}
			}
			break;
		case RegexFunction::FUNCTION_SET:
		case RegexFunction::FUNCTION_IDENTIFIER:
		case RegexFunction::FUNCTION_ANCHOR:
		case RegexFunction::FUNCTION_REFERENCE:
		case RegexFunction::FUNCTION_LITERAL:
			{
				int len = capture.Size;
				RegexStack<Substring> cap;
				cap.Push(capture);

				RegexStack<int> back;

				int index = base;

				if (!state->Lazy)
				{
					for (q = 0;q < state->Maximum;++q)
					{
						back.Push(index);
						if (!ProcessTerminal(input,state->Function,index,capture))
							break;						
					}

					if (q < state->Minimum)
						break;
				}

				s = (next.Length()>0)?next.Pop():0;				
				q = state->Minimum;

				if (back.Size > 0) back.Pop();

				while (true)
				{

					if (s != 0)
					{				
						cap.Size = len;								

						b = index;
						o = -1;
						o = ProcessBacktrack(input,s,next,cap,b,o);
						if (o >= offset && o > -1)
						{
							offset = o;
							capture.Size = 0;
							capture.Push(cap);
							break;
						}
					}
					else
					{
						offset = index;
						break;
					}

					if (state->Lazy)
					{
						if (offset > -1 || q++ >= state->Maximum)
							break;

						if (!ProcessTerminal(input,state->Function,index,capture))
							break;
					}
					else
					{

						if (back.Size == state->Minimum || back.Size == 0)
							break;

						index = back.Pop();
					}
				}

				if (s != 0) next.Push(s);

				if (q < state->Minimum || q > state->Maximum)
				{
					offset = -1;
				}

			}
			break;

		}

		return offset;
	}

	bool ProcessTerminal(Substring & input, RegexFunction * function, int & index, RegexStack<Substring> & capture)
	{
		switch (function->Type)
		{
		case RegexFunction::FUNCTION_REFERENCE:
			{

				if (((RegexFunctionReference*)function)->Index > capture.Length() || ((RegexFunctionReference*)function)->Index <= 0)
					return true;

				Substring ref = capture[((RegexFunctionReference*)function)->Index-1];

				if (ref.IsEmpty())
					return true;

				if (ref.Is(input.Data+index,ref.Size))
				{
					index += ref.Size;
					return true;
				}
			}
			break;
		case RegexFunction::FUNCTION_SET:
			if ((index > -1 && index < input.Size) && ((RegexFunctionSet*)function)->Match(input.Data[index]))
			{
				++index;
				return true;
			}
			break;
		case RegexFunction::FUNCTION_ANCHOR:
			{
				int anchor = ((RegexFunctionAnchor*)function)->Anchor;
				switch (anchor)
				{					
				case RegexFunctionAnchor::ANCHOR_START:
					{

						if (index == 0)
							return true;

						if ((Options&RegexConstants::OPTION_MULTILINE) && index > 0 && Characters::IsNewline(input.Data[index-1]) && !Characters::IsNewline(input.Data[index]))
							return true;
					}
					break;
				case RegexFunctionAnchor::ANCHOR_END:
					{

						if (index == input.Size) 
							return true;

						if ((Options&RegexConstants::OPTION_MULTILINE) && index < input.Size && Characters::IsNewline(input.Data[index]) && !Characters::IsNewline(input.Data[index-1]))
							return true;
					}
					break;
				case RegexFunctionAnchor::ANCHOR_STRING_START:
					{
						if (index == 0)
							return true;
					}
					break;
				case RegexFunctionAnchor::ANCHOR_STRING_END:
					{
						if (index == input.Size) 
							return true;
					}
					break;
				case RegexFunctionAnchor::ANCHOR_WORD:
				case RegexFunctionAnchor::ANCHOR_NON_WORD:
					{

						bool match = false;
						if ((index == 0 && Characters::IsIdentifier(input.Data[index])) || (index == input.Size && Characters::IsIdentifier(input.Data[index-1])) || 
							((index > 0 && index < input.Size) && 
								(Characters::IsIdentifier(input.Data[index]) && !Characters::IsIdentifier(input.Data[index-1])) ||
								(!Characters::IsIdentifier(input.Data[index]) && Characters::IsIdentifier(input.Data[index-1]))))
							match = true;

						return (anchor == RegexFunctionAnchor::ANCHOR_WORD)?match:!match;
					}
					break;
				}
			}
			break;
		case RegexFunction::FUNCTION_IDENTIFIER:
			if ((index > -1 && index < input.Size) && ((RegexFunctionIdentifier*)function)->Match(input.Data[index]))
			{
				++index;
				return true;
			}
			break;
		case RegexFunction::FUNCTION_LITERAL:
			if ((index > -1 && index+((RegexFunctionLiteral*)function)->Token.Size <= input.Size) && 
				((RegexFunctionLiteral*)function)->Token.Is(input.Data+index,((RegexFunctionLiteral*)function)->Token.Size))
			{
				index += ((RegexFunctionLiteral*)function)->Token.Size;
				return true;
			}
			break;
		}

		return false;
	}

	int ProcessStack(Substring & input, RegexState * state, int index = 0)
	{
		Map<RegexState*,RegexState*> scope;

		RegexStack<RegexState*> next;
		RegexStack<RegexState*> prev;
		RegexStack<Substring> capture;

		int mode = MODE_NEXT;

		int base = index;
		int offset = -1;

		RegexState * s = 0;
		RegexState * p = 0;
		RegexState * n = 0;
		RegexState * b = 0;

		next.Push(state);

		while( (mode == MODE_THIS) || (mode == MODE_NEXT && next.Length() > 0) || (mode == MODE_PREV && prev.Length() > 1) )

		{		

			if (mode != MODE_THIS)
			{
				if (mode == MODE_NEXT)
				{

					if (s != 0 && base < s->Base) 
						base = s->Base;

					prev.Push(next.Pop());				
					s = prev.Peek();

					s->Clear();	
					s->Base = base;
				}
				else
				if (mode == MODE_PREV)
				{

					if (s != 0 && offset < s->Offset)
						offset = s->Offset;

					b = scope[s];
					if (b)
					{

						b->Index = s->Base;
					}

					next.Push(prev.Pop());
					s = prev.Peek();
				}

				if (s->Maximum == 0) s->Maximum = Maximum(s);
				if (s->Minimum == 0) s->Minimum = Minimum(s);
			}

			switch (s->Function->Type)
			{
			case RegexFunction::FUNCTION_SEQUENCE:
				if (mode == MODE_NEXT)
				{

					if (s->Children.Size < 2)
						break;

					next.Push(s->Children[1]);
					next.Push(s->Children[0]);
				}
				else
				if (mode == MODE_PREV)
				{
					if (offset > s->Offset)
						s->Offset = offset;

					next.Pop();
					next.Pop();
				}
				break;
			case RegexFunction::FUNCTION_GROUP:
				switch (s->Switch)			
				{
				case 0:

					if (s->Minimum == 0)
					{

						base = s->Base;

						if (next.Length() > 0)
						{
							s->Switch = 1;
							mode = MODE_NEXT;
							break;
				case 1:		
							if (offset > s->Offset)
								s->Offset = offset;
						}
						else
						{
							s->Offset = s->Base;
						}
					}

					base = s->Base;
					mode = MODE_NEXT;
					scope.Insert(next.Peek(),s);
					next.Push(s->Children[0]);

					s->Switch = 2;
					break;
				case 2:

					if (s->Index > s->Base || offset > s->Offset)
					{

						if (offset > s->Offset)
						{
							s->Offset = offset;
						}

						++s->Quantity;					

						if (s->Quantity < s->Maximum)
						{												

							if (s->Index > s->Base)
							{
								s->Base = s->Index;
								base = s->Base;
							}

							mode = MODE_NEXT;
							break;
						}
					}

					next.Pop();
					scope.Remove(next.Peek());

					if (s->Quantity < s->Minimum || s->Quantity > s->Maximum)
					{
						offset = -1;
					}

				}
				break;
			case RegexFunction::FUNCTION_CHOICE:
				switch(s->Switch)
				{	
				case 0:
					if (s->Index < s->Children.Size)
					{
						next.Push(s->Children[s->Index]);
						++s->Index;
						s->Switch = 1;

						base = s->Base;

						mode = MODE_NEXT;
						break;
				case 1:

						if (offset > s->Offset)
							s->Offset = offset;

						next.Pop();
						s->Switch = 0;

						if (s->Index < s->Children.Size)						
						{
							mode = MODE_THIS;
							break;	
						}
					}

					mode = MODE_PREV;
				}
				break;
			case RegexFunction::FUNCTION_SET:
			case RegexFunction::FUNCTION_IDENTIFIER:
			case RegexFunction::FUNCTION_LITERAL:
				switch(s->Switch)
				{
				case 0:
					s->Index = base;
				case 1:
					if (s->Quantity < s->Maximum)
					{
						if (ProcessTerminal(input,s->Function,s->Index,capture))
						{

							s->Switch = 1;
							s->Quantity++;
							base = s->Index;

							if (next.Length() > 0)
							{
								s->Switch = 2;
								mode = MODE_NEXT;
								break;
				case 2:			
								if (offset > s->Offset)
									s->Offset = offset;

								s->Switch = 1;

								if (s->Quantity < s->Maximum)
								{
									mode = MODE_THIS;
									break;								
								}
							}
							else
							{

								offset = base;
								s->Offset = offset;

								if (s->Quantity < s->Maximum)
								{
									mode = MODE_THIS;
									break;								
								}
							}
						}
					}

					if (s->Minimum == 0)
					{

						base = s->Base;

						if (next.Length() > 0)
						{
							s->Switch = 3;
							mode = MODE_NEXT;
							break;

				case 3:		

							if (offset > s->Offset)
								s->Offset = offset;
						}
						else
						{

							offset = s->Base;
						}
					}
					else
					if (s->Quantity < s->Minimum || s->Quantity > s->Maximum)
					{
						offset = -1;
					}

					mode = MODE_PREV;
				}
				break;

			}
		}

		return (s!=0)?s->Offset:-1;
	}

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class RegexParser : public StringParser
{
public:

	RegexState * Parse()
	{
		RegexState * state = ParsePattern();

		if (!Eof())
		{
			Raise("RegexParser::Parser - Failure at column %d.\n",Column());
		}

		return state;
	}

	RegexState *  ParsePattern()
	{
		return ParseChoice();
	}

	RegexState * ParseChoice()
	{
		Substring token;
		Mark(token);
		RegexState * child = ParseSequence();

		if (!Eof() && Is('|'))
		{
			RegexState * parent = new RegexState(new RegexFunction(RegexFunction::FUNCTION_CHOICE));
			parent->Children.Append(child);

			do
			{
				Next();
				if ((child = ParseSequence()) != 0)
				{
					parent->Children.Append(child);
				}
				else
				{
					break;
				}
			}
			while (Is('|'));

			Trap(token);
			parent->Function->Token = token;
			return parent;
		}

		return child;
	}

	RegexState * ParseSequence()
	{
		Substring token;
		Mark(token);

		RegexState * child = ParseGroup();

		if (!Eof() && !Is('|') && !Is(')') && child != 0)
		{

			RegexState * parent = new RegexState(new RegexFunction(RegexFunction::FUNCTION_SEQUENCE));
			parent->Children.Append(child);

			do
			{
				child = ParseGroup();
				if (child)
				{
					parent->Children.Append(child);
				}
			}
			while (!Eof() && !Is('|') && !Is(')') && child != 0);

			Trap(token);
			parent->Function->Token = token;
			return parent;
		}

		return child;
	}

	RegexState * ParseGroup()
	{
		RegexState * state = ParseSet();
		if (state) return state;

		if (!Eof() && Is('('))
		{
			Substring token;
			Mark(token);
			Next();

			RegexFunctionGroup * group=0;

			if (Is('?'))
			{
				Next();
				switch(*At())
				{
				case '=': group = new RegexFunctionGroup(RegexFunctionGroup::GROUP_LOOKAHEAD_EQUALS); break;
				case '!': group = new RegexFunctionGroup(RegexFunctionGroup::GROUP_LOOKAHEAD_NOT_EQUALS); break;
				case ':': group = new RegexFunctionGroup(RegexFunctionGroup::GROUP_PASSIVE); break;
				default : group = new RegexFunctionGroup();
				}
			}
			else
			{
				group = new RegexFunctionGroup();
			}				

			state = new RegexState(group);

			state->Children.Append(new RegexState(new RegexFunctionScope()));
			state->Children[0]->Parent = state;
			state->Children.Append(ParsePattern());

			if (Is(')'))
			{
				Next();
				Trap(token);
				group->Token = token;
				ParseQuantifier(state);
			}
			else
			{
				Raise("RegexParser::ParseParenthesis - Expected closing \")\".\n");
			}
		}

		return state;
	}

	RegexState * ParseSet()
	{
		RegexState * state = ParseLiteral();
		if (state) return state;

		if (!Eof() && Is('['))
		{
			Substring token;
			Mark(token);
			Next();

			RegexFunctionSet * set;

			if (Is('^'))
			{
				Next();
				set = new RegexFunctionSet(RegexFunctionSet::SET_EXCLUSION);
			}
			else
			{
				set = new RegexFunctionSet(RegexFunctionSet::SET_INCLUSION);
			}

			state = new RegexState(set);

			while (! (Eof() || Is(']')))
			{
				if (Is("a-z"))
				{
					Skip(3);
					set->Range.On(RegexRange::RANGE_ALPHA_LOWERCASE);
				}
				else
				if (Is("A-Z"))
				{
					Skip(3);
					set->Range.On(RegexRange::RANGE_ALPHA_UPPERCASE);
				}
				else
				if (Is("0-9"))
				{
					Skip(3);
					set->Range.On(RegexRange::RANGE_NUMERIC);
				}
				else
				if (Is('\\'))
				{
					int type;
					ParseIdentifier(type);
					set->Identifier.On(type);

					if (type == RegexIdentifier::IDENTIFIER_SYMBOL)
					{
						set->Symbol << Token;
					}
				}
				else
				{
					set->Symbol << *At();
					Accept();
				}
			}

			if (Is(']'))
			{
				Next();
				Trap(token);
				set->Token = token;
				ParseQuantifier(state);
			}
			else
			{
				Raise("RegexParser::ParseRange - Expected closing \"]\".\n");
			}
		}

		return state;
	}

	bool IsLiteral()
	{
		return (*Scanner)() && IsLiteral(*At());
	}

	bool IsLiteral(char data)
	{
		switch((int)data)
		{
			case '.':case '*':case '?':case '+':
			case '(':case ')':
			case '{':case '}':
			case '[':case ']':
			case '^':case '$':case '|':case '\\':
			return false;
			default:
			return true;
		}
	}

	RegexState * ParseLiteral()
	{
		RegexState * state = ParseAny();
		if (state) return state;

		if ( !Eof() && IsLiteral())
		{
			Mark();
			int literals=0;
			do
			{

				if (literals > 0 && !Eof(1) && IsQuantifier(*At(1,1)))
					break;

				Next();
				++literals;
			}
			while ( !Eof() && IsLiteral());

			Trap();
			state = new RegexState(new RegexFunctionLiteral(Token));
			ParseQuantifier(state);
		}

		return state;
	}

	RegexState * ParseAny()
	{
		RegexState * state = ParseTerminal();
		if (state) return state;

		if (Is('.'))
		{
			Mark();
			Next();
			Trap();
			state = new RegexState(new RegexFunctionIdentifier(RegexIdentifier::IDENTIFIER_ANY));
			state->Function->Token = Token;
			ParseQuantifier(state);
		}

		return state;
	}

	bool ParseIdentifier(int & type)
	{
		if (Is('\\'))
		{

			Next();
			Mark();
			Next();
			Trap();

			switch((int)*Token.Data)
			{
				case 'w': type = RegexIdentifier::IDENTIFIER_WORD;break;
				case 'W': type = RegexIdentifier::IDENTIFIER_NON_WORD;break;
				case 's': type = RegexIdentifier::IDENTIFIER_WHITESPACE;break;
				case 'S': type = RegexIdentifier::IDENTIFIER_NON_WHITESPACE;break;
				case 'd': type = RegexIdentifier::IDENTIFIER_DIGIT;break;
				case 'D': type = RegexIdentifier::IDENTIFIER_NON_DIGIT;break;
				case 'b': type = RegexIdentifier::IDENTIFIER_BACKSPACE;break;
				case 't': type = RegexIdentifier::IDENTIFIER_TAB;break;
				case 'r': type = RegexIdentifier::IDENTIFIER_CARRIAGE_RETURN;break;
				case 'v': type = RegexIdentifier::IDENTIFIER_VERTICAL_TAB;break;
				case 'f': type = RegexIdentifier::IDENTIFIER_FORM_FEED;break;
				case 'n': type = RegexIdentifier::IDENTIFIER_NEW_LINE;break;
				default:  type = RegexIdentifier::IDENTIFIER_SYMBOL;
			}

			return true;
		}

		return false;
	}

	RegexState * ParseTerminal()
	{
		Substring token;
		Mark(token);

		RegexState * state=0;
		int type;

		if (Is('\\'))
		{
			if (IsDecimal(1))
			{
				Next();
				ParseDecimal();

				if (Characters::IsDecimal(Token.Data,Token.Size))
				{
					state = new RegexState(new RegexFunctionReference(Token.Decimal()));
					ParseQuantifier(state);
				}				
			}
			else
			if (IsAny(1,"AZ"))
			{
				Next();
				Mark();Next();Trap();
				switch(*Token.Data)
				{
				case 'A': state = new RegexState(new RegexFunctionAnchor(RegexFunctionAnchor::ANCHOR_STRING_START)); break;
				case 'Z': state = new RegexState(new RegexFunctionAnchor(RegexFunctionAnchor::ANCHOR_STRING_END)); break;
				}

				ParseQuantifier(state);

			}
			else
			if (IsAny(1,"bB"))
			{
				Next();
				Mark();Next();Trap();
				switch(*Token.Data)
				{
				case 'b': state = new RegexState(new RegexFunctionAnchor(RegexFunctionAnchor::ANCHOR_WORD)); break;
				case 'B': state = new RegexState(new RegexFunctionAnchor(RegexFunctionAnchor::ANCHOR_NON_WORD)); break;
				}
				ParseQuantifier(state);
			}
			else
			{
				ParseIdentifier(type);
				state = new RegexState(new RegexFunctionIdentifier(type));
				if (type == RegexIdentifier::IDENTIFIER_SYMBOL)
				{
					((RegexFunctionIdentifier*)state->Function)->Symbol << Token;
				}
				ParseQuantifier(state);
			}
		}
		else

		if (Is('^'))
		{

			Mark();Next();Trap();
			state = new RegexState(new RegexFunctionAnchor(RegexFunctionAnchor::ANCHOR_START));
			ParseQuantifier(state);
		}
		else

		if (Is('$'))
		{
			Mark();Next();Trap();
			state = new RegexState(new RegexFunctionAnchor(RegexFunctionAnchor::ANCHOR_END));
			ParseQuantifier(state);
		}

		Trap(token);
		if (state) state->Function->Token = token;

		return state;
	}

	bool IsQuantifier()
	{
		return (*Scanner)() && IsQuantifier(*At());
	}

	bool IsQuantifier(char data)
	{
		return data == '*' || data == '+' || data == '?' || data == '{';
	}

	bool ParseQuantifier(RegexState * state)
	{

		RegexQuantifier * quantifier = 0;
		if (Is('*'))
		{
			Next();
			quantifier = new RegexQuantifier(0,0);
		}
		else
		if (Is('+'))
		{
			Next();
			quantifier = new RegexQuantifier(1,0);
		}
		else
		if (Is('?'))
		{
			Next();
			quantifier = new RegexQuantifier(0,1);
		}
		else
		if (Is('{'))
		{
			Substring minimum;
			Substring maximum;

			Next();
			Mark(minimum);

			while (IsNumeric())
			{
				Next();

				if (Is(','))
				{
					Trap(minimum);
					Next();
					Mark(maximum);
				}
			}

			if (Is('}'))
			{
				if (minimum.IsEmpty())
					Trap(minimum);
				else
					Trap(maximum);

				Next();
				quantifier = new RegexQuantifier(minimum.Integer(),maximum.Integer());
			}
			else
			{
				Raise("RegexParser::ParseQuantifier - Expected closing \"}\".\n");
			}
		}

		if (quantifier)
		{
			if (Is('?'))
			{
				Next();
				quantifier->Mode = RegexQuantifier::QUANTIFIER_LAZY;
			}
			else
			if (Is('+'))
			{
				Next();
				quantifier->Mode = RegexQuantifier::QUANTIFIER_POSSESSIVE;
			}

			state->Quantifier = quantifier;

		}

		state->Maximum = RegexInterpreter::Maximum(state);
		state->Minimum = RegexInterpreter::Minimum(state);

		state->Lazy = (state->Quantifier)?state->Quantifier->Mode == RegexQuantifier::QUANTIFIER_LAZY:false;

		return true;
	}

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Pattern : public String
{
public:

	int Options;
	Array< Array<Substring> > Matches;

	explicit Pattern(const Substring & sequence):String(sequence),Options(0)
	{
	}

	Pattern():Options(0)
	{
	}

	int Match(Substring & sequence) {return Match(1,sequence.Data,sequence.Size);}
	int Match(const char * data) {return Match(1,(char*)data,String::Length(data));}
	int Match(char * data, int size) {return Match(1,data,size);}
	int Match(int limit, Substring & sequence) {return Match(limit,sequence.Data,sequence.Size);}
	int Match(int limit, const char * data) {return Match(limit,(char*)data,String::Length(data));}
	virtual int Match(int limit, char * data, int size) = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef HERO_USING_DEELX

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Regex : public Pattern, public RegexConstants
{
public:

	void * State;

	Regex(const Regex & regex):Pattern(regex),State(0) {Construct();}
	Regex(const Substring & sequence):State(0) {Construct(sequence);}
	Regex(const char * data):State(0) {Construct((char*)data,String::Length(data));}
	Regex(char * data, int size):State(0) {Construct(data,size);}

	Regex():State(0) 
	{

	}

	~Regex()
	{

	}

	Regex & Construct(const Substring & sequence) {return Construct(sequence.Data,sequence.Size);}
	Regex & Construct(const char * data) {return Construct((char*)data,String::Length(data));}
	Regex & Construct(char * data, int size)
	{
		String::Construct(data,size);
		return Construct();
	}

	Regex & Construct();

	using Pattern::Match;
	int Match(int limit, char * data, int size);

	Regex & operator = (const Regex & regex) {Construct(regex);return *this;}
	Regex & operator = (const Superstring & superstring) {Construct(superstring);return *this;}
	Regex & operator = (const String & data) {Construct(data.Data,data.Size);return *this;}
	Regex & operator = (const Substring & data) {Construct(data.Data,data.Size);return *this;}
	Regex & operator = (const char * data) {Construct(data);return *this;}

	static String & Escape(String & string)
	{

		string.Replace("[","\\[");
		string.Replace("]","\\]");
		string.Replace("(","\\(");
		string.Replace(")","\\)");
		string.Replace(".","\\.");
		string.Replace("*","\\*");
		string.Replace("+","\\+");
		string.Replace("?","\\?");
		string.Replace("^","\\^");
		string.Replace("$","\\$");
		return string;
	}

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#else

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Regex : public Pattern, public RegexConstants
{
public:

	RegexInterpreter Interpreter;
	RegexState * State;

	Regex(const Regex & regex):Pattern(regex),State(0) {Construct();}
	Regex(const Substring & sequence):State(0) {Construct(sequence);}
	Regex(const char * data):State(0) {Construct((char*)data,String::Length(data));}
	Regex(char * data, int size):State(0) {Construct(data,size);}

	Regex():State(0) 
	{

	}

	~Regex()
	{
		if (State)
			delete State;
	}

	Regex & Construct(const Substring & sequence) {return Construct(sequence.Data,sequence.Size);}
	Regex & Construct(const char * data) {return Construct((char*)data,String::Length(data));}
	Regex & Construct(char * data, int size)
	{
		String::Construct(data,size);
		return Construct();
	}

	Regex & Construct()
	{
		if (State)
			delete State;

		RegexParser parser;
		parser.Assign(*this);
		State = parser.Parse();

		Interpreter.State = State;
		Interpreter.Matches = &Matches;
		Interpreter.Options = Options;

		return *this;
	}

	using Pattern::Match;
	int Match(int limit, char * data, int size)
	{

		Interpreter.Options = Options;		
		int matches = Interpreter.Match(limit,data,size);

		return matches;
	}

	Regex & operator = (const Regex & regex) {Construct(regex);return *this;}
	Regex & operator = (const Superstring & superstring) {Construct(superstring);return *this;}
	Regex & operator = (const String & data) {Construct(data.Data,data.Size);return *this;}
	Regex & operator = (const Substring & data) {Construct(data.Data,data.Size);return *this;}
	Regex & operator = (const char * data) {Construct(data);return *this;}

	static String & Escape(String & string)
	{

		string.Replace("[","\\[");
		string.Replace("]","\\]");
		string.Replace("(","\\(");
		string.Replace(")","\\)");
		string.Replace(".","\\.");
		string.Replace("*","\\*");
		string.Replace("+","\\+");
		string.Replace("?","\\?");
		string.Replace("^","\\^");
		string.Replace("$","\\$");
		return string;
	}

};

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

