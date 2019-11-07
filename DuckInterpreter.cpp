/**
 *  DuckInterpreter.cpp
 *  Implementation of DuckInterpreter.cpp
 *
 *
 *  Created by Salil D. Maharjan on 3/13/19.
 *  Copyright © 2019 Salil D. Maharjan. All rights reserved.
 */

#include "DuckInterpreter.hpp"
#include "SymbolTable.hpp"
//#include "stdafx.h"
#include "PrefixHeader.pch"

/**
 * DuckInterpreter::DuckInterpreter. Constructor for DuckInterpreter class.
 * Empty.
 * @author Salil Maharjan
 * @date 03/13/19
 */
DuckInterpreter::DuckInterpreter() {}

/**
 * DuckInterpreter::~DuckInterpreter. Destructor for DuckInterpreter class.
 * Empty.
 * @author Salil Maharjan
 * @date 03/13/19
 */
DuckInterpreter::~DuckInterpreter(){}

/**
 * DuckInterpreter::RunInterpreter. Method to run the interpreter.
 * Gets the statements to execute until completed.
 * @see Statement::GetStatement
 * @see ExecuteStatement
 * @author Salil Maharjan
 * @date 03/13/19
 */
void DuckInterpreter::RunInterpreter()
{
    int nextStatement = 0;
    
    while(true)
    {
        string statement = m_statements.GetStatement(nextStatement);
        //cout<<"Statement: "<<statement<<endl;
        nextStatement = ExecuteStatement(statement,nextStatement);
    }
    
}

/**
 * DuckInterpreter::ExecuteStatement. Method to execute a statement.
 * Gets the statement type of a_statement and executes it by calling the respective functions.
 * @param a_statement string The statement to be executed.
 * @param a_nextStatement int Current statement number.
 * @return int Next statement number to execute.
 * @see GetStatementStype
 * @see EvaluateArithmeticStatement
 * @see EvaluateIfStatement
 * @see EvaluatePrintStatement
 * @see EvaluateReadStatement
 * @see EvaluateGotoStatement
 * @author Salil Maharjan
 * @date 03/13/19
 */
int DuckInterpreter::ExecuteStatement(string a_statement, int a_nextStatement)
{
    // Clear the stacks
    m_numberStack.clear();
    m_operatorStack.clear();
    
    // Get the first syntactic element of the statement.
    StatementType type = GetStatementStype(a_statement);
    
    // Based on the type, execute the remainder of the statement.
    switch (type)
    {
        case StatementType::ArithmeticStat:
            EvaluateArithmeticStatement(a_statement);
            return a_nextStatement + 1;
            
        case StatementType::IfStat:
            return EvaluateIfStatement(a_statement, a_nextStatement);
            
        case StatementType::StopStat:
            this->~DuckInterpreter();
            cout<< "**Exiting by a stop statement**"<<endl;
            cout<<"**Duck thanks you for using this language. Quack**"<<endl;
            exit(EXIT_SUCCESS);
            
        case StatementType::EndStat:
            this->~DuckInterpreter();
            cout<<"**Exiting by an end stateement**"<<endl;
            cout<<"**Duck thanks you for using this language. Quack**"<<endl;
            exit(EXIT_SUCCESS);
            
        case StatementType::PrintStat:
            EvaluatePrintStatement(a_statement);
            return a_nextStatement + 1;
            
        case StatementType::ReadStat:
            EvaluateReadStatement(a_statement);
            return a_nextStatement + 1;
            
        case StatementType::GotoStat:
            return EvaluateGotoStatement(a_statement);
            
        default:
            cerr << "BUGBUG - program terminate: invalid return value from GetStatementStype for the statement: " << a_statement << endl;
            exit(1);
    }
}

/**
 * DuckInterpreter::ParseNextElement. Element Parse Method.
 * Parses the next element of the statement. Uses istringstream to get each element separated by a white space.
 * @param a_statement const string The statement to parse.
 * @param a_nextPos int The element number of the element to parse from the statement.
 * @param a_stringValue string Captures the string elements from statement. "NULL" if the element is not a string.
 * @param numValue double Captures numeric elements from the statement. INT_MAX if the element is not a number.
 * @return int Position of the next element.
 * @author Salil Maharjan
 * @date 03/13/19
 */
int DuckInterpreter::ParseNextElement(const string &a_statement, int a_nextPos, string &a_stringValue, double &numValue)
{
    // Holds the next string element
    string str_element;
    bool end_of_statement = false;
    
    
    istringstream line(a_statement.c_str(), (int)a_statement.size());
    
    for(int i=0;i<=a_nextPos;i++)
    {
        str_element="";
        line>>str_element;
    }
    
    // Checking for end of statement semi-colon and removing it
    if(str_element.find(";") != string::npos && str_element.find(";")==str_element.length()-1 &&str_element.length()>1)
    {
        str_element.pop_back();
        end_of_statement = true;
    }
    
    // Checking if it is a number.
    // The number is captured in numValue and a_stringValue is set to "NULL".
    if(isdigit(str_element[0]))
    {
        numValue = atoi(str_element.c_str());
        a_stringValue = "NULL";
    }
    // Checking if it is a string.
    // The string is captured in a_stringValue and numValue is set to INT_MAX.
    else
    {
        a_stringValue = str_element;
        numValue = INT_MAX;
    }
    
    // If the end of statement is reached, we return INT_MAX
    if(end_of_statement || str_element == ";")
        return INT_MAX;
    
    // Returning the next element number
    a_nextPos++;
    return a_nextPos;
}

/**
 * DuckInterpreter::InsideQuotes. Quote checker method.
 * Checks if a_checkItem, which is a substring of a_string, is inside quotes in the string.
 * @param a_checkItem string Item to check if it is inside quotation.
 * @param a_string string The main string that has check_item in it.
 * @return bool True/False whether a_checkItem is inside quotations or not.
 * @author Salil Maharjan
 * @date 03/13/19
 */
bool DuckInterpreter::InsideQuotes(string a_checkItem, string a_string)
{
    // Checking if there is quotations in statement
    if(a_string.find("\"") != string::npos)
    {
        if(a_string.find("\"") < a_string.find(a_checkItem))
            return true;
        else
            return false;
    }
    else
        return false;
}

/**
 * DuckInterpreter::IsCommented. Comment checker method.
 * Checks if a_checkItem, which is a substring of a_string, is commented or not.
 * @param a_checkItem string Item to check if it is commented.
 * @param a_string string The main string that has check_item in it.
 * @return bool True/False whether a_checkItem is after comments or not.
 * @author Salil Maharjan
 * @date 03/13/19
 */
bool DuckInterpreter::IsCommented(string a_checkItem, string a_string)
{
    // Checking if there are comments in the statement
    if(a_string.find("//") != string::npos)
    {
        if(a_string.find("//") < a_string.find(a_checkItem))
            return true;
        else
            return false;
    }
    else
        return false;
}

/**
 * DuckInterpreter::GetStatementStype. Method that gets the Statement type.
 * Finds the statement type of a_string and returns it. Checks for specific identifiers that each statement has, does checks for comments and quotes and determines the statement type.
 * @param a_string const string String to find out the statement type of.
 * @return DuckInterpreter::StatementType The Statement Type of the passed string.
 * @see InsideQuotes
 * @see IsCommented
 * @author Salil Maharjan
 * @date 03/13/19
 */
DuckInterpreter::StatementType DuckInterpreter::GetStatementStype(const string &a_string)
{
    // Checking for whole line comments
    if(a_string.find("//") != string::npos)
    {
        if(a_string.find(";") < a_string.find("//")){}
        else
            return StatementType::CommentStat;
    }
    
    // Checking for if statements
    if(a_string.find("if") != string::npos)
    {
        if(InsideQuotes("if", a_string) || IsCommented("if", a_string)){}
        else
            return StatementType::IfStat;
    }
    // Checking for read statements
    else if(a_string.find("read") != string::npos)
    {
        if(InsideQuotes("read", a_string) || IsCommented("read", a_string)){}
        else
            return StatementType::ReadStat;
    }
    // Checking for print statements
    else if(a_string.find("print") != string::npos)
    {
        if(InsideQuotes("print", a_string) || IsCommented("print", a_string)){}
        else
            return StatementType::PrintStat;
    }
    // Checking for stop statements
    else if(a_string.find("stop;") != string::npos || a_string.find("stop ;") != string::npos)
    {
        if(InsideQuotes("stop", a_string) || IsCommented("stop", a_string)){}
        else
            return StatementType::StopStat;
    }
    // Checking for end statements
    else if(a_string.find("end;") != string::npos || a_string.find("end ;") != string::npos)
    {
        if(InsideQuotes("end", a_string) || IsCommented("end", a_string)){}
        else
            return StatementType::EndStat;
    }
    // Checking for goto statements
    else if(a_string.find("goto") != string::npos)
    {
        if(InsideQuotes("goto", a_string) || IsCommented("goto", a_string)){}
        else
            return StatementType::GotoStat;
    }
    // Else it is a arithmetic statement.
    else
    {
        // If we cannot find a ';', it means that the statement is invalid.
        // And we cannot determine the statement type.
        if(a_string.find(";")==string::npos)
        {
            cerr<< "Invalid statement type!"<<endl;
            cerr<<"Statement: "<< a_string<<endl;
            exit(1);
        }
        return StatementType::ArithmeticStat;
    }
    
    cerr<<"Cannot determine statement type."<<endl;
    exit(1);
}


/**
 * DuckInterpreter::EvaluateArithmeticStatement. Method to Evaluate Arithmetic Statements.
 * We know at this point that we have an arithementic expression. Execute this statement.  Any error will perminate the program.
 * The function first checks for unary arithmetic statements, if not the statement is evaluated by getting the result variable,
 * the assignment operator and then evaluating the rest of the arithmetic statement by calling EvaluateArithmeticExpression
 * @param a_statement const string String to find out the statement type of.
 * @see EvaluateArithmeticExpression
 * @see SymbolTable::GetVariableValue
 * @see SymbolTable::RecordVariableValue
 * @author Salil Maharjan
 * @date 03/13/19
 */
void DuckInterpreter::EvaluateArithmeticStatement(const string &a_statement)
{
    // Checking for unary operation
    // Unary Addition Case:
    if(a_statement.find("++") != string::npos)
    {
        // Get the variable
        string temp = a_statement.substr(0,(a_statement.find("++")-1));
        
        // Perform unary addition
        double temp_value;
        if(m_symbolTable.GetVariableValue(temp, temp_value)==false)
        {
            cerr << "Invalid variable: " << temp;
            cerr << "Cannot find value" <<endl;
            exit(1);
        }
        
        // Performing unary addition and recording the value.
        temp_value++;
        m_symbolTable.RecordVariableValue(temp, temp_value);
        
        return;
    }
    
    // Unary Subtraction
    if(a_statement.find("--") != string::npos)
    {
        // Get the variable
        string temp = a_statement.substr(0,(a_statement.find("--")-1));
        
        // Perform unary subtraction
        double temp_value;
        if(m_symbolTable.GetVariableValue(temp, temp_value)==false)
        {
            cerr << "Invalid variable: " << temp;
            cerr << "Cannot find value" <<endl;
            exit(1);
        }
        
        // Performing unary subtraction and recording the value.
        temp_value--;
        m_symbolTable.RecordVariableValue(temp, temp_value);
        
        return;
    }
    
    // If we get here, the expression is not unary.

    // Record the variable that we will be assignning a value.
    int nextPos = 0;
    string resultVariable;
    double placeHolder;
    nextPos = ParseNextElement(a_statement, nextPos, resultVariable, placeHolder);
    assert(!resultVariable.empty());
    
    // Recording and checking for an assignment operator.
    string assigmentOp;
    nextPos = ParseNextElement(a_statement, nextPos, assigmentOp, placeHolder);
    assert(assigmentOp == "=");
    
    // Evaluating the rest of the arithmetic statement.
    double result = EvaluateArithmenticExpression(a_statement, nextPos);
    
    // Record the result.
    m_symbolTable.RecordVariableValue(resultVariable, result);
    
}

/**
 * DuckInterpreter::GetOperatorPrecedence. Method to get the operator precedence.
 * Checks a_operator and returns the precedence of the operator. Used by EvaluateArithmeticExpression.
 * @param a_operator const string String to find out the statement type of.
 * @return int The precedence of the passed operator 'a_operator'
 * @author Salil Maharjan
 * @date 03/13/19
 */
int DuckInterpreter::GetOperatorPrecedence(string a_operator)
{
    if(a_operator == ";")
        return 1;
    if(a_operator == "(" || a_operator == ")")
        return 2;
    if(a_operator == "==" || a_operator == "!=" || a_operator == ">=" || a_operator == "<="
       || a_operator == ">" || a_operator == "<")
        return 3;
    if (a_operator == "+" || a_operator == "-")
        return 4;
    if (a_operator == "*" || a_operator == "/" || a_operator == "%")
        return 5;
    return 0;
}

/**
 * DuckInterpreter::DoOperation. Method to perform arithmetic operations.
 * Does 'a_operation' on 'a_val1' and 'a_val2'. Used by EvaluateArithmeticExpression.
 * @param a_val1 double Holds the first value to perform operation with.
 * @param a_val2 double Holds the second value to perform operation with.
 * @param a_operation string String that holds what operation is to be performed.
 * @return double The result of performing operation "a_operation" on "a_val1" and "a_val2".
 * @author Salil Maharjan
 * @date 03/13/19
 */
double DuckInterpreter::DoOperation(double a_val1, double a_val2, string a_operation)
{
    if(a_operation == "+")
        return a_val1 + a_val2;
    if(a_operation == "-")
        return a_val1 - a_val2;
    if(a_operation == "*")
        return a_val1 * a_val2;
    if(a_operation == "/")
        return a_val1 / a_val2;
    if(a_operation == "%")
        return fmod(a_val1,a_val2);
    if(a_operation == "<")
        return (a_val1 < a_val2);
    if(a_operation == "<=")
        return (a_val1 <= a_val2);
    if(a_operation == ">")
        return (a_val1 > a_val2);
    if(a_operation == ">=")
        return (a_val1 >= a_val2);
    if(a_operation == "==")
        return (a_val1 == a_val2);
    if(a_operation == "!=")
        return (a_val1 != a_val2);
    else
    {
        cerr<< " Cannot do operation" << a_operation << endl;
        exit(1);
    }
}

/**
 * DuckInterpreter::EvaluateArithmenticExpression. Method to perform arithmetic operations.
 * Evaluate an arithmetic expression that is after the assignment operator (=). Used by EvaluateArithmeticStatement.
 * @param a_statement const string Holds the entire arithmetic expression.
 * @param a_nextPos int Holds the value of the first position after the assignment operator to start evaluating the statement.
 * @return double The final result of the arithmetic expression in "a_statement".
 * @see ParseNextElement
 * @see DoOperation
 * @see GetOperatorPrecedence
 * @see SymbolTable::GetVariableValue
 * @see SymbolTable::RecordVariableValue
 * @author Salil Maharjan
 * @date 03/13/19
 */
double DuckInterpreter::EvaluateArithmenticExpression(const string &a_statement, int a_nextPos)
{
    // Position holder for function ParseNextElement.
    string stringValue;
    double numValue;
    
    // Temp variable to store variable value. Used by GetVariableValue.
    double temp;

    // Loop till we reach end of the statement.
    while(a_nextPos!=INT_MAX)
    {
        // Get next element.
        a_nextPos = ParseNextElement(a_statement, a_nextPos, stringValue, numValue);
        
        // Checking for separate semi-colons and unary not operator.
        if(stringValue == ";" || stringValue == "!")
            continue;
        
        // Checking for a string value
        if(numValue==INT_MAX)
        {
            // Checking for opening brace.
            if(stringValue == "(")
                m_operatorStack.push_back(stringValue);
            
            // Checking if it is a variable.
            else if(m_symbolTable.GetVariableValue(stringValue, temp) == true)
                // Pushing the value in temp to the stack.
                m_numberStack.push_back(temp);
            
            // Solving brace if closing brace found.
            else if (stringValue == ")")
            {
                // Performing value1 (t_operator) value2
                while(!m_operatorStack.empty() && m_operatorStack.back() != "(")
                {
                    // Getting values from the stacks.
                    int value2 = m_numberStack.back();
                    m_numberStack.pop_back();
                    
                    int value1 = m_numberStack.back();
                    m_numberStack.pop_back();
                    
                    string t_operator = m_operatorStack.back();
                    m_operatorStack.pop_back();
                    
                    m_numberStack.push_back(DoOperation(value1, value2, t_operator));
                }
                
                // Remove opening brace
                m_operatorStack.pop_back();
            }
            // An operator.
            else
            {
                // While the top operator has same or greater precedence than the current operator.
                // We perform t_operator to the top two values on stack.
                while(!m_operatorStack.empty() && GetOperatorPrecedence(m_operatorStack.back())
                      >= GetOperatorPrecedence(stringValue))
                {
                    int value2 = m_numberStack.back();
                    m_numberStack.pop_back();
                    
                    int value1 = m_numberStack.back();
                    m_numberStack.pop_back();
                    
                    string t_operator = m_operatorStack.back();
                    m_operatorStack.pop_back();
                    
                    m_numberStack.push_back(DoOperation(value1, value2, t_operator));
                }
                // Push current element to stack, which is the result of the operation performed.
                m_operatorStack.push_back(stringValue);
            }
        }
        // Else it is a numeric value
        else
            m_numberStack.push_back(numValue);
    }
    
    // Checking for any remaining operations in the operator stack.
    if(!m_operatorStack.empty())
    {
        // Performing the top operation of m_operatorStack on the top two values of m_numberStack.
        while (m_operatorStack.back() != "\0" && !m_operatorStack.empty())
        {
            int value2 = m_numberStack.back();
            m_numberStack.pop_back();
            
            int value1 = m_numberStack.back();
            m_numberStack.pop_back();
            
            string t_operator = m_operatorStack.back();
            m_operatorStack.pop_back();
            
            m_numberStack.push_back(DoOperation(value1, value2, t_operator));
        }
    }
    
    // The top contains the final result.
    return m_numberStack.back();
}

/**
 * DuckInterpreter::CheckNotOperator. Method to check for not(!) operator in the statement.
 * Checks and counts for not(!) operators in the statement and determines if we need to perform the not(!) operation.
 * Handles cases when there are multiple not operators and determines if the not operation cancels out or not.
 * Removes the ! operation recorded from the statement and returns a bool if we need to perform the operation or not.
 * @param a_statement const string Holds the arithmetic expression.
 * @return bool Whether we need to perform the not(!) operation.
 * @author Salil Maharjan
 * @date 03/13/19
 */
bool DuckInterpreter::CheckNotOperator(string &a_statement)
{
    // Not operator flag.
    bool notOperator = false;
    
    // Checking for unary "!" operator.
    if(a_statement.find("!") != string::npos && (a_statement.find("=") != a_statement.find("!")+1))
    {
        // Counting the instances unary "!" operator
        size_t notOperatorCount = std::count(a_statement.begin(), a_statement.end(),'!');
        
        // Checking for !=
        if(a_statement.find("!=") != string::npos)
            notOperatorCount--;
        
        // Check if we need to perform the logical not operation
        if(notOperatorCount%2 != 0)
            notOperator = true;
        
        // Removing ! operators.
        while(notOperatorCount != 0)
        {
            a_statement.replace(a_statement.find("!"), 1, "");
            notOperatorCount--;
        }
    }
    return notOperator;
}

/**
 * DuckInterpreter::EvaluateIfStatement. Method to evaluate If Statements.
 * Evaluates an if statement to determine if the goto should be executed.
 * @param a_statement const string Holds the if statement.
 * @param a_nextStatement int Current statement position. Used to return the next position of the statement.
 * @return int The next position of the statement that needs to be executed.
 * @see ParseNextElement
 * @see EevaluateIfGotoExpression
 * @see CheckNotOperator
 * @see EvaluateArithmeticExpression
 * @author Salil Maharjan
 * @date 03/13/19
 */
int DuckInterpreter::EvaluateIfStatement(string a_statement, int a_nextStatement)
{
    // Flag to track if we find a logical not(!) operator
    bool notOperator = false;
    
    // Get past of the "if" in the if statement.
    int nextPos = 0;
    string resultString;
    double placeHolder;
    nextPos = ParseNextElement(a_statement, nextPos, resultString, placeHolder);
    assert(resultString == "if");
    
    // Searching from the end, find the goto statement and replace it by ";"  Record
    // the label in the goto.
    string label = EvaluateIfGotoExpression(a_statement);
    int labelLocation = m_statements.GetLabelLocation(label);
    
    // Verify that the label from the goto exists.
    m_statements.GetStatement(labelLocation);
    
    // Checking for Not(!) operators
    notOperator = CheckNotOperator(a_statement);
  
    // Evaluate the remaining arithmentic expression.
    double result = EvaluateArithmenticExpression(a_statement, nextPos);
    
    // Doing the not operation if flagged.
    if(notOperator)
        result = (!result);
    
    // If the result is zero, don't execute the goto.
    if (result == 0)
        return a_nextStatement + 1;
    
    // Return the goto label location.
    return labelLocation;
}

/**
 * DuckInterpreter::EvaluateIfGotoExpression. Method to evaluate goto expressions in an If statement.
 * Searches till the end of the statement for the label and returns it.
 * Then replaces the goto statement with ";"
 * @param a_statement string Holds the if statement.
 * @return string The label associated with the goto statement.
 * @author Salil Maharjan
 * @date 03/13/19
 */
string DuckInterpreter::EvaluateIfGotoExpression(string &a_statement)
{
    // Holds the next string element
    string buffer;
    // Holds the last item - label.
    string end_item;
    
    // Parsing the last element string
    // Not using ParseNextElement to reduce overheads.
    istringstream line(a_statement.c_str(), (int)a_statement.size());
    for(;;)
    {
        buffer="";
        line>>buffer;
        int test = (int)buffer.size();
        if( test == 0 )
            break;
        end_item = buffer;
    }
    
    // Checking for end of statement semi-colon and removing it
    if(end_item.find(";") && end_item.find(";")==end_item.length()-1)
        end_item.pop_back();
    
    // Replacing the goto statement with ";"
    a_statement.replace(a_statement.find(" goto"), a_statement.find(";"), ";");
    
    // Returning the label.
    return end_item;
    
}

/**
 * DuckInterpreter::EvaluateQuotedPrompt. Method to evaluate quoted prompts.
 * Function that evaluates quoted prompts. Used by EvaluatePrintStatement and EvaluateReadStatement to evaluate prompts
 * in quotations. Parameters are passed by reference from the calling function.
 * @param a_statement string Holds the statement with quoted prompt.
 * @param quoted bool flag used to track quoted part of the statement.
 * @param nextPos int Position of the current element in the statement where we need to start evaluating from.
 * @param resultString string Position holder to get string element from ParseNextElement
 * @param placeHolder double Position holder to get numeric element from ParseNextElement.
 * @see ParseNextElement
 * @see EvaluatePrintStatement
 * @see EvaluateReadStatement
 * @see Statement::StandardSpaceFormat
 * @author Salil Maharjan
 * @date 03/13/19
 */
// Evaluates Quoted Prompts. Used by EvaluatePrintStatement and EvaluateReadStatement.
void DuckInterpreter::EvaluateQuotedPrompt(string &a_statement, bool &quoted, int &nextPos, string &resultString, double &placeHolder)
{
    // Checking for the starting quotations:
    if(quoted == false && (resultString.find("\"") != resultString.length()-1))
        cout << resultString.substr(resultString.find("\"")+1,string::npos)<<" ";
    // Case where we have separate multiple quotations.
    else
        // Since we are parsing by whitespaces, it is removed. Printing out the removed delimiter.
        cout<<" ";
    
    // Updating quote flag for the calling function.
    quoted = true;
    
    // Printing the entire quotation until the end quotation is reached.
    while(quoted == true)
    {
        nextPos = ParseNextElement(a_statement, nextPos, resultString, placeHolder);
        
        // If we find the ending quotations
        if(resultString.find("\"") != string::npos)
        {
            quoted = false;
            // Checking if there are elements before ending quotation
            if(resultString.find("\"") != 0 )
                cout<<resultString.substr(0,resultString.find("\""));
            break;
        }
        else
            // Printing whitespace as we parse elements by whitespace and is removed.
            cout << resultString << " ";
    }
    
    // Asserting that nothing is attached to commas in the statement.
    // Spaces would have been added by function Statement::StandardSpaceFormat
    if(resultString.find(",") != string::npos && quoted == false)
        assert(resultString.find(",")==0 || resultString.find(",")==resultString.length()-1);
    
}

/**
 * DuckInterpreter::EvaluatePrintStatement. Method to evaluate Print statements.
 * Evaluates print statements. Asserts for "print" keyword, prints the quoted prompts and variables that are comma separated
 * in the code.
 * @param a_statement string Holds the print statement.
 * @see ParseNextElement
 * @see EvaluateQuotedPrompt
 * @see SymbolTable::GetVariableValue
 * @author Salil Maharjan
 * @date 03/13/19
 */
void DuckInterpreter::EvaluatePrintStatement(string a_statement)
{
    // Flag used to check for quotations.
    bool quoted = false;
    
    // Get past of the "print" in the print statement.
    int nextPos = 0;
    string resultString;
    double placeHolder;
    nextPos = ParseNextElement(a_statement, nextPos, resultString, placeHolder);
    assert(resultString == "print");
    
    // Evaluating the print expression.
    while(nextPos!=INT_MAX)
    {
        nextPos = ParseNextElement(a_statement, nextPos, resultString, placeHolder);
        
        // Checking for separate commas.
        if(resultString.find(",") != string::npos && resultString.length()==1)
            continue;
        
        // Checking for quotations and calling EvaluateQuotedPrompt to print quotations.
        if(resultString.find("\"") != string::npos)
        {
            EvaluateQuotedPrompt(a_statement, quoted, nextPos, resultString, placeHolder);
            continue;
        }
        
        // Checking for variables and printing them out.
        if(m_symbolTable.GetVariableValue(resultString, placeHolder) == true)
            cout << placeHolder;
    }
    
    // End of Print Statement
    cout<<endl;
}

/**
 * DuckInterpreter::EvaluateReadStatement. Method to evaluate Read statements.
 * Evaluates read statements. Asserts for "read" keyword, prints the quoted prompts and gets input from the user
 * for the specified variables.
 * @param a_statement string Holds the read statement.
 * @see ParseNextElement
 * @see EvaluateQuotedPrompt
 * @see SymbolTable::GetVariableValue
 * @see SymbolTable::RecordVariableValue
 * @author Salil Maharjan
 * @date 03/13/19
 */
void DuckInterpreter::EvaluateReadStatement(string a_statement)
{
    // Flag used to check for quotations.
    bool quoted = false;
    
    // Get past of the "read" in the read statement.
    int nextPos = 0;
    string resultString;
    double placeHolder;
    nextPos = ParseNextElement(a_statement, nextPos, resultString, placeHolder);
    assert(resultString == "read");
    
    // Evaluating the entire read statement.
    while(nextPos!=INT_MAX)
    {
        nextPos = ParseNextElement(a_statement, nextPos, resultString, placeHolder);
        
        // Checking for separate commas.
        if(resultString.find(",") != string::npos && resultString.length()==1)
            continue;
        
        // Checking for prompt.
        if(resultString.find("\"") != string::npos)
        {
            EvaluateQuotedPrompt(a_statement, quoted, nextPos, resultString, placeHolder);
            continue;
        }
        // Else it is a variable
        else
        {
            // Checking and removing trailing commas if any.
            if(resultString.find(",") != string::npos && resultString.find(",")==resultString.length()-1 && resultString.length()>1) resultString.pop_back();
            
            // Get input and store it in the variable map.
            double temp;
            scanf("%lf", &temp);
            m_symbolTable.RecordVariableValue(resultString, temp);
        }
    }
    
    //End of Read Statement
    cout<<endl;
}

/**
 * DuckInterpreter::EvaluateGotoStatement. Method to evaluate Goto statements.
 * Evaluates goto statements. At this point, we know that it is a goto statement. The label is the second syntatic element.
 * We parse it and verify that it is a valid label and return the corresponding value of its location.
 * @param a_statement const string Holds the goto statement.
 * @return int Position of the label specified in the goto statement.
 * @see ParseNextElement
 * @see Statement::GetLabelLocation
 * @see Statement::GetStatement
 * @author Salil Maharjan
 * @date 03/13/19
 */
int DuckInterpreter::EvaluateGotoStatement(const string &a_statement)
{
    string label;
    double placeHolder;
    
    // Label is the second syntactic element.
    ParseNextElement(a_statement, 1, label, placeHolder);
    
    int labelLocation = m_statements.GetLabelLocation(label);
    
    // Verify that the label exists.
    m_statements.GetStatement(labelLocation);
    
    return labelLocation;
}
