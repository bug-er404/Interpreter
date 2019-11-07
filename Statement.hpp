/**
 *  Statement.hpp
 *  Statement Class header file.
 *  Records and provides statements and labels.
 *
 *  Created by Salil Maharjan on 3/13/19.
 *  Copyright © 2019 Salil Maharjan. All rights reserved.
 */

#pragma once
#include "PrefixHeader.pch"
//#include "stdafx.h"

class Statement
{
public:
    Statement();
    ~Statement();
    
    // Method to record statements from a file.
    void RecordStatements(string a_sourceFileName);
    
    // Accessor to get statements from the class.
    string GetStatement(int a_statementNum)
    {
        if(a_statementNum < m_statements.size())
            return m_statements[a_statementNum];
        else
        {
            cerr << "Invalid Label Number to Statement" << a_statementNum;
            exit(1);
        }
    }
  
    // Gets back the statement number that the label is pointing
    int GetLabelLocation(string a_string);
    
private:
    // Vector that holds the statements.
    vector<string> m_statements;
    vector<string>::iterator itr;
    
    // Map that holds the labels from the code their respective position in the code.
    map<string,int> m_labelToStatement;
    map<string, int>::iterator mpi;

    // Get a uniform space formatting
    void StandardSpaceFormat(string &a_string);
    
    // Check if it is a character that we need to check spaces for.
    bool NeedSpace(char element);
};
