#pragma once

#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "Time.hpp"
using namespace std;

class Location {
public:
    string name;

    vector<pair<string, string>> arrivals; // time, code
    vector<pair<string, string>> departures;
    vector<pair<pair<string, string>, bool>> eventsMerged();
    void sortEvents();

    Location(string n);
};