#include "flight.hpp"
#include <iostream>
#include <mysqlx/xdevapi.h>

using namespace std;

// class Flight
Flight::Flight(string id, string dep, string arr, string time, int total, int remaining)
    : flightID(id), departure(dep), arrival(arr), flightTime(time), totalTickets(total), remainingTickets(remaining) {}

bool Flight::bookTicket()
{
    if (remainingTickets > 0)
    {
        remainingTickets--; // update on vector only
        try // update on database mysql
        {
            mysqlx::Session sess("127.0.0.1", 33060, "root", "");  // password empty
            mysqlx::Schema schema = sess.getSchema("flight_db");
            mysqlx::Table flightsTable = schema.getTable("flights");

            flightsTable.update()
                        .set("remainingTickets", remainingTickets)
                        .where("flightID = :id")
                        .bind("id", flightID)
                        .execute();
            return true;
        }
        catch (const mysqlx::Error& err)
        {
            cout << "mysql error: " << err.what() << endl;
            return false;
        }
    }
    else
    {
        return false;
    }
}

void Flight::refundTicket()
{
    if (remainingTickets < totalTickets)
    {
        remainingTickets++;
        cout << "Ticket refunded." << endl;
    }
    else
    {
        cout << "No tickets refundable." << endl;
    }
    try
    {
        mysqlx::Session sess("127.0.0.1", 33060, "root", ""); // password empty
        mysqlx::Schema schema = sess.getSchema("flight_db"); 
        mysqlx::Table flightsTable = schema.getTable("flights");

        flightsTable.update()
                    .set("remainingTickets", remainingTickets)
                    .where("flightID = :id")
                    .bind("id", flightID)
                    .execute();

    }
    catch (const mysqlx::Error& err)
    {
        cout << "mysql error: " << err.what() << endl;
    }
    
}

void Flight::displayInfo() const
{
    cout << "Flight ID : " << flightID << " | Departure : " << departure << " | Arrival : " << arrival << " | Time : " << flightTime << " | Tickets: " << remainingTickets << "/" << totalTickets << endl;
}
string Flight::getID() const { return flightID; }
string Flight::getDeparture() const { return departure; }
string Flight::getArrival() const { return arrival; }
string Flight::getTime() const { return flightTime; }
int Flight::getRemainingTickets() const { return remainingTickets; }



// class FlightDatabase
void FlightDatabase::loadFromDatabase() 
{
    try 
    {
        mysqlx::Session session("127.0.0.1", 33060, "root", ""); // password empty
        mysqlx::Schema db = session.getSchema("flight_db");

        mysqlx::Table flightTable = db.getTable("flights");

        mysqlx::RowResult res = flightTable.select("flightID", "departure", "arrival", "flightTime", "totalTickets", "remainingTickets").execute();

        for (mysqlx::Row row : res)
        {
            Flight f(
            row[0].get<std::string>(),   // flightID
            row[1].get<std::string>(),   // departure
            row[2].get<std::string>(),   // arrival
            row[3].get<std::string>(),   // flightTime
            row[4].get<int>(),          // totalTickets
            row[5].get<int>());          // remainingTickets
            
            flights.push_back(f);
        }
    }
    catch (const mysqlx::Error &err)
    {
        std::cerr << "ERROR: " << err.what() << endl;
    }
    catch (std::exception &e)
    {
        std::cerr << "STD EXCEPTION: " << e.what() << endl;
    }
}

void FlightDatabase::addFlight(const Flight& flight)
{
    flights.push_back(flight);
}

Flight* FlightDatabase::findFlightByID(const string& id)
{
    for (Flight& flight : flights)
    {
        if (flight.getID() == id)
            return &flight;
    }
    return nullptr;
}

vector<Flight*> FlightDatabase::findFlightsByDeparture(const string& dep)
{
    vector<Flight*> results;
    for (Flight& flight : flights)
    {
        if (flight.getDeparture() == dep)
            results.push_back(&flight);
    }
    return results;
}

vector<Flight*> FlightDatabase::findFlightsByArrival(const string& arr)
{
    vector<Flight*> results;
    for (Flight& flight : flights)
    {
        if (flight.getArrival() == arr)
            results.push_back(&flight);
    }
    return results;
}

vector<Flight*> FlightDatabase::findFlightsByTime(const string& time)
{
    vector<Flight*> results;
    for (Flight& flight : flights)
    {
        if (flight.getTime() == time)
            results.push_back(&flight);
    }
    return results;
}

void FlightDatabase::displayAllFlights() const
{
    for (const auto& flight : flights)
        flight.displayInfo();
}



// class FlightQuery
FlightQuery::FlightQuery(FlightDatabase& database) : db(database) {}

void FlightQuery::QueryBy()
{
    cout << "Query by:" << endl;
    cout << "1. Flight ID\n2. Departure Station\n3. Arrival Station\n4. Flight Time" << endl;
    cout << "Enter choice: ";
    char choice;
    cin >> choice;
    
    string input;
    vector<Flight*> results;

    switch (choice)
    {
        case '1':
            cout << "Enter flight ID: ";
            cin >> input;
            
            if (Flight* f = db.findFlightByID(input))
                f->displayInfo();
            else
                cout << "Flight not found." << endl;
            break;

        case '2':
            cout << "Enter departure station: ";
            cin >> input;
            
            results = db.findFlightsByDeparture(input);
            displayResults(results);
            break;

        case '3':
            cout << "Enter arrival station: ";
            cin >> input;
           
            results = db.findFlightsByArrival(input);
            displayResults(results);
            break;

        case '4':
            cout << "Enter flight time: ";
            cin >> input;
            
            results = db.findFlightsByTime(input);
            displayResults(results);
            break;

        default:
            cout << "Invalid option." << endl;
    }
}

void FlightQuery::displayResults(const vector<Flight*>& flights)
{
    if (flights.empty())
        cout << "No matching flights." << endl;
    else
    {
        for (const auto& flight : flights)
        {
            flight->displayInfo();
        }
    }
}


void displayMenu() 
{
    cout << "   ___   _     ___            ___            __    _             ____         __          " << endl;
    cout << "  / _ | (_)___/ (_)__  ___   / _ )___  ___  / /__ (_)__  ___ _  / __/_ _____ / /____ __ _ " << endl;
    cout << " / __ |/ / __/ / / _ \\/ -_) / _  / _ \\/ _ \\/  '_// / _ \\/ _ `/ _\\ \\/ // (_-</ __/ -_)  ' \\" << endl;
    cout << "/_/ |_/_/_/ /_/_/_//_/\\__/ /____/\\___/\\___/_/\\_\\/_/_//_/\\_, / /___/\\_, /___/\\__/\\__/_/_/_/" << endl;
    cout << "                                                       /___/      /___/                   " << endl;
 
    cout << "*******************************************************************************************" << endl;
    cout << "*                                                                                         *" << endl;
    cout << "*  1. View all flights                                                                    *" << endl;
    cout << "*  2. Search for a flight                                                                 *" << endl;
    cout << "*  3. Book a ticket                                                                       *" << endl;
    cout << "*  4. Refund a ticket                                                                     *" << endl;
    cout << "*  5. Exit                                                                                *" << endl;
    cout << "*                                                                                         *" << endl;
    cout << "*******************************************************************************************" << endl;
    cout << "Choose an option: ";
}

int main() 
{
    FlightDatabase db;
    FlightQuery query(db);

    db.loadFromDatabase(); // COMMENT THIS LINE ; IF YOU CAN'T CONNECT TO DB
    // UNCOMMENT THE FOLLOWING LINES TO ADD FLIGHTS MANUALLY ; IF YOU CAN'T CONNECT TO DB
    // db.addFlight(Flight("A101", "Paris", "London", "10:00", 100, 100));
    // db.addFlight(Flight("B202", "Paris", "Berlin", "12:30", 80, 80));
    // db.addFlight(Flight("C303", "Lyon", "Madrid", "15:45", 50, 50));
    // db.addFlight(Flight("D404", "Beijing", "Paris", "22:00", 2, 2));

    char choice;
    do 
    {
        displayMenu();
        cin >> choice;
        
        cout << endl;
        switch (choice) 
        {
            case '1':
                db.displayAllFlights();
                break;

            case '2':
                query.QueryBy();
                break;

            case '3':
            {
                string id;
                cout << "Enter flight ID to book: ";
                cin >> id;
                Flight* flight = db.findFlightByID(id);
                if (flight) {
                    if (flight->bookTicket())
                        cout << "Ticket booked successfully." << endl;
                    else
                        cout << "No tickets available." << endl;
                } else {
                    cout << "Flight not found." << endl;
                }
                break;
            }

            case '4':
            {
                string id;
                cout << "Enter flight ID to refund: ";
                cin >> id;
                Flight* flight = db.findFlightByID(id);
                if (flight)
                    flight->refundTicket();
                else
                    cout << "Flight not found or there is no ticket to refund" << endl;
                break;
            }

            case '5':
                cout << "Exit!" << endl;
                break;

            default:
                cout << "Invalid choice. Try again." << endl;
        }
    }
    while (choice != '5');

    return 0;
}
