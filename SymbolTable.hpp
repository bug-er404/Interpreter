/**
 *  SymbolTable.hpp
 *  SymbolTable Class header file.
 *  Records value of variables for access. This class will provide a mapping between the variables and their associated data.
 *
 *
 *  Created by Salil Maharjan on 3/13/19.
 *  Copyright © 2019 Salil Maharjan. All rights reserved.
 */

#pragma once

class SymbolTable
{
public:
    SymbolTable();
    ~SymbolTable();
    
    // Record the value of a variable.
    void RecordVariableValue(string a_variable, double a_value)
    {
        m_SymbolTable[a_variable] = a_value;
    }
    
    // Accessor to get the value of a variable. Returns false if the variable does not exist.
    bool GetVariableValue(string a_variable, double &a_value);
    
private:
    // Unordered map that has the variable as a string and its corresponding value as a double.
    unordered_map<string, double> m_SymbolTable;
};

