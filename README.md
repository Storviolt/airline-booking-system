# Flight Reservation System – C++ / MySQL

## Requirement

- **C++11 or higher**
- **MySQL Connector/C++ 8.0+ (MySQL X DevAPI)**: Used to interact with the MySQL database via C++ with the `#include <mysqlx/xdevapi.h>` header.
- **MySQL Server 8.0 or above**: Ensures compatibility with the MySQL X DevAPI and C++ connector.

## Prerequisite

Development was initially done on **macOS**, so the original Makefile had macOS-specific include/link paths and flags. 

> The Makefile uses the `-lmysqlcppconn` flag on Linux and `-lmysqlcppconnx` flag on MacOS.

Before running the program, make sure that MySQL service is **running** on your system.

### Setup Instructions

#### Linux

```bash
# Install MySQL and connector
sudo apt update
sudo apt install mysql-server
sudo apt install libmysqlcppconn-dev

# Start MySQL service
sudo service mysql start

# Access MySQL as root (empty password)
sudo mysql -u root
```

#### MacOS

```bash
# Install MySQL and connector using Homebrew
brew install mysql
brew install mysql-connector-c++

# Start MySQL service
brew services start mysql

# Access MySQL as root (empty password)
mysql -u root
```

### Initialize database

```mysql -u root < flights.sql```
If you can't initialize database with mysql-connector-c++, you can manually add flights into the vector flights by commenting/uncommenting the main function.

### Start the Program

```bash
# Compile the program with
make

# Adjust the Makefile if it doesn't compile. Run command :
./main

# Here is an example of MacOS makefile:
INCLUDE = /opt/homebrew/Cellar/mysql-connector-c++/9.3.0/include
LIB = /opt/homebrew/Cellar/mysql-connector-c++/9.3.0/lib

main: main.cpp flight.hpp
	g++ -o main main.cpp -lmysqlcppconnx -std=c++11 -I$(INCLUDE) -L$(LIB) 
clean:
	rm -rf main
```

### Useful MySQL commands

```
mysql
SHOW DATABASES;
USE flight_db;
SHOW TABLES;
SELECT * FROM flights;
```

