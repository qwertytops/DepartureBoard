#include "FileParser.hpp"

using namespace std;

string FileParser::GetInput(string message) {
    cout << message << endl;
    string input; 
    getline(cin, input);

    if (input == "exit" || input == "quit") 
    {
        exit(EXIT_SUCCESS);
    }

    return input;
}

vector<string> FileParser::ReadFile(string path) {

    path.erase(path.find_last_not_of(" \n\r\t")+1);
    if (path.at(0) == '"' && path.size() > 1) // removes potential quotes around path
    {
        path = path.substr(1, path.size() - 2);
    }
    fstream inputFile(path);

    if (!inputFile.is_open()) {
        cerr << "Could not open file! Potential causes:" << endl << 
        "Typo in file name. Name is case sensitive" << endl << 
        "File extension missing (.ttb)" << endl <<
        "One or more spaces in the path - enclose the whole path in quotation marks" <<endl <<
        "(Windows) File is locked. Try running this program before opening file in RailOS" << endl << endl;
        restartProgram();
    }
    
    stringstream buffer;
    buffer << inputFile.rdbuf();
    string contents = buffer.str();
    inputFile.close();

    return split(contents, '\0');
}

unordered_map<string, Location*> FileParser::FindLocations(vector<string>& entries)
{
    unordered_map<string, Location*> result = {};

    for (string& s : entries) 
    {
        vector<string> lines = split(s, ',');
        if (lines.size() <= 1 || lines[0].at(0) == '*') 
        {
            continue; // it's a comment entry
        }

        for (string& l : lines)
        {
            vector<string> parts = split(l, ';');

            if (FileParser::isTimeTimeLoc(parts)) 
            {
                result[parts[2]] = new Location(parts[2]);
            }
            if (FileParser::isTimeLoc(parts)) {
                result[parts[1]] = new Location(parts[1]);
            }
        }
        
    }
    return result;
}

unordered_map<string, Service*> FileParser::FindServices(vector<string>& entries)
{
    unordered_map<string, Service*> result = {};

    for (string& s : entries)
    {
        vector<string> lines = split(s, ',');
        if (s.size() <= 1 || s.at(0) == '*' || lines.size() < 3) 
        {
            continue; // it's a comment entry or other non-service
        }
        string code = split(lines[0], ';')[0];
        result[code] = new Service(code);        
    }

    return result;
}

void FileParser::HandleServices(vector<string>& entries, unordered_map<string, Location *>& locations, unordered_map<string, Service*>& services)
{
    for (string& s : entries)
    {
        vector<string> lines = split(s, ',');

        if (s.size() <= 1 || s.at(0) == '*' || lines.size() < 3) 
        {
            continue; // it's a comment entry or other non-service
        }

        string headcode = split(lines[0], ';')[0];
        if (lines.size() > 1 && lines[lines.size() - 1].at(0) == 'R') 
        {
            HandleRepeat(lines, locations, services, headcode);
        }
        else
        {
            AssignServiceEvents(lines, locations, services, headcode, 0);
        }
    }
    

}

void FileParser::HandleRepeat(vector<string>& lines, unordered_map<string, Location *>& locations, unordered_map<string, Service*>& services, string headcode) 
{
    // split into separate services
    vector<string> repeatInfo = split(lines[lines.size() - 1], ';'); // R, mins between, headcode increase, num repeats
    int minsBetween = stoi(repeatInfo[1]);
    int headcodeIncrease = stoi(repeatInfo[2]);
    int numRepeats = stoi(repeatInfo[3]);
    for (int i = 0; i <= numRepeats; i++)
    {
        string newHeadcode = incrementHeadcode(headcode, headcodeIncrease*i);

        if (i != 0) 
        {
            services[newHeadcode] = new Service(newHeadcode);
        }
        
        AssignServiceEvents(lines, locations, services, newHeadcode, minsBetween * i);
    }
}

void FileParser::AssignServiceEvents(vector<string>& lines, unordered_map<string, Location *>& locations, unordered_map<string, Service*>& services, string headcode, int timeAdd)
{
    enum State 
    {
        UNKNOWN, MOVING, STATIONARY,
    };
    int state = UNKNOWN;
    for (size_t i = 0; i < lines.size(); ++i)
    {
        vector<string> parts = split(lines[i], ';');

        if (FileParser::isTimeTimeLoc(parts)) // TimeTimeLoc
        {
            services[headcode]->arrivals[Time::addMinutes(parts[0], timeAdd)] = locations[parts[2]];
            locations[parts[2]]->arrivals.push_back({Time::addMinutes(parts[0], timeAdd), headcode});
            services[headcode]->departures[Time::addMinutes(parts[1], timeAdd)] = locations[parts[2]];
            locations[parts[2]]->departures.push_back({Time::addMinutes(parts[1], timeAdd), headcode});
            state = MOVING;
        }
        if (FileParser::isTimeLoc(parts))
        {
            if (state == UNKNOWN)
            {
                /* 
                starting from next line, check each line
                If TimeTimeLoc is reached, current line is departure
                If TimeLoc is reached, same location means current is arrival, otherwise departure
                */
                for (size_t j = i + 1; j < lines.size(); j++) 
                {
                    vector<string> parts1 = split(lines[j], ';');
                    if (FileParser::isTimeTimeLoc(parts1))
                    {
                        locations[parts[1]]->departures.push_back({Time::addMinutes(parts[0], timeAdd), headcode});
                        state = MOVING;
                    } 
                    else if (FileParser::isTimeLoc(parts1))
                    {
                        if (parts[1] == parts1[1]) // locations are the same
                        {
                            services[headcode]->arrivals[Time::addMinutes(parts[0], timeAdd)] = locations[parts[1]];
                            locations[parts[1]]->arrivals.push_back({Time::addMinutes(parts[0], timeAdd), headcode});
                            state = STATIONARY;
                        }
                        else
                        {
                            services[headcode]->departures[Time::addMinutes(parts[0], timeAdd)] = locations[parts[1]];
                            locations[parts[1]]->departures.push_back({Time::addMinutes(parts[0], timeAdd), headcode});
                            state = MOVING;
                        }
                    }
                }
            } 
            else if (state == MOVING)
            {
                services[headcode]->arrivals[Time::addMinutes(parts[0], timeAdd)] = locations[parts[1]];
                locations[parts[1]]->arrivals.push_back({Time::addMinutes(parts[0], timeAdd), headcode});
                state = STATIONARY;
            }
            else if (state == STATIONARY)
            {
                services[headcode]->departures[Time::addMinutes(parts[0], timeAdd)] = locations[parts[1]];
                locations[parts[1]]->departures.push_back({Time::addMinutes(parts[0], timeAdd), headcode});
                state = MOVING;
            }
        }
    }
}

void FileParser::restartProgram()
{
    char exePath[1024];

#ifdef _WIN32
    // Get executable path
    GetModuleFileName(NULL, exePath, sizeof(exePath));

    // Restart program
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    if (CreateProcess(exePath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        exit(0);
    }
#elif __linux__
    // Get executable path
    ssize_t count = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);
    if (count != -1)
    {
        exePath[count] = '\0';
        execl(exePath, exePath, NULL);
    }
#elif __APPLE__
    uint32_t size = sizeof(exePath);
    if (_NSGetExecutablePath(exePath, &size) == 0)
    {
        execl(exePath, exePath, NULL);
    }
#endif

    std::cerr << "Failed to restart program!" << std::endl;
}

vector<string> FileParser::split(string s, char delim) 
{
    vector<string> result;
    
    size_t size = s.size();

    string temp = "";
    for (size_t i = 0; i < size; i++)
    {
        if (s.at(i) == delim) {
            result.push_back(temp);
            temp = "";
            continue;
        }

        temp += s.at(i);
    }
    result.push_back(temp);
    return result;
}

bool FileParser::isTimeTimeLoc(vector<string> parts)
{
    return parts.size() == 3 && Time::isTime(parts[1]);
}

bool FileParser::isTimeLoc(vector<string> parts)
{
    return parts.size() == 2 && Time::isTime(parts[0]) && parts[1] != "cdt";
}

string FileParser::incrementHeadcode(string headcode, int amount)
{   
    try 
    {
        int lastTwoDigits = stoi(headcode.substr(headcode.size()-2, 2));
        string pad = lastTwoDigits + amount < 10 ? "0" : "";
        return headcode.substr(0, headcode.size()-2) + pad + to_string(lastTwoDigits + amount);
    } 
    catch (exception)
    {
        return headcode;
    }
    
}
