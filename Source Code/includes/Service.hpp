#pragma once

#include <vector>

#include "Location.hpp"

using namespace std;

class Service 
{
public:
    string code;
    map<string, Location*> arrivals;
    map<string, Location*> departures;

    Location* GetSource();
    Location* GetDestination();
    Location* GetNextLocation(string time);

    Service(string c);
    
    vector<pair<pair<string, Location*>, bool>> eventsMerged();
};