#include "Service.hpp"

Service::Service(string c) 
{
    code = c;
    arrivals = {};
    departures = {};
}

Location* Service::GetSource()
{
    auto firstArrival = arrivals.begin();
    auto firstDeparture = departures.begin();
    return Time::toInt(firstArrival->first) < Time::toInt(firstDeparture->first) ? firstArrival->second : firstDeparture->second;
}

Location* Service::GetDestination()
{   
    return prev(arrivals.end())->second;
}

Location* Service::GetNextLocation(string time)
{
    Location* next = {};
    for (const auto &e : arrivals) 
    {
        if (Time::toInt(e.first) > Time::toInt(time))
        {
            next = e.second;
            break;
        }
    }

    return next != nullptr ? next : new Location("error");
}

vector<pair<pair<string, Location*>, bool>> Service::eventsMerged()
{
    vector<pair<pair<string, Location*>, bool>> result = {};
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