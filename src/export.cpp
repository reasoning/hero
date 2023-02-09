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

#include "export.h"

#include "hero/file.h"
#include "hero/folder.h"
#include "hero/parser.h"
#include "hero/string.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CodeParser::ParseCode(const String& string, const String& path)
{
    if (!Handler) return false;

    Assign(string);

    CodeToken comment;

    while (!Eof())
    {
        SkipWhitespace();

        if (Is("//"))
        {
            Mark(comment);
            comment.Offset = Scanner->Token.Offset;
            while (!Eof() && !IsNewline())
            {
                Next();
            }

            Trap(comment);

            if (!comment.StartsWith("////////////////////////////////////////"))
                Handler->HandleComment(comment, path);
        }
        else if (Is("/*"))
        {
            Mark(comment);
            comment.Offset = Scanner->Token.Offset;
            Next();
            while (!Eof(2) && !Is("*/"))
            {
                Next();
            }

            if (!Eof(2) && Skip("*/"))
            {
                Trap(comment);
                Handler->HandleComment(comment, path);
            }
        }
        else if (Is("\""))
        {
            CodeToken literal;
            Mark(literal);
            literal.Offset = Scanner->Token.Offset;

            Next();
            while (!Eof() && !Is("\""))
            {
                if (Is("\\"))
                    Skip(2);
                else
                    Next();
            }
            Next();
            Trap(literal);
        }
        else
        {
            Next();
        }
    }

    return false;
}

void CodeReader::Write()
{
    Iterand<Tokens> files = Files.Forward();
    while (files)
    {
        StringStream code;

        FileStream file(Files.Keys[files.Index]);

        file.Open(File::OPTIONS_OPEN_BINARY_READ);

        file.Seek(0);

        files().Sort(CodeTokenCompare());

        {
            int offset = 0;
            Iterand<CodeToken> tokens = files().Forward();
            while (tokens != 0)
            {
                CodeToken token = tokens();
                if (token.Offset < offset)
                {
                    AssertBreak();
                }

                offset = token.Offset;
                ++tokens;
            }
        }

        int offset = 0;

        Iterand<CodeToken> tokens = files().Forward();
        while (tokens != 0)
        {
            CodeToken token = tokens();
            if (token.Offset - offset < 0)
            {
                AssertBreak();
                ++tokens;
                continue;
            }

            if (token.Offset - offset > 0)
                offset += file.Read((Stream&)code, token.Offset - offset);

            if (!Definitions.Select(tokens()))
            {
                offset = file.Seek(tokens().Size, 0);
            }

            ++tokens;
        }

        file.Read((Stream&)code);

        code.Seek(0);

        Regex regex("(\\n\\s*\\n)+");
        code.Replace(regex, "\n\n");

        file.Close();

        file.Replace("src/", "exp/");
        file.Open(File::OPTIONS_CREATE_BINARY_MUTABLE);
        file.Write((Stream&)code);
        file.Close();

        ++files;
    }
}

void CodeReader::Copyright()
{
    String copyright;
    File("COPYRIGHT").Read(copyright);
    copyright.Prepend("/*\n");
    copyright.Append("\n*/\n");

    Iterand<Tokens> files = Files.Forward();
    while (files)
    {
        StringStream code;
        FileStream file(Files.Keys[files.Index]);
        file.Replace("src/", "exp/");

        file.Read((Stream&)code);
        file.Close();
        file.Open(File::OPTIONS_OPEN_BINARY_MUTABLE);
        file.Write(copyright);
        code.Seek(0);
        file.Write((Stream&)code);
        file.Close();

        ++files;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
