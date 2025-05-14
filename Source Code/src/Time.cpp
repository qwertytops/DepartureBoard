#include "Time.hpp"
using namespace std;


int Time::toInt(string s) {
    string hour = s.substr(0, 2);
    string minute = s.substr(3, 2);
    return stoi(hour) * 60 + stoi(minute);
}

bool Time::isTime(string s)
{
    try
    {
        if (s.at(2) == ':') 
        {
            return true;
        }
        return false;
    }
    catch(const exception& e)
    {
        return false;
    }
}

string Time::addMinutes(string s, int minutes)
{
    if (s.at(0) == 'W')
    {
        s = s.substr(1, 5);
    }

    if (!isTime(s)) { throw new exception; }
    int hour = stoi(s.substr(0, 2));
    int minute = stoi(s.substr(3, 2));

    while (minutes >= 60) 
    {
        hour++;
        minutes -= 60;
    }
    while (minutes >= 1) 
    {
        if (minute == 59)
        {
            minute -= 59;
            hour++;
        }
        else
        {
            minute++;
        }
        minutes -= 1;
    }
    string padH = hour < 10 ? "0" : "";
    string padM = minute < 10 ? "0" : "";
    return padH + to_string(hour) + ":" + padM + to_string(minute);
}
