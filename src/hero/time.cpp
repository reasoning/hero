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



#include "hero/time.h"
#include "hero/number.h"
#include "hero/format.h"
#include "hero/parser.h"
#include "hero/string.h"

#include <memory.h>




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Local::Construct(long long seconds)
{
	
	
	
	

	
	
	

	long long minutes = seconds/60;
	seconds = seconds%60;

	long long hours = minutes/60;
	minutes = minutes%60;

	long long days = hours/24;
	hours = hours%24;

	
	
	int offset = 0;

	if (days < 0)
	{
		int year = 1970-1;
		offset = Time::DaysInYear(year);
		while (days < -offset)
		{
			days += offset;
			year--;
			offset = Time::DaysInYear(year);
		}

		int month = 12;
		offset = Time::DaysInMonth(month,year);
		while (days < -offset)
		{
			days += offset;
			month--;
			offset = Time::DaysInMonth(month,year);
		}

		Year = year;
		Month = month;
		
		

		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
	
		
		if (hours + minutes + seconds == 0)
		{
			
			Day = offset+(days+1);
			Hour = Minute = Second = 0;
		}
		else
		{
			Day = offset+days;
			Hour = 24+hours;
			if (minutes+seconds != 0)
				--Hour;
			Minute = 60+minutes;
			if (seconds != 0)
				--Minute;
			Second = 60+seconds;		
		}

	}
	else
	{
		int year = 1970;
		offset = Time::DaysInYear(year);
		while (days > offset)
		{
			days -= offset;
			year++;
			offset = Time::DaysInYear(year);
		}

		int month = 1;
		offset = Time::DaysInMonth(month,year);
		while (days > offset)
		{
			days -= offset;
			month++;
			offset = Time::DaysInMonth(month,year);
		}

		Year = year;
		Month = month;
		
		
		Day = days+1;
		Hour = hours;
		Minute = minutes;
		Second = seconds;

	}


	return true;

}

long long Local::Seconds()
{
	 

	long long seconds=0;

	if (Year < 1970)
	{
		for (int month=Month;month<12;++month)
			seconds -= Time::DaysInMonth(month,Year) * Time::SecondsPerDay;

		for (int day=Day;day<Time::DaysInMonth(Month,Year);++day)
			seconds -= Time::SecondsPerDay;

		for (int hour=Hour;hour<24;++hour)
			seconds -= Time::SecondsPerHour;

		for (int minute=Minute;minute<60;++minute)
			seconds -= Time::SecondsPerMinute;

		seconds -= Time::SecondsPerMinute-Second;

	}
	else
	{
		for (int month=1;month<Month;++month)
			seconds += Time::DaysInMonth(month,Year) * Time::SecondsPerDay;

		for (int day=1;day<Day;++day)
			seconds += Time::SecondsPerDay;

		for (int hour=1;hour<Hour;++hour)
			seconds += Time::SecondsPerHour;

		for (int minute=1;minute<Minute;++minute)
			seconds += Time::SecondsPerMinute;

		seconds += Second;

	}
	

	return seconds;

	
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Elapsed::Construct()
{
	
	return true;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Time::Construct()
{
	Seconds = 0;
	Milliseconds = 0;

	
	
	
	
	
	
	timeb time;
	ftime(&time);


	
	
	
	
	
	
	
	
	
	
	struct tm local;
	if (Times::Localtime((time_t*)&time.time,&local))
	{
		local.tm_isdst=0;

		
		
		
		

		time_t seconds = mktime(&local);
		Construct(seconds);

		return true;
	}

	return false;
}

bool Time::Construct(const Calendar & calendar)
{
	Seconds = 0;
	Milliseconds = 0;
	if (calendar.Year && calendar.Month)
	{
		
		
		
		
		
		Epoch epoch("1/1/1970");
		time_t seconds = Calendar(calendar).Seconds(epoch);

		

		return Construct(seconds);
	}

	return false;
}

bool Time::Construct(char * data, int size)
{
	Seconds = 0;
	Milliseconds = 0;
	Calendar calendar(data,size);
	if (calendar.Year)
	{
		
		
		
		
		
		
		
		return Construct(calendar);
	}

	return false;
}

Time & Time::operator += (const Interval & interval)
{
	Seconds += interval.Seconds;
	Milliseconds += interval.Microseconds / Time::MicrosecondsPerMillisecond;
	return *this;
}

Time & Time::operator -= (const Interval & interval)
{
	Seconds -= interval.Seconds;
	Milliseconds -= interval.Microseconds / Time::MicrosecondsPerMillisecond;
	return *this;
}

Interval Time::operator - (const Time & time)
{
	return Elapsed(*this,time).Interval;
}

Time Time::operator - (Interval & interval)
{
	return Time(Seconds-interval.Seconds,Milliseconds-(interval.Microseconds/Time::MicrosecondsPerMillisecond));
}

Time Time::operator + (Interval & interval)
{
	return Time(Seconds-interval.Seconds,Milliseconds-(interval.Microseconds/Time::MicrosecondsPerMillisecond));
}


int Time::Hour()
{
	

	Calendar calendar(*this);
	return calendar.Hour;
}

int Time::Minute()
{
	

	Calendar calendar(*this);
	return calendar.Minute;
}

int Time::Second()
{
	

	Calendar calendar(*this);
	return calendar.Second;
}

int Time::Millisecond()
{
	return Milliseconds;
}



int Time::Weekday()
{
	Calendar calendar(*this);
	return calendar.Weekday();
}


String Time::Weekday(Time & time)
{
	
	

	
	


	int weekday = time.Weekday();
	return String(Weekday::Weekdays[weekday]);

}

String Time::Weekday(Calendar & calendar)
{
	
	int weekday = calendar.Weekday();
	return String(Weekday::Weekdays[weekday]);
}

int Time::Day()
{
	
	

	Calendar calendar(*this);
	return calendar.Day;
}


String Time::Day(Time & time)
{
	
	
	

	return String(time.Day());
}

String Time::Day(Calendar & calendar)
{
	return String(calendar.Day);
}

String Time::Day(int day)
{
	return String(day);
}


int Time::Month()
{
	
	

	Calendar calendar(*this);
	return calendar.Month;
}


String Time::Month(Time & time)
{
	

	
	
	return String(Month::Months[time.Month()-1]);
}

String Time::Month(Calendar & calendar)
{
	return String(Month::Months[calendar.Month-1]);
}

String Time::Month(int month)
{
	return String(Month::Months[month-1]);
}


int Time::Year()
{
	
	

	Calendar calendar(*this);
	return calendar.Year;
}

String Time::Year(Time & time)
{
	
	
	

	return String(time.Year());
}

String Time::Year(Calendar & calendar)
{
	return String(calendar.Year);
}

String Time::Year(int year)
{
	return String(year);
}



int Time::Ordinal()
{		
	return Time::Ordinal(Calendar(*this));	
}

int Time::Ordinal(const Calendar & calendar)
{
	

	int days=0;
	for (int month=1;month < calendar.Month;++month)
		days += DaysInMonth(month);
	days += calendar.Day;
	
	return days;
}


bool Time::IsLeapYear() 
{
	

	Calendar calendar(*this);
	return IsLeapYear(calendar.Year);

}

int Time::DaysInYear()
{
	
	Calendar calendar(*this);
	return IsLeapYear(calendar.Year);
}

int Time::DaysInMonth()
{
	

	Calendar calendar(*this);
	return DaysInMonth(calendar.Month);
}

void Time::Print(String & string)
{
	

	Calendar calendar(*this);
	Formatter::Append(string,"%s %s %02d %04d",Time::Weekday(calendar).Slice(0,3).Print(), Time::Month(calendar).Slice(0,3).Print(),calendar.Day,calendar.Year); 
	string << " ";
	Formatter::Append(string,"%02d:%02d:%02d %s",calendar.Hour%12,calendar.Minute,calendar.Second,((calendar.Hour%12)==calendar.Hour)?"AM":"PM");
	
	

}	


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TimeParser::Parse()
{
	while (!Eof())
	{
		
		while (ParseClock() || ParseDate() || ParseMonth() || ParseDay() || ParseYear() || ParseWeekday() || ParseSymbol())
		{
			if (Eof())
				break;
		}
		return Eof();

		
	}

	return false;
}

bool TimeParser::ParseClock()
{
	if (IsNumeric())
	{
		class Token state(Scanner->Token);
		while(ParseNumber())
		{
			if (Token.Size <= 2 && Is(':'))
			{
				
				Next();
				int hour = Token.Integer();
				if (ParseNumber())
				{
					int minute = Token.Integer();
					int second = 0;
					if (Is(':')) 
					{
						Next();
						if (ParseNumber())
							second = Token.Integer();
					}
					
					SkipWhitespace();

					

					
					
					

					ParseWord();
					if (Token.Is("am",true))
					{
						SkipWord();

						
						
						
						
						
						

						if (hour == 12 || hour == 24)
							hour = 0;
						
						Assert(hour < 12);
					}
					else
					if (Token.Is("pm",true))
					{
						SkipWord();

						
						
						
						
						

						
						if (hour < 12)
							hour +=12;
					}
						
					Handled->ProcessHour(hour);
					Handled->ProcessMinute(minute);
					Handled->ProcessSecond(second);
					
					return true;
				}					
			}
		}
		Load(state);
	}

	return false;
}

bool TimeParser::ParseClockLiteral()
{
	
	return false;
}

bool TimeParser::ParseDate()
{
	if (IsAny(1,"/-.") || IsAny(2,"/-.") || IsAny(3,"/-.") || IsAny(4,"/-."))
	{
		class Token state(Scanner->Token);
		
		
		
		
		
		
		
		

		int day=0;
		int month=0;
		int year=0;

		ParseDateLiteral();
		Next();	

		
		if (Token.Size >= 3)
		{
			
			
			
			
			year = Token.Integer();
			if (ParseDateLiteral() && Token.Size <= 2 && IsAny("/-."))
			{
				month = Token.Integer();
				Next();
				if (ParseDateLiteral() && Token.Size <= 2 && (IsWhitespace() || Eof()))
				{	
					day = Token.Integer();
					
					if (day <= 31 && month <= 12)
					{
						Handled->ProcessDay(day);
						Handled->ProcessMonth(month);
						Handled->ProcessYear(year);
						return true;
					}
				}
			}
		}
		else
		if (Token.Size <= 2)
		{
			
			day = Token.Integer();
			if (ParseDateLiteral() && Token.Size <= 2 && IsAny("/-."))
			{
				month = Token.Integer();
				Next();
				if (ParseDateLiteral() && (IsWhitespace() || IsSymbol() || Eof()))
				{	
					year = Token.Integer();

					
					
					if (day > 31 || month > 12)
					{
						int num = day;
						day = month;
						month = num;
					}
					
					if (day <= 31 && month <= 12)
					{
						Handled->ProcessDay(day);
						Handled->ProcessMonth(month);
						Handled->ProcessYear(year);
						return true;
					}
				}
			}
		}

		Load(state);
	}

	return false;
}

bool TimeParser::ParseDateLiteral()
{
	if (IsNumeric())
	{
		ParseNumber();
		
		
		if ( Token.Size <= 4 && (IsAny("/-.") || IsWhitespace() || IsSymbol() || Eof()))
		{
			
			return true;
		}
	}
	return false;
}


bool TimeParser::ParseDay()
{
	
	
	
	
	
	if (IsNumeric())
	{
		class Token state(Scanner->Token);
		ParseNumber();

		if (Token.Size <= 2 && (Is(',') || IsWhitespace() || Is(Number::Suffix(Token.Integer())) ))
		{
			if (Is(Number::Suffix(Token.Integer()))) Next();
			Next();
			Handled->ProcessDay(Token.Integer());
			return true;
		}

		Load(state);
	}

	return false;
}

bool TimeParser::ParseDayLiteral()
{
	
	return false;
}

bool TimeParser::ParseWeekday()
{
	
	

	if (IsAlpha())
	{
		if (ParseWeekdayLiteral())
		{
			if (Is(",")) Next();
			return true;
		}
	}

	return false;
}

bool TimeParser::ParseWeekdayLiteral()
{
	if (IsAlpha())
	{
		class Token state(Scanner->Token);
		if (ParseWord())
		{
			
			
			
			
			int weekday = 0;
			if ((Token.Is("sun",true)||Token.Is("sunday",true))
				|| (++weekday && (Token.Is("mon",true)||Token.Is("monday",true)))
				|| (++weekday && (Token.Is("tue",true)||Token.Is("tuesday",true)))
				|| (++weekday && (Token.Is("wed",true)||Token.Is("wednesday",true)))
				|| (++weekday && (Token.Is("thu",true)||Token.Is("thursday",true)))
				|| (++weekday && (Token.Is("fri",true)||Token.Is("friday",true)))
				|| (++weekday && (Token.Is("sat",true)||Token.Is("saturday",true)))
				)
			{
				Handled->ProcessWeekday(weekday);
				return true;
			}
		}

		Load(state);
	}
	return false;
}


bool TimeParser::ParseMonth()
{
	if (IsAlpha())
	{
		if (ParseMonthLiteral())
		{
			if (Is(",")) Next();
			return true;
		}
	}

	return false;
}

bool TimeParser::ParseMonthLiteral()
{
	if (IsAlpha())
	{
		class Token state(Scanner->Token);
		if (ParseWord())
		{
			
			
			
			
			int month = 1;
			if ((Token.Is("jan",true)||Token.Is("january",true))
				|| (++month && (Token.Is("feb",true)||Token.Is("february",true)))
				|| (++month && (Token.Is("mar",true)||Token.Is("march",true)))
				|| (++month && (Token.Is("apr",true)||Token.Is("april",true)))
				|| (++month && (Token.Is("may",true)))
				|| (++month && (Token.Is("jun",true)||Token.Is("june",true)))
				|| (++month && (Token.Is("jul",true)||Token.Is("july",true)))
				|| (++month && (Token.Is("aug",true)||Token.Is("august",true)))
				|| (++month && (Token.Is("sep",true)||Token.Is("september",true)))
				|| (++month && (Token.Is("oct",true)||Token.Is("october",true)))
				|| (++month && (Token.Is("nov",true)||Token.Is("november",true)))
				|| (++month && (Token.Is("dec",true)||Token.Is("december",true)))
				)
			{
				Handled->ProcessMonth(month);
				return true;
			}
		}

		Load(state);
	}
	return false;
}

bool TimeParser::ParseYear()
{
	if (IsNumeric())
	{
		class Token state(Scanner->Token);
		
		
		if (ParseNumber() && Token.Size == 4 && !IsAny("/-."))
		{
			Handled->ProcessYear(Token.Integer());
			return true;
		}
		Load(state);
	}
	return false;

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char * Month::Months[] = {"January","February","March","April","May","June","July","August","September","October","November","December"}; 

const char * Weekday::Weekdays[] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Calendar::Construct(char * data, int size)
{
	Day = Month = Year = Hour = Minute = Second = 0;

	CalendarReader reader(this);
	reader.Assign(data,size);
	return reader.Parse();
}

bool Calendar::Construct(const Calendar & calendar)
{
	Day = calendar.Day;
	Month = calendar.Month;
	Year = calendar.Year;
	Hour = calendar.Hour;
	Minute = calendar.Minute;
	Second = calendar.Second;
	return true;
}


bool Calendar::Construct(const struct tm & local)
{
	Day = local.tm_mday;
	Month = local.tm_mon+1; 
	
	
	
	
	
	
	
	Year = local.tm_year+1900;
	Hour = local.tm_hour;
	Minute = local.tm_min;
	Second = local.tm_sec;
	return true;
}

bool Calendar::Construct(const time_t & seconds)
{
	
	
	
	
	
	
	

	

	Local local(seconds);
	Year = local.Year;
	Month = local.Month;
	Day = local.Day;
	Hour = local.Hour;
	Minute = local.Minute;
	Second = local.Second;

	return true;
}


bool Calendar::Construct(const Time & time)
{
	return Construct((time_t)time.Seconds);
}

void Calendar::Format(String & string, const char * format)
{
	
	

	string.Reserve(64);

	if (Year && Month)
	{
		
		
		
		
		
		
		Timestamp timestamp;
		timestamp.Construct(format);
		timestamp.Format(string,*this);

		
	}

	string.Term();
}


void Calendar::Print(class String & string)
{
	
	Format(string,"%a %b %d %Y %I:%M:%S %p");
	
	
	
	
	
	
	
	
	
	
	

	
	
	
	
	
	

}


double Calendar::Years(const Calendar & calendar)
{
	
	Calendar * prior = this;
	Calendar * after = (Calendar*)&calendar;
	if (calendar  < (*this))
	{
		prior = (Calendar*)&calendar;
		after = this;
	}

	
	double years = prior->CalendarMonthsRemaining()/Time::MonthsPerYear;

	
	years += ((prior->Year+1)<after->Year)?after->Year-(prior->Year+1):0;

	
	
	
	if (prior->Year == after->Year)
		years -= after->CalendarMonthsRemaining()/Time::MonthsPerYear;
	else
		years += after->CalendarMonths()/Time::MonthsPerYear;

	return (calendar < (*this))?years:-years;
}

double Calendar::Months(const Calendar & calendar)
{
	
	Calendar * prior = this;
	Calendar * after = (Calendar*)&calendar;
	if (calendar  < (*this))
	{
		prior = (Calendar*)&calendar;
		after = this;
	}

	
	double months = prior->CalendarMonthsRemaining();

	
	for (int year=prior->Year+1;year<after->Year;++year)
		months += Time::MonthsPerYear;

	
	
	
	if (prior->Year == after->Year)
		months -= after->CalendarMonthsRemaining();
	else
		months += after->CalendarMonths();

	return (calendar < (*this))?months:-months;
}

double Calendar::Weeks(const Calendar & calendar)
{
	
	Calendar * prior = this;
	Calendar * after = (Calendar*)&calendar;
	if (calendar  < (*this))
	{
		prior = (Calendar*)&calendar;
		after = this;
	}

	
	double weeks = prior->CalendarWeeksRemaining();

	
	for (int year=prior->Year+1;year<after->Year;++year)
		weeks += Time::DaysInYear(year) / Time::WeeksPerYear;

	
	
	
	if (prior->Year == after->Year)
		weeks -= after->CalendarWeeksRemaining();
	else
		weeks += after->CalendarWeeks();

	return (calendar < (*this))?weeks:-weeks;
}

double Calendar::Days(const Calendar & calendar)
{
	
	Calendar * prior = this;
	Calendar * after = (Calendar*)&calendar;
	if (calendar  < (*this))
	{
		prior = (Calendar*)&calendar;
		after = this;
	}

	
	double days = prior->CalendarDaysRemaining();

	
	for (int year=prior->Year+1;year<after->Year;++year)
		days += Time::DaysInYear(year);

	
	
	
	if (prior->Year == after->Year)
		days -= after->CalendarDaysRemaining();
	else
		days += after->CalendarDays();

	return (calendar < (*this))?days:-days;
}

double Calendar::Hours(const Calendar & calendar)
{
	
	Calendar * prior = this;
	Calendar * after = (Calendar*)&calendar;
	if (calendar  < (*this))
	{
		prior = (Calendar*)&calendar;
		after = this;
	}

	
	double hours = prior->CalendarHoursRemaining();

	
	for (int year=prior->Year+1;year<after->Year;++year)
		hours += Time::DaysInYear(year) * Time::HoursPerDay;

	
	
	
	if (prior->Year == after->Year)
		hours -= after->CalendarHoursRemaining();
	else
		hours += after->CalendarHours();

	return (calendar < (*this))?hours:-hours;
}

double Calendar::Minutes(const Calendar & calendar)
{
	
	Calendar * prior = this;
	Calendar * after = (Calendar*)&calendar;
	if (calendar  < (*this))
	{
		prior = (Calendar*)&calendar;
		after = this;
	}

	
	double minutes = prior->CalendarMinutesRemaining();

	
	for (int year=prior->Year+1;year<after->Year;++year)
		minutes += Time::DaysInYear(year) * Time::MinutesPerDay;

	
	
	
	if (prior->Year == after->Year)
		minutes -= after->CalendarMinutesRemaining();
	else
		minutes += after->CalendarMinutes();

	return (calendar < (*this))?minutes:-minutes;
}

long long Calendar::operator - (const Calendar & calendar)
{
	

	Calendar * prior = this;
	Calendar * after = (Calendar*)&calendar;
	if (calendar  < (*this))
	{
		prior = (Calendar*)&calendar;
		after = this;
	}

	long long seconds=0;
	
	
	for (int month=1;month<prior->Month;++month)
		seconds += Time::DaysInMonth(month,prior->Year) * Time::SecondsPerDay;
	
	seconds += (prior->Day-1) * Time::SecondsPerDay;
	seconds += (prior->Hour*Time::SecondsPerHour) + (prior->Minute*Time::SecondsPerMinute) + prior->Second;

	
	
	seconds = (prior->Year == after->Year)?-seconds:Time::DaysInYear(prior->Year) * Time::SecondsPerDay - seconds;

	
	for (int year=prior->Year+1;year<after->Year;++year)
		seconds += Time::DaysInYear(year) * Time::SecondsPerDay;

	
	for (int month=1;month<after->Month;++month)
		seconds += Time::DaysInMonth(month,after->Year) * Time::SecondsPerDay;

	seconds += (after->Day-1) * Time::SecondsPerDay;
	seconds += (after->Hour*Time::SecondsPerHour) + (after->Minute*Time::SecondsPerMinute) + after->Second;

	return (calendar < (*this))?seconds:-seconds;
}



int Calendar::Weekday()
{
	
	
	


	

	
	
	

	

	
	
	
	
	

	

	
	
	
	
	
	

	
	

	
	
	
	
	
	
	Calendar gregorian(15,10,1582);
	int days = Days(gregorian);
	int weekday = days%7;
	weekday = (weekday+5)%7;
	
	return weekday;
	
	

	
	

	
	
	
	
	
	

	
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CalendarReader::CalendarReader(class Calendar * calendar):Calendar(calendar)
{
}

bool CalendarReader::Parse()
{
	Handled = this;
	TimeParser::Parse();
	return ((long long)*Calendar) != 0;
}

void CalendarReader::ProcessDay(int day)
{
	Calendar->Day = day;
}

void CalendarReader::ProcessWeekday(int weekday)
{
	
}

void CalendarReader::ProcessMonth(int month)
{
	
	
	

	Calendar->Month = month;
}

void CalendarReader::ProcessYear(int year)
{
	Calendar->Year = year;
}


void CalendarReader::ProcessHour(int hour)
{
	Calendar->Hour = hour;
}

void CalendarReader::ProcessMinute(int minute)
{
	Calendar->Minute = minute;
}

void CalendarReader::ProcessSecond(int second)
{
	Calendar->Second = second;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Interval::Construct(char * data, int size)
{
	Seconds = 0;
	Microseconds = 0;

	IntervalReader reader(this);
	reader.Assign(data,size);
	return reader.Parse();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IntervalReader::Parse()
{
	Handled = this;
	TimeParser::Parse();
	return Interval->Seconds > 0 || Interval->Microseconds > 0;
}


void IntervalReader::ProcessHour(int hour)
{
	(*Interval) += hour * Time::SecondsPerHour;
}

void IntervalReader::ProcessMinute(int minute)
{
	(*Interval) += minute * Time::SecondsPerMinute;
}

void IntervalReader::ProcessSecond(int second)
{
	(*Interval) += second;
}

void IntervalReader::ProcessDay(int day)
{
	
}

void IntervalReader::ProcessWeekday(int weekday)
{
	
}

void IntervalReader::ProcessMonth(int month)
{
	
}

void IntervalReader::ProcessYear(int year)
{
	
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





double Epoch::Years()
{
	if (Ticks == 0) return 0;

	long long ticks = Ticks/Scale;
	
	double seconds = CalendarSecondsRemaining();
	double years=0; 
	if (ticks < seconds)
	{
		
		
		seconds= Time::DaysInYear(Year)*Time::SecondsPerDay;
		years += (double)ticks/seconds;
	}
	else
	{
		
		

		
		years = CalendarMonthsRemaining()/Time::MonthsPerYear;
		ticks -= seconds;
		for (int year=Year+1;ticks>0;++year)
		{
			seconds= Time::DaysInYear(year)*Time::SecondsPerDay;
			years += (ticks-seconds > 0)?1:(double)ticks/seconds;
			ticks -= seconds;
		}
	}

	return years;
}

double Epoch::Years(const Epoch & epoch)
{
	
	

	
	
	
	double years = Calendar::Years((Calendar&)epoch);

	if (years > 0)
	{
		
		years -= Years();

		
		

		return years - ((Epoch&)epoch).Years();
	}
	else
	{
		
		years += ((Epoch&)epoch).Years();

		return Years() + years;
	}
}

double Epoch::Months()
{
	
	if (Ticks == 0) return 0;

	long long ticks = Ticks/Scale;
	
	double seconds = CalendarSecondsRemaining();
	double months = 0;
	if (ticks < seconds)
	{
		
		
		
		
		for (int month=Month;ticks>0;++month)
		{
			seconds = Time::DaysInMonth(month,Year)*Time::SecondsPerDay;
			months += (ticks-seconds > 0)?1:(double)ticks/seconds;
			ticks -= seconds;	
		}
	}
	else
	{
		
		

		
		ticks -= seconds;
		months = CalendarMonthsRemaining();
		for (int year=Year+1;ticks>0;++year)
		{
			seconds= Time::DaysInYear(year)*Time::SecondsPerDay;
			if (ticks-seconds > 0)
			{
				months += Time::MonthsPerYear;
				ticks -= seconds;
			}
			else
			{
				
				
				
				for (int month=1;ticks>0;++month)
				{
					seconds = Time::DaysInMonth(month,year)*Time::SecondsPerDay;
					months += (ticks-seconds > 0)?1:(double)ticks/seconds;
					ticks -= seconds;	
				}
			}
		}
	}
	
	return months;
}

double Epoch::Months(const Epoch & epoch)
{
	

	double months = Calendar::Months((Calendar&)epoch);

	if (months > 0)
	{
		months -= Months();
		return months - ((Epoch&)epoch).Months();
	}
	else
	{
		months += ((Epoch&)epoch).Months();
		return Months() + months;
	}
}


double Epoch::Seconds(const Epoch & epoch) 
{
	
	
	
	
	double factor = (epoch.Scale > Scale)?epoch.Scale:Scale;
	
	
	if (factor == 0) factor = 1;

	
	
	

	long long ticks = 0;
	long long seconds = (Calendar&)(*this)-(Calendar&)epoch;
    if (seconds > 0)
	{
		
		ticks = Ticks/((double)Scale/factor);
		ticks += seconds*factor;
		
		return (double)(ticks - epoch.Ticks/((double)epoch.Scale/factor))/factor;
	}
	else 
	{
		
		ticks = epoch.Ticks/((double)epoch.Scale/factor);
		ticks -= seconds*factor;

		return (double)(Ticks/((double)Scale/factor) - ticks)/factor;
	}
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Timestamp::Construct(const char * format)
{

	
	
	
	String::Construct((char*)format,String::Length(format));

	StringParser parser(*this);
	
	
	
	
	
	
	
	
	
	
	

	
	int index=0;

	while (!parser.Eof())
	{
		if (parser.Is('%'))
		{			

			Field field;
			
			parser.Mark(field.Format);			
			parser.Next();

			
			if (parser.Is("-"))
			{
				parser.Next();
				switch (*parser.At())
				{
					case 'd': field.On(Field::TYPE_FORMAT_a);break;
					case 'm': field.On(Field::TYPE_FORMAT_m);break;
					case 'y': field.On(Field::TYPE_FORMAT_y);break;
					case 'H': field.On(Field::TYPE_FORMAT_H);break;
					case 'I': field.On(Field::TYPE_FORMAT_I);break;
					case 'M': field.On(Field::TYPE_FORMAT_M);break;
					case 'S': field.On(Field::TYPE_FORMAT_S);break;
					case 'j': field.On(Field::TYPE_FORMAT_j);break;
					default:
						continue;
				}
			}
			else
			{
				switch (*parser.At())
				{
					case 'a': field.On(Field::TYPE_FORMAT_a);break;
					case 'A': field.On(Field::TYPE_FORMAT_A);break;
					case 'w': field.On(Field::TYPE_FORMAT_w);break;
					case 'd': field.On(Field::TYPE_FORMAT_dd);break;
					case 'b': field.On(Field::TYPE_FORMAT_b);break;
					case 'B': field.On(Field::TYPE_FORMAT_B);break;
					case 'm': field.On(Field::TYPE_FORMAT_mm);break;
					case 'y': field.On(Field::TYPE_FORMAT_yy);break;
					case 'Y': field.On(Field::TYPE_FORMAT_Y);break;
					case 'H': field.On(Field::TYPE_FORMAT_HH);break;
					case 'I': field.On(Field::TYPE_FORMAT_II);break;
					case 'p': field.On(Field::TYPE_FORMAT_p);break;
					case 'M': field.On(Field::TYPE_FORMAT_MM);break;
					case 'S': field.On(Field::TYPE_FORMAT_SS);break;
					case 'f': field.On(Field::TYPE_FORMAT_f);break;
					case 'z': field.On(Field::TYPE_FORMAT_z);break;
					case 'Z': field.On(Field::TYPE_FORMAT_Z);break;
					case 'j': field.On(Field::TYPE_FORMAT_jj);break;
					case 'U': field.On(Field::TYPE_FORMAT_U);break;
					case 'W': field.On(Field::TYPE_FORMAT_W);break;
					case 'c': field.On(Field::TYPE_FORMAT_c);break;
					case 'x': field.On(Field::TYPE_FORMAT_x);break;
					case 'X': field.On(Field::TYPE_FORMAT_X);break;

					case '%': field.On(Field::TYPE_PERCENT); break;
					default:
						continue;
				}
			}
			

			parser.Next();
			parser.Trap(field.Format);

			field.Index = index++;
			Fields.Append(field);

		}
		else
		{
			parser.Next();
		}
	}

	
	
	

}



void Timestamp::Format(String & string, Calendar & calendar)
{
	
	
	
	
	
	
	
	
	
	
	
	int from=0;
	int to=0;
	for (int i=0;i<Fields.Count;++i)
	{
		Field & field = Fields[i];

		
		to = IndexOf(field.Format);
		string << Slice(from,to);
		from = to+field.Format.Size;

		if (field.Is(Field::TYPE_FORMAT_a|Field::TYPE_FORMAT_A))
		{
			Substring weekday = Weekday::Weekdays[calendar.Weekday()];
			if (field.Is(Field::TYPE_FORMAT_a))
				weekday = weekday.Slice(0,3);				
			string.Append(weekday);
		}
		else
		if (field.Is(Field::TYPE_FORMAT_w))
		{
			int weekday = calendar.Weekday();
			string.Append(weekday);
		}
		else
		if (field.Is(Field::TYPE_FORMAT_dd|Field::TYPE_FORMAT_d))
		{
			if (field.Is(Field::TYPE_FORMAT_dd))
				Formatter::Append(string,"%02d",calendar.Day);
			else
				string.Append(calendar.Day);				
		}
		else		
		if (field.Is(Field::TYPE_FORMAT_b|Field::TYPE_FORMAT_B))
		{
			Substring month = Month::Months[calendar.Month-1];		
			if (field.Is(Field::TYPE_FORMAT_b))
				month = month.Slice(0,3);
			string.Append(month);
		}
		else
		if (field.Is(Field::TYPE_FORMAT_mm|Field::TYPE_FORMAT_m))
		{
			if (field.Is(Field::TYPE_FORMAT_mm))
				Formatter::Append(string,"%2d",calendar.Month);
			else
				string.Append(calendar.Month);
		}
		else
		if (field.Is(Field::TYPE_FORMAT_yy|Field::TYPE_FORMAT_y))
		{
			int year = calendar.Year%100;
			if (field.Is(Field::TYPE_FORMAT_yy))
				Formatter::Append(string,"%2d",year);
			else
				string.Append(year);
		}
		else		
		if (field.Is(Field::TYPE_FORMAT_Y))
		{
			
			
			
			
			
			
			
			Formatter::Append(string,"%04d",calendar.Year);
			
		}
		else
		if (field.Is(Field::TYPE_FORMAT_HH|Field::TYPE_FORMAT_H))
		{
			if (field.Is(Field::TYPE_FORMAT_HH))
				Formatter::Append(string,"%02d",calendar.Hour);
			else
				Append(calendar.Hour);
		}
		else
		if (field.Is(Field::TYPE_FORMAT_II|Field::TYPE_FORMAT_I))
		{		
			if (field.Is(Field::TYPE_FORMAT_II))
				Formatter::Append(string,"%02d",calendar.Hour%12);
			else
				Append(calendar.Hour%12);
		}
		else
		if (field.Is(Field::TYPE_FORMAT_p))
		{
			String ampm = ((calendar.Hour%12)==calendar.Hour)?"AM":"PM";
			Append(ampm);
		}		
		else
		if (field.Is(Field::TYPE_FORMAT_MM|Field::TYPE_FORMAT_M))
		{
			if (field.Is(Field::TYPE_FORMAT_MM))
				Formatter::Append(string,"%02d",calendar.Minute);
			else
				string.Append(calendar.Minute);	
		}
		else
		if (field.Is(Field::TYPE_FORMAT_SS|Field::TYPE_FORMAT_S))
		{
			if (field.Is(Field::TYPE_FORMAT_SS))
				Formatter::Append(string,"%02d",calendar.Second);
			else
				string.Append(calendar.Second);
		}
		else
		if (field.Is(Field::TYPE_FORMAT_jj|Field::TYPE_FORMAT_j))
		{
			
			int ordinal = Time::Ordinal(calendar);
			if (field.Is(Field::TYPE_FORMAT_jj))
				Formatter::Append(string,"%03d",ordinal);
			else
				string.Append(ordinal);
		}
				
		
		
		
		
		

	}

	
	string << Slice(from);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
