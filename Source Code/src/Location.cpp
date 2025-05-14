#include "Location.hpp"

using namespace std;

vector<pair<pair<string, string>, bool>> Location::eventsMerged()
{
    vector<pair<pair<string, string>, bool>> result = {};

    auto itA = arrivals.begin();
    auto itD = departures.begin();

    while (itA != arrivals.end() && itD != departures.end())
    {
        if (Time::toInt(itA->first) <= Time::toInt(itD->first))
        {
            result.push_back({{itA->first, itA->second}, true});
            itA++;
        }
        else
        {
            result.push_back({{itD->first, itD->second}, false});
            ++itD;
        }
    }

    while (itA != arrivals.end())
    {
        result.push_back({{itA->first, itA->second}, true});
        ++itA;
    }

    while (itD != departures.end())
    {
        result.push_back({{itD->first, itD->second}, false});
        ++itD;
    }

    return result;
}

void Location::sortEvents()
{
    sort(arrivals.begin(), arrivals.end());
    sort(departures.begin(), departures.end());

    arrivals.erase( unique( arrivals.begin(), arrivals.end() ), arrivals.end() );
    departures.erase( unique( departures.begin(), departures.end() ), departures.end() );
}

Location::Location(string n)
{
    name = n;
    arrivals = {};
    departures = {};
}