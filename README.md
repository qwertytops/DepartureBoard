# Departure Board

Author: Liam van der Moezel
Created 25 Jun 24
Last updated: 31 Jul 24

Source code available at https://github.com/qwertytops/DepartureBoard
Designed for use with RailOS which can be found on https://railwayoperationsimulator.com

## Description
This program reads a .ttb file and displays the arrivals and/or departures scheduled for a specified location or service

## Instructions for use

**MacOS users** For security reasons you must compile the program yourself (Instructions in Mac Compilation Guide)

1. Run the executable compatible with your operating system
2. Enter the file path of the .ttb file you want to view (absolute or relative)
3. Select display mode
    (1) Departure Board: shows arrivals/departures for a location
    (2) Service Timetable: shows arrivals/departures for a service
4. Enter location name or service code depending on mode
5. Select events to display
6. Program will loop back to step 4
7. Display mode can be changed by entering 'mode', which will return to step 3

Quit program by:
- entering 'exit' or 'quit' at step 2 or 3
- entering ctrl-C at any point
- closing the terminal

## Bugs
Please report any bugs to Qwertytops on the RailOS Discord, via Discord DMs, or as an issue on https://github.com/qwertytops/DepartureBoard
