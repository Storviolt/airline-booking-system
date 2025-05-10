#pragma once

#include <string>
#include <vector>

class Flight 
{ 
private:
    std::string flightID; 
    std::string departure;
    std::string arrival; 
    std::string flightTime;
    int totalTickets;
    int remainingTickets; 

public:
    Flight(std::string id, std::string dep, std::string arr, std::string time, int total, int remaining); 
    bool bookTicket();
    void refundTicket();
    void displayInfo() const;
 
    std::string getID() const;
    std::string getDeparture() const;
    std::string getArrival() const;
    std::string getTime() const;
    int getRemainingTickets() const;
};
 
class FlightDatabase
{
private:
    std::vector<Flight> flights; 

public:
    void loadFromDatabase();
    void addFlight(const Flight &flight);
    Flight* findFlightByID(const std::string &id);
    std::vector<Flight*> findFlightsByDeparture(const std::string &dep);
    std::vector<Flight*> findFlightsByArrival(const std::string &arr);
    std::vector<Flight*> findFlightsByTime(const std::string &time);
    void displayAllFlights() const;
};

class FlightQuery
{
private:
    FlightDatabase &db;
    void displayResults(const std::vector<Flight*> &flights);

public:
    FlightQuery(FlightDatabase &database);
    void QueryBy();
};

void displayMenu();
