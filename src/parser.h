#ifndef PARSER_H
#define PARSER_H


Expression* parseExpression(TokenScanner &tokenScanner);


Expression* readExpression(TokenScanner &tokenScanner, int precedence);


Expression* readTerm(TokenScanner &tokenScanner);


int getPrecedence(string token);


#endif // PARSER_H
