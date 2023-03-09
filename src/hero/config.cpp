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


#include "hero/config.h"
#include "hero/parser.h"
#include "hero/stream.h"
#include "hero/string.h"
#include "hero/structure.h"
#include "hero/tokenizer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Options::Parse(char * data, int size)
{
	Append(data,size);
	return Parse();
}

bool Options::Parse(int argc, char * argv[])
{
	for (int i=0;i<argc;++i)
		Append(argv[i]);
	return Parse();
}

bool Options::Parse()
{
	StringParser parser(*this);
	while (!parser.Eof())
	{
		parser.SkipWhitespace();
		if (parser.Is("-"))
		{
			parser.Next();
			if (parser.Is("-"))
				parser.Next();

			parser.Mark();
			while (!parser.Eof() && parser.IsAlphanumeric())
				parser.Next();
			parser.Trap();

			if (parser.Token.Size > 0)
			{
				String opt = parser.Token;
				Array<Substring> values;

				parser.SkipWhitespace();
				while (!parser.Eof() && !parser.Is("-"))
				{
					parser.Mark();
					while(!parser.Eof() && !parser.IsWhitespace())
						parser.Next();
					parser.Trap();

					Substring value = parser.Token;
					if (!value.IsEmpty())
						values.Append(value);

					parser.SkipWhitespace();
				}

				if (!Match(opt,values))
				{

					Raise("Options::Parse: Unmatched option %s",opt.Print());
				}

			}
		}
		else
		{
			parser.Next();
		}
	}

	Apply();

	return true;
}

bool Options::Match(const Substring & opt, Array<Substring> & values)
{
	Iterand<Arg*> args = Args.Forward();
	while (args)
	{	
		if (args()->Match(opt))
		{

			Iterand<Substring> value = values.Forward();
			while(value)
			{
				args()->Values.Append(value());		
				++value;
			}

			return true;
		}

		++args;
	}

	return false;
}	

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Args::Construct(int argc, char * argv[])
{
	String arguments;
	for (int i=0;i<argc;++i)
		arguments << argv[i] << " ";

	return Construct(arguments);
}

bool Args::Construct(char * data, int size)
{
	String::Construct(data,size);

	StringParser parser;
	parser.Assign(*this);
	bool error = false;
	while ( !error && !parser.Eof() )
	{
		if (parser.Is('-'))
		{
			parser.Next();
			if (parser.Is('-'))
				parser.Next();

			Segment * path = new Segment();
			Append(path);

			if (parser.ParseWord())
			{
				if (parser.Eof() || parser.SkipWhitespace())
				{						
					Segment * name = new Segment();
					Segment * value = new Segment();
					path->Append(name);
					path->Append(value);
					name->Assign(parser.Token);

					if (!parser.Is('-'))
					{
						parser.Mark();
						while (!parser.Eof() && !parser.IsWhitespace()) parser.Next();
						parser.Trap();

						if (!parser.Token.IsEmpty())
							value->Assign(parser.Token);
					}

				}
				else
				{
					error = true;
				}
			}
			else
			{
				error = true;
			}
		}
		else
		{
			parser.Mark();
			while (!parser.Eof() && !parser.IsWhitespace())
				parser.Next();
			parser.Trap();

			if (!parser.Token.IsEmpty())
			{
				Segment * path = new Segment();
				path->Assign(parser.Token);
				Append(path);
			}
		}

		if ( !error && !parser.Eof() && !parser.SkipWhitespace() && !parser.Is('-'))
			error = true;
	}

	if (error)
	{
		Release(false);
		Raise("Args::Construct: Invalid token in arguments at column %d.",parser.Column());
		return false;
	}
	else
	{
		return true;
	}
}

Segment * Args::Select(char * name, int size, bool caseless)
{
	Path::Enumerator iterator(*this);
	for (iterator.Forward();iterator.Has();iterator.Move())
	{
		if (iterator.Reference().Children() == 2)
		{

			if (iterator.Reference()[0].Is(name,size,caseless))
			{
				return &iterator.Reference()[1];
			}
		}
		else
		{

			if (iterator.Reference().Is(name,size,caseless))
				return &iterator.Reference();
		}
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Configuration::Configuration()
{

}

Configuration::~Configuration()
{
	Sections.Destroy();
}

bool Configuration::Load(char * data, int size)
{
	TextFileStream textFileStream(data,size);
	TokenizerStream streamIterator(textFileStream);
	streamIterator.Separators.Append(new Segment("\r"));
	streamIterator.Separators.Append(new Segment("\n"));

	Hero::Section * section = 0;

	Result<bool,int> res;

	bool continued=false;
	for (streamIterator.Forward();streamIterator.Has();streamIterator.Move())
	{

		StringParser parser;
		parser.Assign(*streamIterator());
		parser.ParseWhitespace();

		if (continued)
		{
			parser.Mark();
			while (!parser.Eof() && !parser.Is("\\")) parser.Next();
			parser.Trap();
			continued = (parser.Is("\\"))?true:false;

			section->Properties.Values[res.Index].Append(parser.Token);
		}
		else
		if (parser.Is("#") || parser.Is("!") || parser.Is(";") || parser.Is("//"))
		{

		}
		else
		if (parser.Is("["))
		{
			parser.Mark();
			while (!parser.Eof() && !parser.Is("]"))
				parser.Next();
			parser.Trap();

			if (parser.Is("]"))
			{
				if (section) Sections.Append(section);
				section = new Hero::Section();

				section->Name = parser.Token;
				parser.Next();
			}
			else
			{
				Raise("Properties::Load: Missing \"]\" after section name, line %d column %d\n",parser.Line(),parser.Column());
			}

		}
		else
		if (!parser.Eof())
		{
			parser.Mark();
			while (!parser.Eof() && !parser.IsWhitespace() && !parser.Is("="))
				parser.Next();
			parser.Trap();

			Substring name(parser.Token);

			if (parser.Is("="))
			{
				parser.Next();
				parser.Mark();
				while (!parser.Eof() && !parser.Is("\\")) parser.Next();
				parser.Trap();
				continued = (parser.Is("\\"))?true:false;

				Substring value(parser.Token);

				if (!section)
				{
					section = new Hero::Section();
				}

				res = section->Properties.Insert(name,value);
			}
			else
			{
				Raise("Configuration::Load: Missing \"=\" after attribute name, line %d column %d\n",parser.Line(),parser.Column());
			}
		}
	}

	if (section)
		Sections.Append(section);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ConfigurationParser::ParseSection(Section & section)
{

	while(!Eof())
	{
		SkipWhitespace();

		if (Is("["))
		{
			Mark();
			while (!Eof() && !Is("]"))
				Next();
			Trap();

			if (Is("]"))
			{
				section.Name = Token;
				Next();
				ParseProperties(section.Properties);
			}
			else
			{
				Raise("ConfigurationParser::Load: ParseSection \"]\" after section name, line %d column %d\n",Line(),Column());
			}
		}
	}

}

void ConfigurationParser::ParseProperties(Properties & properties)
{

	bool continued=false;

	while (!Eof() && !Is("["))
	{
		ParseWhitespace();

		if (continued)
		{
			Mark();
			while (!Eof() && !Is("\\")) Next();
			Trap();
			continued = (Is("\\"))?true:false;

		}
		else
		if (Is("#") || Is("!") || Is(";") || Is("//"))
		{

			SkipLine();
		}
		else
		if (!Eof())
		{
			Mark();
			while (!Eof() && !IsWhitespace() && !Is("="))
				Next();
			Trap();
			SkipWhitespace();

			Substring name(Token);

			if (Is("="))
			{
				Next();
				SkipWhitespace();

				Mark();
				while (!Eof() && !Is("\\") && !Is(Characters::NewLine) && !Is(Characters::CarriageReturn)) Next();
				Trap();
				continued = (Is("\\"))?true:false;

				Substring value(Token);

				if ((value.StartsWith("'") && value.EndsWith("'")) || (value.StartsWith('"') && value.EndsWith('"')))
				{

					value.Trim(1);
				}

				properties.Insert(name,value);
			}
			else
			{
				Raise("ConfigurationParser::ParseProperties: Missing \"=\" after attribute name, line %d column %d\n",Line(),Column());
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ConfigurationFile::Read(Properties & properties)
{

	String string;
	File::Read(string);

	ConfigurationParser configurationParser(string);

	while (!configurationParser.Eof())
		configurationParser.ParseProperties(properties);
}

void ConfigurationFile::Write(Properties & properties)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

