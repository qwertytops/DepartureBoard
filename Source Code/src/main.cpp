#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>

#include "FileParser.hpp"
#include "Location.hpp"
#include "Service.hpp"
#include "DisplaySettings.hpp"

using namespace std;

int main() 
{
    string filePath = FileParser::GetInput("Enter file path: ");

    vector<string> entries = FileParser::ReadFile(filePath);

    unordered_map<string, Location*> locations = FileParser::FindLocations(entries);
    unordered_map<string, Service*> services = FileParser::FindServices(entries);

    FileParser::HandleServices(entries, locations, services);
    
    DisplaySettings::GetInfoToDisplay(DisplaySettings::GetMode(), locations, services);

    return 1;
}