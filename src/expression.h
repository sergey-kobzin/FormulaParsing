#ifndef EXPRESSION_H
#define EXPRESSION_H


#include <string>
#include "map.h"
#include "tokenscanner.h"
using namespace std;


// IdentifiersTable class prototype.
class IdentifiersTable;


/**
 * Enumeration of the expression types.
 */
enum ExpressionType {
    CONSTANT,
    IDENTIFIER,
    COMPOUND,
    FUNCTION
};


/**
 * The Expression class.
 */
class Expression {
    public:
        // Common methods for all subclasses of the Expression class.
        Expression();
        virtual ~Expression();
        virtual ExpressionType getType();
        virtual double calculate(IdentifiersTable &identifiersTable);
        virtual string toString();

        // Specific methods for all subclasses of the Expression class.
        virtual double getValue();
        virtual string getName();
        virtual string getOperation();
        virtual Expression* getLeftOperand();
        virtual Expression* getRightOperand();
};


/**
 * The ConstantExpression class
 */
class ConstantExpression : public Expression {
    private:
        double value;
    public:
        ConstantExpression(double value);
        ExpressionType getType();
        double calculate(IdentifiersTable &identifiersTable);
        string toString();
        double getValue();
};


/**
 * The IdentifierExpression class
 */
class IdentifierExpression : public Expression {
    private:
        string name;
    public:
        IdentifierExpression(string name);
        ExpressionType getType();
        double calculate(IdentifiersTable &identifiesTable);
        string toString();
        string getName();
};


/**
 * The CompoundExpression class
 */
class CompoundExpression : public Expression {
    private:
        string operation;
        Expression* leftOperand;
        Expression* rightOperand;
    public:
        CompoundExpression(string operation, Expression* leftOperand, Expression* rightOperand);
        ~CompoundExpression();
        ExpressionType getType();
        double calculate(IdentifiersTable &identifiersTable);
        string toString();
        string getOperation();
        Expression* getLeftOperand();
        Expression* getRightOperand();
};


/**
 * The FunctionExpression class
 */
class FunctionExpression : public Expression {
    private:
        string function;
        Expression* operand;
    public:
        FunctionExpression(string function, Expression* operand);
        ~FunctionExpression();
        ExpressionType getType();
        double calculate(IdentifiersTable &identifiersTable);
        string toString();
        string getFunction();
        Expression* getOperand();
};


/**
 * The IdentifiersTable class
 */
class IdentifiersTable {
    private:
        Map<string, double> identifiersTable;
    public:
        void setValue(string name, double value);
        double getValue(string name);
        bool isDefine(string name);
};


#endif // EXPRESSION_H
