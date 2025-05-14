#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <vector>
#include <sstream>

#ifdef _WIN32
    #include <windows.h>
#elif __linux__ || __APPLE__
    #include <unistd.h>
    #include <mach-o/dyld.h>
#endif

#include "Service.hpp"
#include "Location.hpp"
using namespace std;

class FileParser {
public:
    static string GetInput(string message);

    static vector<string> ReadFile(string path);

    static unordered_map<string, Location*> FindLocations(vector<string>& entries);
    static unordered_map<string, Service*> FindServices(vector<string>& entries);

    static void HandleServices(vector<string>& entries, unordered_map<string, Location*>& locations, unordered_map<string, Service*>& services);

private:
    static void HandleRepeat(vector<string>& lines, unordered_map<string, Location *>& locations, unordered_map<string, Service*>& services, string headcode);
    static void AssignServiceEvents(vector<string>& lines, unordered_map<string, Location *>& locations, unordered_map<string, Service*>& services, string headcode, int timeAdd);

    static void restartProgram();
    
    static vector<string> split(string s, char delim);
    static bool isTimeTimeLoc(vector<string> parts);
    static bool isTimeLoc(vector<string> parts);
    static string incrementHeadcode(string headcode, int amount);
};
