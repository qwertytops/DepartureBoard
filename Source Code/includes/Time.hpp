#pragma once

#include <string>
#include <iostream>
using namespace std;

class Time {
public:
    static int toInt(string s);

    static bool isTime(string s);

    static string addMinutes(string a, int minutes);
};