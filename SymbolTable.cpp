/**
 *  SymbolTable.cpp
 *  Implementation of SymbolTable.hpp
 *
 *
 *  Created by Salil Maharjan on 3/13/19.
 *  Copyright © 2019 Salil Maharjan. All rights reserved.
 */

#include "PrefixHeader.pch"
//#include "stdafx.h"
#include "SymbolTable.hpp"

/**
 * SymbolTable::SymbolTable. Constructor for SymbolTable class.
 * Empty.
 * @author Salil Maharjan
 * @date 03/13/19
 */
SymbolTable::SymbolTable(){}

/**
 * SymbolTable::~SymbolTable. Destructor for SymbolTable class.
 * Empty.
 * @author Salil Maharjan
 * @date 03/13/19
 */
SymbolTable::~SymbolTable(){}

/**
 * SymbolTable::GetVariableValue. Accessor to get the variable value.
 * Checks if the variable is in the map and return its corresponding value.
 * Returns false if the value is not found.
 * @author Salil Maharjan
 * @date 03/13/19
 */
bool SymbolTable::GetVariableValue(string a_variable, double &a_value)
{
    if(m_SymbolTable.find(a_variable) != m_SymbolTable.end())
    {
        a_value = m_SymbolTable[a_variable];
        return true;
    }
    else
        return false;
}
