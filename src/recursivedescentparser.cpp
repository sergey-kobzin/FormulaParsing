#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "tokenscanner.h"
#include "error.h"
#include "expression.h"
#include "parser.h"
using namespace std;


/**
 * Prepare the inputed line to the right format.
 *
 * @param line - The inputed line.
 * @return - The prepared line.
 */
string prepareLine(string line) {
    // Convert all chars of the inputed line to the lower case.
    line = toLowerCase(line);
    // Delete all white spaces from the line.
    string lineWithoutSpaces = "";
        for (int i = 0; i < line.length(); i++)
            if (line[i] != ' ')
                lineWithoutSpaces += line[i];
    // Delete all unary minuses from the line.
    string::size_type pos;
    while (true) {
        pos = lineWithoutSpaces.find("=-");
        if (pos == string::npos)
            break;
        lineWithoutSpaces = lineWithoutSpaces.replace(pos, 2, "=0-");
    }
    while (true) {
        pos = lineWithoutSpaces.find("(-");
        if (pos == string::npos)
            break;
        lineWithoutSpaces = lineWithoutSpaces.replace(pos, 2, "(0-");
    }
    return lineWithoutSpaces;
}

int main() {
    IdentifiersTable identifiersTable;
    TokenScanner tokenScanner;
    tokenScanner.scanNumbers();
    while (true) {
        string line = prepareLine(getLine("Enter the expression: "));
        if (line == "quit")
            break;
        tokenScanner.setInput(line);
        Expression* expression = parseExpression(tokenScanner);
        double value = expression->calculate(identifiersTable);
        cout << value << endl;
        if (expression != NULL)
            delete expression;
    }
    return 0;
}
