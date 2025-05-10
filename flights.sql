CREATE DATABASE flight_db;
USE flight_db;

CREATE TABLE flights (
    flightID VARCHAR(10) PRIMARY KEY,
    departure VARCHAR(10),
    arrival VARCHAR(10),
    flightTime VARCHAR(10),
    totalTickets INT,
    remainingTickets INT
);

INSERT INTO flights (flightID, departure, arrival, flightTime, totalTickets, remainingTickets) VALUES
('A101', 'Paris', 'London', '10:00', 100, 100),
('B202', 'Paris', 'Berlin', '12:30', 80, 80),
('C303', 'Lyon', 'Madrid', '15:45', 50, 50),
('D404', 'Beijing', 'Paris', '22:00', 2, 2);