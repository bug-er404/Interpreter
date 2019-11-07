/**
 *  DuckInterpreter.hpp
 *  DuckInterpreter Class header file that runs the interpreter.
 *  Uses Statement.hpp and SymbolTable.hpp.
 *
 *  Created by Salil Maharjan on 3/13/19.
 *  Copyright © 2019 Salil Maharjan. All rights reserved.
 */

#pragma once
#include "PrefixHeader.pch"
//#include "stdafx.h"
#include "Statement.hpp"
#include "SymbolTable.hpp"

class DuckInterpreter
{
public:
    DuckInterpreter();
    ~DuckInterpreter();
    
    // Method to record statements from a source file.
    // Calls Statement::RecordStatements.
    void RecordStatements(string a_fileName)
    {
        m_statements.RecordStatements(a_fileName);
        
    }
    
    // Method that runs the interpreter.
    void RunInterpreter();
    
private:
    // Statement variable that holds the code statements.
    Statement m_statements;
    
    // SymbolTable variable that holds the value of all variables.
    SymbolTable m_symbolTable;
    
    // Operator stack used for evaluating arithmetic statements.
    vector<string> m_operatorStack;
    // Number stack used for evaluating arithmetic statements.
    vector<double> m_numberStack;
    
    // Class of all the different types of statements in the Duck Language.
    enum class StatementType
    {
        ArithmeticStat,
        IfStat,
        ReadStat,
        PrintStat,
        StopStat,
        EndStat,
        GotoStat,
        CommentStat,
    };

    // Method to execute statements.
    int ExecuteStatement(string a_statement, int a_nextStatement);
    
    // Method to parse elements.
    int ParseNextElement(const string &a_statement, int a_nextPos, string &a_stringValue, double &numValue);
    
    // Method to get statement type.
    StatementType GetStatementStype(const string &a_string);
    
    // Checks if a_checkItem is inside quotes in string a_statement.
    bool InsideQuotes(string a_checkItem, string a_statement);
    
    // Checks if a_checkItem is in the comment section.
    bool IsCommented(string a_checkItem, string a_string);
    
    // Evaluate an arithmetic statement.
    void EvaluateArithmeticStatement(const string &a_statement );
    
    // Evaluate an arithmetic expression.  Return the value.
    double EvaluateArithmenticExpression(const string &a_statement, int nextPos);
    
    // Gets Operator Precedence of t_operator. Used by EvaluateArithmeticExpression.
    int GetOperatorPrecedence(const string a_operator);
    
    // Does operation on val1 and val2. Used by EvaluateArithmeticExpression.
    double DoOperation(double a_val1, double a_val2, string a_operation);
    
    // Checks and counts for not(!) operators and returns if we need to perform the operation or not.
    // Removes the unary operators once recorded.
    bool CheckNotOperator(string &a_statement);
    
    // Evaluates an if statement to determine if the goto should be executed.
    int EvaluateIfStatement(string a_statement, int a_nextStatement);
    
    // Searches till the end of the statement for the label and returns it
    // And replaces the goto statement with ";"
    string EvaluateIfGotoExpression(string &a_statement);

    // Evaluates Quoted Prompts. Used by EvaluatePrintStatement and EvaluateReadStatement.
    void EvaluateQuotedPrompt(string &a_statement, bool &quoted, int &nextPos, string &resultString, double &placeHolder);
    
    // Evaluates Print statements.
    void EvaluatePrintStatement(string a_statement);
    
    // Evaluates Read statements.
    void EvaluateReadStatement(string a_statement);
    
    // Evaluates Goto statement.
    int EvaluateGotoStatement(const string &a_statement);
    
};


