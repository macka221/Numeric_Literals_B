#include <iostream>
#include <string>
#include <fstream>
#define MAX_SIZE 50


using namespace std;

bool initialState(string);
bool state1(string);
bool state2(string);
bool state3(string, int);
bool state4(string, int);
bool state5(string, int);
bool state6(string, int);
bool state7(string, int);
bool state8(string, int);
bool stateG() { return false; }; // Garbage State of the machine

int main()
{
    string fileName;
    ifstream currentFile;
    ofstream outputFile;

    cout << "Welcome to the Literals Program. This program reads\n"
         << "data from the specified .txt file and determines if\n"
         << "each line is a correct literal and prints it to a new\n"
         << "file 'Numeric_Literal_Output.txt' as well as the screen.\n\n"
         << "Enter the file name(w/out .txt)-> ";
    getline(cin, fileName);

    fileName = fileName + ".txt";

    currentFile.open( fileName ), outputFile.open( "Numeric_Literal_Output.txt" );

    while ( true )
    {
        string temp;

        if ( currentFile.eof() )
            break;

        getline(currentFile, temp);

        bool safe = initialState(temp);


        !safe ? temp = temp + "  is Not a numeric literal"
            : temp = temp + "  is a numeric literal";

        outputFile << temp << endl;
        cout << temp << endl;
    }

    outputFile.close(), currentFile.close();

    return 0;
}

//================================================================
//  Args:
//      -literal: input from file
//
//  Starting state of DFA. This will handle the initial checks of
//  literal. It will check the size of literal, and the first char
//  in the literal to make sure it's accepted.
//================================================================

bool initialState(string literal)
{
    char start = literal[0];
    if ( literal.size() > MAX_SIZE )
        return stateG();

    switch(start)
    {
    case '.':
        return state3(literal, 0);
    case '+':
    case '-':
        return state2(literal);
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return state1(literal);
    default:
        return stateG();
    }
}

//================================================================
//  Args:
//      -literal: input from file
//
//  If the first character in literal is an integer then it is
//  passed to this state. This state is an acceptable state. The
//  input will move to S8 if there is a decimal value, S5 if there
//  is an e/E and loop if there is another integer value, garbage
//  otherwise. If it doesn't change states then it returns true.
//================================================================

bool state1(string literal)
{
    if (literal.size() > 1)
    {
        for (int i = 1; i < literal.size(); i++)
        {
            char input = literal[i];

            switch(input)
            {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                break;
            case 'e':
            case 'E':
            {
                return state5(literal, i);
            }
            case '.':
            {
                return state8(literal, i);
            }
            default:
            {
                return stateG();
            }
            }
        }
    }
    else
    {
        char lit = literal[0];

        switch(lit)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return !stateG();
        default:
            return stateG();
        }
    }
    return !stateG();
}

//================================================================
//  Args:
//      -literal: input from file
//
//  If the first character is a sign then the literal is passed to
//  this state. This state will pass to S1 if the next value in the
//  literal is an integer, if a decimal will pass to S3, otherwise
//  it goes to a garbage state.
//================================================================

bool state2(string literal)
{
    char start = literal[1];

    switch(start)
    {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return state1(literal);
    case '.':
        return state3(literal, 1);
    default:
        return stateG();
    }
}

//================================================================
//  Args:
//      -literal: input from file
//      -lastPos: last position read.
//
//  This state receives the literal from either S2 or the initial
//  state. This will only accept integer values and pass the
//  literal to S4, otherwise it will send the literal to garbage.
//================================================================

bool state3(string literal, int lastPos)
{
    char pos = literal[lastPos + 1];

    switch(pos)
    {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return state4(literal, lastPos + 1);
    default:
        return stateG();
    }
}

//================================================================
//  Args:
//      -literal: input from file
//      -lastPos: last position read
//
//  This only accepts literals from S3, and this is an accepted
//  state. This state will loop on itself if an integer value is
//  read, pass to S5 if an e/E is read, otherwise it will go to a
//  garbage state.
//================================================================

bool state4(string literal, int lastPos)
{
    for (int i = lastPos; lastPos < literal.size(); i++)
    {
        char pos = literal[i];

        switch(pos)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
            if ( i + 1 == literal.size() )
                return !stateG();
            break;
        }
        case 'e':
        case 'E':
            return state5(literal, i);
        default:
            return stateG();
        }
    }
	return 0;
}

//================================================================
//  Args:
//      -literal: input from file
//      -lastPos: last position read
//
//  This receives data from S1 and S4. This state will pass to S6
//  if a +/- is read, S7 if an integer is read, and to garbage for
//  any other input.
//================================================================

bool state5(string literal, int lastPos)
{
    char nextPos = literal[lastPos + 1];

    switch(nextPos)
    {
    case '+':
    case '-':
        return state6(literal, lastPos + 1);
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return state7(literal, lastPos + 1);
    default:
        return stateG();
    }
}

//================================================================
//  Args:
//      -literal: input from file
//      -lastPos: last position read
//
//  This receives data from S5. It will only accept an integer as
//  acceptable input and then pass to S7. Any other input is sent
//  to the garbage state.
//================================================================

bool state6(string literal, int lastPos)
{
    char nextPos = literal[lastPos + 1];

    switch(nextPos)
    {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return  state7(literal, lastPos + 1);
    default:
        return stateG();
    }
}

//================================================================
//  Args:
//      -literal: input from file
//      -lastPos: last position read
//
//  This is the last acceptable state, and it receives data from
//  S6 and S5. This state will loop on all integers and will go to
//  a garbage state otherwise.
//================================================================

bool state7(string literal, int lastPos)
{
    bool safe;

    for (int i = lastPos; i < literal.size(); i++)
    {
        char pos = literal[i];

        switch(pos)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            {
                safe = true;
                break;
            }
        default:
            return stateG();
        }
    }
    return safe;
}

//================================================================
//  Args:
//      -literal: input from file
//      -lastPos: last position read
//
//  This is an acceptable state that receives data from S1 only if
//  a decimal is read directly after an integer value. This state
//  loops to itself if an integer value is read, passes to S5 if
//  an e/E is read and sent to garbage for any other input.
//================================================================

bool state8(string literal, int lastPos)
{
    for( int i = lastPos + 1; i < literal.size(); i++)
    {
        char pos = literal[i];

        switch(pos)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
                break;
        case 'e':
        case 'E':
            return state5(literal, i);
        default:
            return stateG();
        }
    }

    return !stateG();
}
