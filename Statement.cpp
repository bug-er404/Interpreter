/**
 *  DuckInterpreter.cpp
 *  Implementation of Statement.hpp
 *
 *
 *  Created by Salil Maharjan on 3/13/19.
 *  Copyright © 2019 Salil Maharjan. All rights reserved.
 */

#include "Statement.hpp"
#include "PrefixHeader.pch"
//#include "stdafx.h"
#include "DuckInterpreter.hpp"

/**
 * Statement::Statement. Constructor for Statement class.
 * Empty.
 * @author Salil Maharjan
 * @date 03/13/19
 */
Statement::Statement(){}

/**
 * Statement::~Statement. Destructor for Statement class.
 * Empty.
 * @author Salil Maharjan
 * @date 03/13/19
 */
Statement::~Statement(){}

/**
 * Statement::RecordStatements. Method to record statements from a source file.
 * Records code statements on each line from file "a_sourceFileName". Every time it gets a line from the file, the method
 * checks if there are any labels in the statement. If there are, it records them in "m_labelToStatement" with the label
 * name as key and associated line number as value. After recording the label, it removes it from the statement.
 * After removing the label, it calls StandardSpaceFormat method to ensure that consistent formatting is followed in the code.
 * After standardizing the spaces, the method records the statement in the vector "m_statements".
 * @param a_sourceFileName string File that has the duck code statements.
 * @see StandardSpaceFormat
 * @author Salil Maharjan
 * @date 03/13/19
 */
void Statement::RecordStatements(string a_sourceFileName)
{
    // Flag if end statement is found.
    bool end_statement_found  = false;
    
    // Loop iterators
    int i,j;
    
    // Valid Label Flag
    bool neg_label=0;
    string buffer;
    
    // Open file and check if it opened correctly.
    ifstream inf(a_sourceFileName);
    if(!inf)
    {
        cerr << "Could not open the file: " << a_sourceFileName << endl;
        exit(1);
    }
    
    // Getting all code statements from the file
    while(!inf.eof())
    {
        // Get each line
        getline(inf,buffer);
        
        // Ignoring empty lines.
        if(buffer.empty() || buffer == " ")
            continue;
        
        // Checking if we have found the if statement.
        // Report error if there are more lines after the end statement.
        if(end_statement_found && !buffer.empty())
        {
            cerr<<"Error: Additional statements found after the end statement."<<endl;
            cerr<<"Statement: "<<buffer<<endl;
            cerr<<"There cannot be code after the end statement" <<endl;
            exit(1);
        }
        
        // Checking for end statement and updating flag.
        // If the loop runs after the flag is true, we have more statements after the end statement.
        // Reports error in this case.
        if(buffer.find("end;") != string::npos || buffer.find("end ;") != string::npos)
        {
            if(buffer.find("\"") == string::npos)
                end_statement_found = true;
        }
        
        
        // Checking for Labels
        for(i=0; i<(buffer.size());i++)
        {
            neg_label=0;
            if(buffer[i]==':')
            {
                // Checking if the label is valid
                for(j=0; j<i; j++)
                {
                    if(buffer[j]=='\"')
                        neg_label=1;
                }
                
                // If label is not valid
                if (neg_label)
                    break;
                
                // Recording valid label in a map and removing it from the statement.
                m_labelToStatement[buffer.substr(0,i)] = ((int)m_statements.size()+1);
                buffer.erase(0,i+1);
                break;
                
            }
        }
        
        // Standardizing the input to have consistent spaces in the statement.
        StandardSpaceFormat(buffer);
        
        // Recording statement
        m_statements.push_back(buffer);
    }
}

/**
 * Statement::GetLabelLocation. Accessor to get label location.
 * Tries to get the location of the label "a_string" from the map "m_labeelToStatement".
 * Throws invalid label error if label is not found.
 * @param a_string string Holds the name of the label
 * @return int The position of the label in the code if found.
 * @author Salil Maharjan
 * @date 03/13/19
 */
int Statement::GetLabelLocation(string a_string)
{
    int pos = 0;
    
    // Try to get the label position from the map.
    try {
        pos = m_labelToStatement.at(a_string);
    } catch (std::out_of_range&  e) {
        std::cerr << "Invalid Label found: " << a_string << endl;
        std::cerr << e.what() << std::endl;
        throw a_string;
    }
    
    return (pos-1);
}

/**
 * Statement::NeedSpace. Method to check if it is a character we need to cheeck spaces for.
 * True if we need to check space for the character, False otherwise.
 * Also used to check if it is safe to insert a space in the position of 'element'.
 * If false, we insert a space. If true, we do nothing.
 * @param element char Element to check if is a character we need to check spaces for.
 * @return bool True/False if it is a character we need to check spaces for.
 * @author Salil Maharjan
 * @date 03/13/19
 */
bool Statement::NeedSpace(char element)
{
    return (!isalpha(element) && !isdigit(element) && element != '_' && element != ' ' && element != ';');
}

/**
 * Statement::StandardSpaceFormat. Method to standardize spaces for the code statements.
 * Get a uniform space formatting for the statements of the Duck language. Uses method NeedSpace to check
 * for characters that need space considerations.
 * @param a_string string Holds the code statement to standardize spaces.
 * @see NeedSpace
 * @author Salil Maharjan
 * @date 03/13/19
 */
void Statement::StandardSpaceFormat(string &a_string)
{
    // Flag to keep track of quotations.
    bool quoted = false;
    
    // Iterating through each character.
    for(int i = 0; i <a_string.length(); i++)
    {
        // Jump to next quotation mark if we find a quotation.
        if(a_string.c_str()[i] == '"')
        {
            i = (int)a_string.find("\"", i+1);
            continue;
        }
        
        // Checking and breaking loop if we find Comment syntax.
        if(a_string.c_str()[i] == '/' && quoted == false && a_string.c_str()[i+1]=='/')
            break;
        
        // Check if it is a character that we need to check spaces for.
        if(NeedSpace(a_string.c_str()[i]))
        {
            // Left boundary condition.
            if(i == 0)
            {
                if (NeedSpace(a_string.c_str()[i+1])){}
                else
                {
                    a_string.insert(i+1, " ");
                    i++;
                }
                continue;
            }
            
            // Right boundary condition.
            if(i == a_string.length()-1)
            {
                if (NeedSpace(a_string.c_str()[i-1])){}
                else
                {
                    a_string.insert(i, " ");
                    i++;
                }
                continue;
            }
            
            // Checking position before if we need spaces.
            if (NeedSpace(a_string.c_str()[i-1]) && a_string.c_str()[i] != '!' && a_string.c_str()[i] != '('){}
            else
            {
                a_string.insert(i, " ");
                i++;
            }
            // Checking position after if we need spaces.
            if (NeedSpace(a_string.c_str()[i+1]) && a_string.c_str()[i] != ')'){}
            else
            {
                a_string.insert(i+1, " ");
                i++;
            }
        }
    }
}
