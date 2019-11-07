/**
 *  main.cpp
 *  Main for the Duck Interpreter. Defines the entry point.
 *  Final Project.
 *
 *  Created by Salil Maharjan on 3/13/19.
 *  Copyright © 2019 Salil Maharjan. All rights reserved.
 */

#include "PrefixHeader.pch"
//#include "stdafx.h"
#include "DuckInterpreter.hpp"

int main(int argc, char *argv[])
{
    // Checking for correct arguments
    if (argc!=2)
    {
        cerr<<"Usage: DuckInterp <filename>"<<endl;
        return 1;
    }
    
    //Create the interpreter object and use it to record the statements
    //and execute them.
    DuckInterpreter duckInt;
    
    // Running the interpreter
    duckInt.RecordStatements(argv[1]);
    duckInt.RunInterpreter();
    
    return 0;
}
