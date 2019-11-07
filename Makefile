duckinterpreter: main.cpp DuckInterpreter.cpp Statement.cpp SymbolTable.cpp
	g++ -std=c++0x -o duckinterpreter main.cpp DuckInterpreter.cpp Statement.cpp SymbolTable.cpp -I