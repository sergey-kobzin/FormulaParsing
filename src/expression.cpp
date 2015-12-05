/**
 * The "expression.h" implementation.
 */

#include <string>
#include "strlib.h"
#include "cmath"
#include "error.h"
#include "expression.h"
using namespace std;


/**
 * The expression constructor.
 */
Expression::Expression() {
}

/**
 * The expression destructor.
 */
Expression::~Expression() {
}

/**
 * Return the expression type.
 *
 * @return - The expression type.
 */
ExpressionType Expression::getType() {
    return CONSTANT;
}

/**
 * Calculate the expression.
 *
 * @param identifiersTable - The identifiers table.
 * @return - The result of the expression calculation.
 */
double Expression::calculate(IdentifiersTable &identifiersTable) {
    return 0;
}


/**
 * Convert the expression to the string.
 *
 * @return - The result of the expression convertion.
 */
string Expression::toString() {
    return "";
}


/**
 * Get the expression value.
 *
 * @return - The expression value.
 */
double Expression::getValue() {
    error("getValue: Illegal expression type.");
    return 0;
}


/**
 * Get the expression name.
 *
 * @return - The expression name.
 */
string Expression::getName() {
    error("getName: Illegal expression type.");
    return "";
}


/**
 * Get the expression operation.
 *
 * @return - The expression operation.
 */
string Expression::getOperation() {
    error("getOperation: Illegal expression type.");
    return "";
}


/**
 * Get the expression left operand.
 *
 * @return - The expression left operand.
 */
Expression *Expression::getLeftOperand() {
    error("getLeftOperand: Illegal expression type.");
    return NULL;
}


/**
 * Get the expression right operand.
 *
 * @return - The expression right operand.
 */
Expression* Expression::getRightOperand() {
    error("getRightOperand: Illegal expression type.");
    return NULL;
}


/**
 * The constant expression constructor.
 */
ConstantExpression::ConstantExpression(double value) {
    this->value = value;
}


/**
 * Get the constant expression type.
 *
 * @return - Always CONSTANT.
 */
ExpressionType ConstantExpression::getType() {
    return CONSTANT;
}

/**
 * Calculate the constant expression.
 *
 * @param identifiersTable - The identifiers table.
 * @return - The result of the constant expression calculation is its value.
 */
double ConstantExpression::calculate(IdentifiersTable &identifiersTable) {
    return value;
}


/**
 * Convert the constant expression to the string.
 *
 * @return - The string form of the constant expression value.
 */
string ConstantExpression::toString() {
    return doubleToString(value);
}


/**
 * Get the constant expression value.
 *
 * @return - The constant expression value.
 */
double ConstantExpression::getValue() {
    return value;
}


/**
 * The identifier expression constructor.
 */
IdentifierExpression::IdentifierExpression(string name) {
    this->name = name;
}


/**
 * Get the identifier expression type.
 *
 * @return - Always IDENTIFIER.
 */
ExpressionType IdentifierExpression::getType() {
    return IDENTIFIER;
}


/**
 * Calculate the identifier expression.
 *
 * @param identifiersTable - The identifiers table.
 * @return - The result of the identifier expression calculation is its value if it is presented in the
 *           identifiers table.
 */
double IdentifierExpression::calculate(IdentifiersTable &identifiersTable) {
    // If the identifier name defined in the identifiers table return its value.
    if (!identifiersTable.isDefine(name))
        error(name + "is undefined.");
    return identifiersTable.getValue(name);
}


/**
 * Convert the identifier expression to the string.
 *
 * @return - The identifier name.
 */
string IdentifierExpression::toString() {
    return name;
}


/**
 * Get the identifier expression name.
 *
 * @return - The identifier name.
 */
string IdentifierExpression::getName() {
    return name;
}


/**
 * The compound expression constructor.
 *
 * @param operation - The operation type.
 * @param leftOperand - The left operand.
 * @param rightOperand - The right operand.
 */
CompoundExpression::CompoundExpression(string operation, Expression *leftOperand, Expression *rightOperand) {
    this->operation = operation;
    this->leftOperand = leftOperand;
    this->rightOperand = rightOperand;
}


/**
 * The compound expression destructor.
 */
CompoundExpression::~CompoundExpression() {
    delete leftOperand;
    delete rightOperand;
}


/**
 * Get the compound expression type.
 *
 * @return - Always COMPOUND.
 */
ExpressionType CompoundExpression::getType() {
    return COMPOUND;
}


/**
 * Calculate the compound expression.
 *
 * @param identifiersTable - The identifiers table.
 * @return - The result of the compound expression calculation.
 */
double CompoundExpression::calculate(IdentifiersTable &identifiersTable) {
    // Calculate the right operand of the compound expression.
    double right = rightOperand->calculate(identifiersTable);

    // If the operation type is "=" change the value of the identifier in the identifiers table.
    if (operation == "=") {
        identifiersTable.setValue(leftOperand->getName(), right);
        return right;
    }

    // Calculate the left operand of the compound expression.
    double left = leftOperand->calculate(identifiersTable);

    // Return the result of the compound expression calculation depending the operation type.
    if (operation == "+")
        return left + right;
    if (operation == "-")
        return left - right;
    if (operation == "*")
        return left * right;
    if (operation == "/") {
        if (right == 0)
            error("Division by 0.");
        return left / right;
    }
    if (operation == "^")
        return pow(left, right);

    // Illegal operation type.
    error("Illegal operation in expression.");
    return 0;
}


/**
 * Convert the compound expression to the string.
 *
 * @return - The string form of the compound expression.
 */
string CompoundExpression::toString() {
    return '(' + leftOperand->toString() + ' ' + operation + ' ' + rightOperand->toString() + ')';
}


/**
 * Get the operation type of the compound expression.
 *
 * @return - The operation type.
 */
string CompoundExpression::getOperation() {
    return operation;
}


/**
 * Get the left operand of the compound expression.
 *
 * @return - The left operand.
 */
Expression* CompoundExpression::getLeftOperand() {
    return leftOperand;
}


/**
 * Get the right operand of the compound expression.
 *
 * @return - The right operand.
 */
Expression* CompoundExpression::getRightOperand() {
    return rightOperand;
}


/**
 * The function expression constructor.
 *
 * @param function - The function type.
 * @param operand - The operand.
 */
FunctionExpression::FunctionExpression(string function, Expression *operand) {
    this->function = function;
    this->operand = operand;
}


/**
 * The function expression destructor.
 */
FunctionExpression::~FunctionExpression() {
    delete operand;
}


/**
 * Get the function expression type.
 *
 * @return - Always FUNCTION.
 */
ExpressionType FunctionExpression::getType() {
    return FUNCTION;
}


/**
 * Calculate the function expression.
 *
 * @param identifiersTable - The identifiers table.
 * @return - The result of the function expression calculation.
 */
double FunctionExpression::calculate(IdentifiersTable &identifiersTable) {
    // Calculate the argument of the function expression.
    double argument = operand->calculate(identifiersTable);

    // Return the result of the function expression calculation depending the function type.
    if (function == "sin")
        return sin(argument);
    if (function == "cos")
        return cos(argument);
    if (function == "tan")
        return tan(argument);
    if (function == "sqrt")
        return sqrt(argument);
    if (function == "exp")
        return exp(argument);
    if (function == "log")
        return log(argument);
    if (function == "abs")
        return abs(argument);

    // Illegal function type.
    error("Illegal function in expression.");
    return 0;
}


/**
 * Convert the function expression to the string.
 *
 * @return - The string form of the function expression.
 */
string FunctionExpression::toString() {
    return '(' + function + '(' + operand->toString() + ')' + ')';
}


/**
 * Get the function type of the function expression.
 *
 * @return - The function type.
 */
string FunctionExpression::getFunction() {
    return function;
}


/**
 * Get the operand of the function expression.
 *
 * @return - The operand.
 */
Expression* FunctionExpression::getOperand() {
    return operand;
}


/**
 * Set the identifier value in the identifiers table.
 *
 * @param name - The identifier name.
 * @param value - The identifier value.
 */
void IdentifiersTable::setValue(string name, double value) {
    identifiersTable.put(name, value);
}


/**
 * Get the identifier value in the identifiers table.
 *
 * @param name - The identifier name.
 * @return - The identifier value.
 */
double IdentifiersTable::getValue(string name) {
    return identifiersTable.get(name);
}


/**
 * Define contains the identifier in the identifiers table.
 *
 * @param name - The identifiers table.
 * @return - True if the identifier name contain in the identifiers table or false in other case.
 */
bool IdentifiersTable::isDefine(string name) {
    return identifiersTable.containsKey(name);
}
