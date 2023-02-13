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
#include "hero/thread.h"
#include "hero/callback.h"
#include "hero/parser.h"
#include "hero/file.h"
#include "hero/folder.h"
#include "hero/regex.h"
#include "hero/structure.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CodeHandler
{
public:

	virtual void HandleDefinition(const Token & token, const String & path, const String & name) {};
	virtual void HandleReference(const Token & token, const String & path, const String & name) {};
	virtual void HandleComment(const Token & token, const String & path) {};
	virtual void HandleWhitespace(const Token & token, const String & path) {};

};

typedef Token CodeToken;

class CodeTokenCompare
{
public:

	CodeToken * Token;

	CodeTokenCompare():Token(0) {}
	CodeTokenCompare(const CodeToken & token):Token(&(CodeToken&)token) {}
	CodeTokenCompare(CodeToken * token):Token(token) {}

	int operator() (const CodeToken & right)	
	{
		return operator()(*Token,right);
	}

	int operator () (const CodeToken & left, const CodeToken & right)
	{

		return left.Offset-right.Offset;
	}
};

class CodeParser : public StringParser
{
public:

	CodeHandler * Handler;

	CodeParser(CodeHandler * handler):Handler(handler)
	{

	}

	bool ParseCode(const String & string, const String & path);

};

class CodeReader : public CodeParser, public CodeHandler
{
public:

	Map<String, int> References;
	Map<CodeToken, String> Definitions;

	typedef Array<CodeToken> Tokens;

	Map<String, Tokens > Files;

	CodeReader():CodeParser(this)
	{

	}

	void HandleToken(const CodeToken & token, const String & path)
	{

		Tokens & tok = Files.At(Files.Update(path).Index);

		int offset=0;

		Iterand<CodeToken> tokens = tok.Forward();
		while (tokens != 0)
		{

			CodeToken token = tokens();
			if (token.Offset-offset < 0)
			{
				AssertBreak();
			}

			++tokens;
		}

		tok.Insert(token,tok.Index(CodeTokenCompare(token)));	
	}

	void HandleDefinition(const CodeToken & token, const String & path, const String & name)
	{
		HandleToken(token,path);

		if (!Definitions.Insert(token,name))
		{

		}
	}

	void HandleComment(const CodeToken & token, const String & path)
	{	
		HandleToken(token,path);
	}	

	void HandleReference(const CodeToken & token, const String & path, const String & name)
	{
		HandleToken(token,path);
	}

	void HandleWhitespace(const CodeToken & token, const String & path)
	{
		HandleToken(token,path);
	}

	void Read()
	{		
		ReadFolder("src/");
	}

	void ReadFolder(const String & path)
	{
		Folder folder(path);
		folder.List();

		Iterand<Folder> folders = folder.Folders.Forward();
		while (folders != 0)
		{
			if (!folders().EndsWith(".svn"))
			{
				ReadFolder(folders());
			}

			++folders;
		}

		Iterand<File> files = folder.Files.Forward();
		while(files != 0)
		{
			if ((!files().Name.StartsWith(".") && !files().Name.StartsWith("_")) &&
				(files().EndsWith(".cpp") || files().EndsWith(".h")))
			{
				ReadFile(files());
			}

			++files;
		}
	}

	void ReadFile(const String & path)
	{

		Files.Update(path);

		String code;

		File file(path);
		file.Read(code);
		file.Close();

		CodeParser::ParseCode(code,path);
	}

	void Write();
	void Copyright();

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

