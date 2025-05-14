#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <vector>

#include "FileParser.hpp"
#include "Service.hpp"
#include "Location.hpp"

using namespace std;

class DisplaySettings 
{
    enum Mode { DepartureBoard, ServiceTimetable };
public:
    static int GetMode();
    static void GetInfoToDisplay(int mode, unordered_map<string, Location*> locations, unordered_map<string, Service*> services);

private:
    static int DisplayMode();
    static void ShowDepartureBoard(unordered_map<string, Location*> locations, unordered_map<string, Service*> services);
    static void ShowServiceTimetable(unordered_map<string, Location*> locations, unordered_map<string, Service*> services);
    static void Display(vector<string> lines);
};