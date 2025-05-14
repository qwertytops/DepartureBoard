#include "DisplaySettings.hpp"

int DisplaySettings::GetMode()
{
    string input = FileParser::GetInput("\nSelect mode:\n1. Departure Board\n2. Service Timetable");
    try
    {
        int mode = stoi(input) - 1;
        return mode;
    }
    catch(const std::exception& e)
    {
        cerr << "Invalid input" << endl;
        GetMode();
    }
    return 0;
}

void DisplaySettings::GetInfoToDisplay(int mode, unordered_map<string, Location*> locations, unordered_map<string, Service*> services)
{
    switch (mode)
    {
        case DepartureBoard:
            ShowDepartureBoard(locations, services);
            break;
        
        case ServiceTimetable:
            ShowServiceTimetable(locations, services);
            break;

        default:
            cerr << "fatal error: mode invalid. Please contact program author";
            exit(EXIT_FAILURE);
            break;
    }
}

int DisplaySettings::DisplayMode()
{
    while (true) 
    {
        string input = FileParser::GetInput("\nDisplay arrivals or departures?\n1. Arrivals\n2. Departures\n3. Both");
        try
        {
            if (stoi(input) - 1 > 2) throw new exception;
            return stoi(input) - 1;
        }
        catch(const exception& e)
        {
            cout << "Invalid input" << endl;
        }
    }
}

void DisplaySettings::ShowDepartureBoard(unordered_map<string, Location*> locations, unordered_map<string, Service*> services)
{
    while (true) 
    {
        string location = FileParser::GetInput("\nEnter location name: ");

        if (location == "mode") 
        {
            GetInfoToDisplay(GetMode(), locations, services);
            return;
        }
        if (locations.count(location) == 0) 
        {
            cout << "Invalid location" << endl;
            continue;
        }

        Location* locObject = locations[location];
        locObject->sortEvents();

        switch (DisplaySettings::DisplayMode())
        {
            case 0:
                for (const auto &a : locObject->arrivals) 
                {
                    cout << a.first << " | " << a.second << " from " << services[a.second]->GetSource()->name << endl;
                }
                break;
            case 1:
                for (const auto &d : locObject->departures) 
                {
                    cout << d.first << " | " << d.second << " to " << services[d.second]->GetDestination()->name << endl;
                }
                break;
            case 2:
                for (const auto &d : locObject->eventsMerged()) 
                {
                    string toFrom = d.second ? "from" : "to";
                    string place = d.second ? services[d.first.second]->GetSource()->name : services[d.first.second]->GetDestination()->name;
                    cout << d.first.first << " | " << d.first.second << " " << toFrom << " " << place << endl;
                }
                break;
            default:
                cerr << "fatal error: mode invalid. Please contact program author";
                exit(EXIT_FAILURE);
                break;
        }
        cout << endl;
    }
}

void DisplaySettings::ShowServiceTimetable(unordered_map<string, Location*> locations, unordered_map<string, Service*> services)
{
    while (true) 
    {
        string service = FileParser::GetInput("\nEnter service code: ");
        if (service == "mode") 
        {
            GetInfoToDisplay(GetMode(), locations, services);
            return;
        }
        if (services.count(service) == 0) 
        {
            cout << "Invalid service code" << endl;
            continue;
        }
        Service* serviceObject = services[service];
        
        switch (DisplaySettings::DisplayMode())
        {
            case 0:
                if (serviceObject->arrivals.size() == 0) 
                {
                    cout << "No arrivals for service " << serviceObject->code;
                    break;
                }
                cout << serviceObject->GetSource()->name << " to " << serviceObject->GetDestination()->name << endl;
                for (const auto &a : serviceObject->arrivals) 
                {
                    cout << a.first << " | " << "Arrive " << a.second->name << endl;
                }
                break;
            case 1:
            if (serviceObject->departures.size() == 0) 
                {
                    cout << "No departures for service " << serviceObject->code << endl;
                    break;
                }
                cout << serviceObject->GetSource()->name << " to " << serviceObject->GetDestination()->name << endl;
                for (const auto &d : serviceObject->departures) 
                {
                    cout << d.first << " | " << "Depart " << d.second->name << endl;
                }
                break;
            case 2:
                for (const auto &d : serviceObject->eventsMerged()) 
                {
                    string arriveDepart = d.second ? "Arrive " : "Depart ";
                    cout << d.first.first << " | "  << arriveDepart << d.first.second->name << endl;
                }
                break;
            default:
                cerr << "fatal error: mode invalid. Please contact program author";
                exit(EXIT_FAILURE);
                break;
        }
        cout << endl;
    }
}

void DisplaySettings::Display(vector<string> lines)
{
    for (string& s : lines) 
    {
        cout << s << endl;
    }
}
