main: main.cpp flight.hpp
	g++ -o main main.cpp -lmysqlcppconn -std=c++11 
clean:
	rm -rf main
