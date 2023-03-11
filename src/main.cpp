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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "main.h"
#include "export.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "hero/hero.h"
#include "hero/callback.h"
#include "hero/error.h"
#include "hero/timer.h"
#include "hero/assert.h"
#include "hero/structure.h"
#include "hero/number.h"
#include "hero/fixed.h"
#include "hero/config.h"
#include "hero/any.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Hero;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <assert.h>
#include <limits.h>

#ifdef HERO_USING_STD
#include <map>
#include <unordered_map>
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int MaxGap(Array<int> nums)
{

    nums.Sort([](int i,int j){return Number::Compare(i,j);});

    int m = 0;
    for (int n=0,i=0,j=1;j < nums.Size;++n)
    {        

        long long diff = (long long)nums[i] - (long long)nums[j];

        nums[n] = Number::Clamp(diff);
        nums[n] = Number::Abs(nums[n]);        

        nums[n] = Number::Clamp(Number::Abs(diff));

        i = j+1; j = i+1;
        if (nums[n] > m) m = nums[n];
    }

    return m;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Printf(char * buf, int size, const char * format, ...)
{
	va_list args;
	va_start(args,format);
	int res = vsnprintf(buf,size,"%ll",args);
	va_end(args);	
    return res;
}

class YellowBrickRoad: public Path
{
public:

    YellowBrickRoad()
    {
        Append(&Brick);
        Append(&Road);
    }

    Path Brick;
    Path Road;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char * argv[])
{   

    Array<int> array = {0,1,2,3};
    int found = array.Find([](int x) {return x > 2;});
    PrintLn("%d",found);

    if (false)
    {    	 
        Timer timer;

        CodeReader reader;
        reader.Read();
        reader.Write();
        reader.Copyright();

        PrintArg("Elapsed:",timer.ElapsedSeconds(),"\n");

        exit(0);
    }

    if (false)
    {
        int v1a=0;
        int v1b=0;
        long long v2=0;
        double v3=0.0;        

        Options options;

        // Bind arg1 to opt a1, A1 val v1a and v1b
        
        options.Option("arg1","a1","A1").Bind(v1a,0).Bind(v1b,1);

        options.Option("arg2","a2").Bind(v2);
        options.Option("arg3","a3").Bind(v3);

        Array<bool> v4;
        options.Option("arg4","a4").Bind(v4,0).Bind(v4,1);
        

        //Map<String,int> v5;
        Multimap<String,int> v5;
        options.Option("arg5","a5").Bind(v5,0).Bind(v5,1).Bind(v5,2);

        String v6;
        options.Option("arg6","a6").Bind(v6);

        options.Parse("--a1 1 -a2 -A1 2 -a3 0.2 --a4 -a4 true -a5 1 -a5 3 -a5 2 -a6");
        //options.Parse("--a4 -a4 true");

        PrintArgLn(v1a);
        PrintArgLn(v1b);
        PrintArgLn(v2);
        PrintArgLn(v3);
        for (bool v : v4) PrintArg(v, " ");
        PrintLn("");
        for (int v : v5) PrintArg(v, " ");
        PrintLn("");
        PrintArgLn(v6);

        exit(0);
    }

    if (false)
    {
        Any any;
        any = 1;
        any.Cast<int>();

        any = "any";

        String str;

        str = any.Cast<String>();
        PrintArgLn(str);

	    char buf[256] = {};
        any = buf;
        str = any.Cast<String>();
        PrintArgLn(str);

	    char x[4] = {'a','b','c','d'};
        any = x;
        str = any.Cast<String>();
        PrintArgLn(str);

        any = Fixed("2e-30");
        PrintArgLn(any.Cast<Fixed>().Str());

        any = 10;
        int a = any.Cast<int>();
        PrintLn("%d",a);
    }

    if (false)
    {
        PrintLn("Range:");

        {
            Iterator<int> it = array.Iterate(0,3);
            for(it.Forward();it.Has();it.Move())
            {
                PrintLn("%d",it());
            }

        }

        {
            Iterator<int> it = array.Iterate();
            for(it.Forward();it.Has();it.Move())
            {
                PrintLn("%d",it());
            }
        }

        {
            Iterand<int> it = array.Forward();
            while(it)
            {
                PrintLn("%d",it());
                ++it;
            }
        }

    }

    if (false)
    {

		struct Triple
		{
			int X;
			int Y;
			int Z;

			Triple():X(0),Y(0),Z(0) 
			{
				Next(0,0);
			}

			Triple & Next(int index, void * context)
			{

				bool resume = true;

				for (; ; ++Z) {
					if (!resume) {
						X = 1;
					}
					for (; X <= Z; ++X) {
						if (!resume) {
							Y = X;
						}
						for (; Y <= Z; ++Y) {
							if (resume) {
								++Y;
							}
							if (X*X + Y*Y == Z*Z) {
								return *this;
							}
							resume = false;
						}
					}
				}	

				return *this;		
			}
		};

		Triple triple;

		Generator<Triple> generator;

		generator.Callback = Callback<Triple&,int,void*>(&triple,&Triple::Next);

		Iterand<Triple> triples = generator.Forward();

		for (int i=0;i<10;++i)
		{		
			PrintLn("%s, %s, %s\n",triples().X,triples().Y,triples().Z);
		}

    }

    if (false)
    {

        Args args("cmdline test/numbers.txt");
        if (args.Length() < 2 || !File::Exists(args[1]))
            return 0;

        FileStream stream(args[1]);
        StreamParser parser(stream);
        while (!parser.Eof())
        {
            if (parser.ParseInteger())
                PrintLn("%s",parser.Token.Print());
            else
                parser.SkipLine();
        }    
    }    

    if (false)
    {

        Args args("--file test/numbers.txt");

        if (args.Segments().Count > 0 && File::Exists(args[0][1]))
        {
            File file(args[0][1]);

            Path lines;
            file.Read(lines);
            lines.Split("\n");
            for (Segment line : lines.Segments())
                PrintLn("%d",line.Integer());

        }

        exit(0);
    }

    if (true)
    {

        Args args("--file test/numbers.txt");

        if (args.Segments().Count > 0 && File::Exists(args[0][1]))
        {
            File file(args[0][1]);

            Path lines;
            file.Read(lines);
            lines.Split("\n");

            long long value;
            for (Segment& line : lines.Segments())                   
                if (line.Integer(value))
                    line.Replace(String::Formatted("%d",value*2));

            File("test/numbers.two").Write(lines);
        }

        exit(0);
    }

    if (false)
    {

        Args args("--file test/numbers.txt");

        if (args.Segments().Count > 0 && File::Exists(args[0][1]))
        {
            File file(args[0][1]);

            Path lines;
            file.Read(lines);
            lines.Split("\n");
            for (Segment& line : lines.Segments())
            {
                long long num = Verify(line.Integer());
                if (Verified())
                    line.Replace(String::Formatted("%d",num*2));
            }

            File("test/numbers.two").Write(lines);
        }

        exit(0);
    }

	if (false)
	{
        Path path = "path";
        PrintLn("Path: %s",path.Print());

		PrintLn("Oz!!");

        YellowBrickRoad oz;
        oz.Brick.Replace("Yellow.Brick");
        oz.Brick.Split(".");
        oz.Road.Replace("Road");

        PrintLn("Wizard: %s",oz.Print());

        PrintLn("Color: %s",oz.Brick[0].Print());
        PrintLn("Type: %s",oz.Brick[1].Print());
        PrintLn("What: %s",oz.Road.Print());

		PrintLn("Wizard: %s",oz.Print());

    }

    if (false)
    {

        int min = -2147483648;
        int max = 2147483647;
        Assert(min == INT_MIN);
        Assert(max == INT_MAX);

        int absmin = Number::Abs(min);
        int absmax = Number::Abs(max);
        PrintLn("%d",absmin);
        PrintLn("%d",absmax);

        Array<int> array = {1,3,4,6,10,100,45,99,(int)0x80000000,(int)0x7FFFFFFF};    

        int n = MaxGap(array);
        Assert(n == max);

        PrintLn("%d",n);

        exit(0);
    }

    if (false)
    {

        PrintLn("WOT");

        int min = -2147483648;
        int max = 2147483647;
        Assert(min == INT_MIN);
        Assert(max == INT_MAX);

        int absmin = Number::Abs(min);
        int absmax = Number::Abs(max);

        PrintLn("%d",absmin);
        PrintLn("%d",absmax);

        Array<int> array;

        array.Append(0x80000000);
        array.Append(0x7FFFFFFF);
        array.Append(0x7FFFFFFF);
        array.Append(0x80000000);

        int n = MaxGap(array);
        Assert(n == max);

        PrintLn("%d",n);

        exit(0);
    }

    if (false)
    {

        Folder folder("bak/");
		folder.List();
    }

    #ifdef HERO_USING_STD
    if (false)
    {

        int limit = 1000000;

		{

			Print("\n");
            Print("std::map<>\n");

			Timer timer;

            {
			    std::map<int,String> map;

			    for(int i=0;i<limit;++i)
			    {

				    map[i] = String(i);

			    }

                PrintArg("Split:",timer.ElapsedSeconds(),"\n");
            }

			PrintArg("Elapsed:",timer.ElapsedSeconds(),"\n");
            Print("\n");

		}

        {
            Print("\n");
            Print("Hero::ArrayMap<>\n");

            Timer timer;

            {
                ArrayMap<int,String> map;
                map.Keys.Reserve(limit);
                map.Values.Reserve(limit);
                map.Idx.Reserve(limit);

                for (int i=0;i<limit;++i)
                {

                    map[i] = String(i);
                }

                PrintArg("Split:",timer.ElapsedSeconds(),"\n");
            }

            PrintArg("Elapsed:",timer.ElapsedSeconds(),"\n");
            Print("\n");
        }

		{
			Print("\n");
			Print("std::unordered_map<>\n");

			Timer timer;

            {
                std::unordered_map<int,String> map;

			    for(int i=0;i<limit;++i)
			    {

				    map[i] = String(i);
			    }

                PrintArg("Split:",timer.ElapsedSeconds(),"\n");
            }

			PrintArg("Elapsed:",timer.ElapsedSeconds(),"\n");
            Print("\n");

		}

        {

            Print("\n");
            Print("Hero::HashMap<>\n");

            Timer timer;

            {
                HashMap<int,String> map;

                for (int i=0;i<limit;++i)
                {

                    map[i] = String(i);
                    Assert(map[i] == String(i));
                }

                PrintArg("Split:",timer.ElapsedSeconds(),"\n");
            }

            PrintArg("Elapsed:",timer.ElapsedSeconds(),"\n");
            Print("\n");
        }

        exit(0);
    }

    #endif    

}