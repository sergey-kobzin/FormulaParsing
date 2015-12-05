/**
 * The "parser.h" implementation.
 */

#include <iostream>
#include <string>
#include "strlib.h"
#include "tokenscanner.h"
#include "error.h"
#include "expression.h"
#include "parser.h"
using namespace std;


/**
 * Parse the expression.
 *
 * @param tokenScanner - The token scanner.
 * @return - The parsed expression.
 */
Expression* parseExpression(TokenScanner &tokenScanner) {
    Expression* expression = readExpression(tokenScanner, 0);
    if (tokenScanner.hasMoreTokens())
        error("Unexpected token \"" + tokenScanner.nextToken() + "\".");
    return expression;
}


/**
 * Read the expression.
 *
 * @param tokenScanner - The token scanner.
 * @param precedence - The expression precedence.
 * @return - The readed expression.
 */
Expression* readExpression(TokenScanner &tokenScanner, int precedence) {
    // Read the term.
    Expression* expression = readTerm(tokenScanner);
    string token;
    while (true) {
        // Get the next token precedence.
        token = tokenScanner.nextToken();
        int tokenPrecedence = getPrecedence(token);
        if (tokenPrecedence <= precedence)
            break;

        // If the current precedence is bigger than next token precedence, read the expression
        // and store it like right operand. Then create the new compound expression.
        Expression* rightOperand = readExpression(tokenScanner, tokenPrecedence);
        expression = new CompoundExpression(token, expression, rightOperand);
    }
    tokenScanner.saveToken(token);
    return expression;
}


/**
 * Read the term. Term is the subspecies of the expression.
 *
 * @param tokenScanner - The token scanner.
 * @return - The readed term.
 */
Expression* readTerm(TokenScanner &tokenScanner) {
    // Get the next token type.
    string token = tokenScanner.nextToken();
    TokenType tokenType = tokenScanner.getTokenType(token);

    // If token type is the word, it can be function or identifier.
    if (tokenType == WORD) {
        // If the current token is the function, read the expression and store it like function operand.
        // Then create the new function expression.
        if ((token == "sin") || (token == "cos") || (token == "tan") ||
                (token == "sqrt") || (token == "exp") || (token == "log") || (token == "abs")) {
            Expression* operand = readExpression(tokenScanner, 5);
            return new FunctionExpression(token, operand);
        }

        // If the current token is word and is not the function, it can be only identifier.
        // Create the new identifier expression.
        return new IdentifierExpression(token);
    }

    // If token type is the number, it can be only constant. Create the new constant expression.
    if (tokenType == NUMBER)
        return new ConstantExpression(stringToDouble(token));

    // If token is "(", read the expression.
    if (token != "(")
        error("Illegal term in expression.");
    Expression* expression = readExpression(tokenScanner, 0);

    // If parentheses are unbalansed.
    if (tokenScanner.nextToken() != ")")
        error("Unbalanced parentheses in expression.");
    return expression;
}


/**
 * Get the expression precedence.
 *
 * @param token - The token.
 * @return - The expression precedence.
 */
int getPrecedence(string token) {
    if (token == "=")
        return 1;
    if ((token == "+") || (token == "-"))
        return 2;
    if ((token == "*") || (token == "/"))
        return 3;
    if (token == "^")
        return 4;
    return 0;
}
