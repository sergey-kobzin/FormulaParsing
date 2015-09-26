#include <iostream>
#include <string>
#include <cmath>
#include "console.h"
#include "gwindow.h"
#include "gobjects.h"
#include "simpio.h"
#include "tokenscanner.h"
#include "vector.h"
using namespace std;

string getFormula() {
    // Input function formula. Automaticaly convert all chars to lower case. Empty line is not ok.
    string input;
    while (true) {
        input = toLowerCase(getLine("Enter the function formula: y = "));
        if (input.length() != 0)
            break;
        cout << "Illegal function format. Try again." << endl;
    }

    // Delete all spaces from the formula.
    string result = "";
    for (int i = 0; i < input.length(); i++)
        if (input[i] != ' ')
            result += input[i];
    return result;
}

/**
 * Convert all unary minuses to the "0-" expression.
 *
 * @param formula
 * @return
 */
string deleteUnaryMinuses(string formula) {
    string result;
    if (formula[0] == '-')
        result = "0-";
    else
        result = formula[0];
    for (int i = 1; i < formula.length(); i++)
        if ((formula[i] == '-') && (formula[i - 1] == '('))
            result += "0-";
        else
            result += formula[i];
    return result;
}

pair<double, double> getScope() {
    pair<double, double> result;
    result.first = getReal("Enter the left arguments scope value: x min = ");
    result.second = getReal("Enter the right arguments scope value: x max = ");
    return result;
}

int getScale() {
    int result = getInteger("Enter the graphics scale (pixels per unit): scale = ");
    return result;
}

Vector<pair<string, int> > defineOperands(string formula) {
    Vector<pair<string, int> > result;
    pair<string, int> operand;
    int priorityFactor = 1;
    TokenScanner tokenScanner(formula);
    tokenScanner.scanNumbers();
    while (tokenScanner.hasMoreTokens()) {
        string token = tokenScanner.nextToken();
        if (token == "(")
            priorityFactor *= 10;
        else if (token == ")") {
            priorityFactor /= 10;
            if (priorityFactor == 0) {
                result.clear();
                break;
            }
        }
        else {
            operand.first = token;
            if ((isdigit(token[0])) || (token == "x"))
                operand.second = 0;
            else if ((token == "+") || (token == "-"))
                operand.second = priorityFactor;
            else if ((token == "*") || (token == "/"))
                operand.second = 2 * priorityFactor;
            else if ((token == "^"))
                operand.second = 3 * priorityFactor;
            else if ((token == "sin") || (token == "cos") || (token == "tan") || (token == "cotan") ||
                     (token == "asin") || (token == "acos") || (token == "atan") || (token == "acotan") ||
                     (token == "exp") || (token == "log") || (token == "sqrt") || (token == "abs"))
                 operand.second = 4 * priorityFactor;
            else {
                result.clear();
                break;
            }
            result.push_back(operand);
        }
    }
    return result;
}

Vector<pair<string, int> > substituteArgument(Vector<pair<string, int> > operands, double argument) {
    for (int i = 0; i < operands.size(); i++)
        if (operands[i].first == "x")
            operands[i].first = doubleToString(argument);
    return operands;
}

int getMaxPriorityIndex(Vector<pair<string, int> > operands) {
    int result = 0;
    for (int i = 1; i < operands.size(); i++)
        if (operands[i].second > operands[result].second)
            result = i;
    return result;
}

Vector<pair<string, int> > normalizeMaxPriority(Vector<pair<string, int> > operands, int maxPriorityIndex) {
    while (operands[maxPriorityIndex].second > 9)
        operands[maxPriorityIndex].second /= 10;
    return operands;
}

Vector<pair<string, int> > calculateMaxPriorityOperand(Vector<pair<string, int> > operands, int maxPriorityIndex) {
    // Operands with priority form 1 to 3 require two operands with priority 0 (left and right).
    if ((operands[maxPriorityIndex].second >= 1) && (operands[maxPriorityIndex].second <= 3)) {
        if (operands[maxPriorityIndex].first == "+")
            operands[maxPriorityIndex].first =
                    doubleToString(stringToDouble(operands[maxPriorityIndex - 1].first) +
                    stringToDouble(operands[maxPriorityIndex + 1].first));
        if (operands[maxPriorityIndex].first == "-")
            operands[maxPriorityIndex].first =
                    doubleToString(stringToDouble(operands[maxPriorityIndex - 1].first) -
                    stringToDouble(operands[maxPriorityIndex + 1].first));
        if (operands[maxPriorityIndex].first == "*")
            operands[maxPriorityIndex].first =
                    doubleToString(stringToDouble(operands[maxPriorityIndex - 1].first) *
                    stringToDouble(operands[maxPriorityIndex + 1].first));
        if (operands[maxPriorityIndex].first == "/")
            operands[maxPriorityIndex].first =
                    doubleToString(stringToDouble(operands[maxPriorityIndex - 1].first) /
                    stringToDouble(operands[maxPriorityIndex + 1].first));
        if (operands[maxPriorityIndex].first == "^")
            operands[maxPriorityIndex].first =
                    doubleToString(pow(stringToDouble(operands[maxPriorityIndex - 1].first),
                    stringToDouble(operands[maxPriorityIndex + 1].first)));
        operands[maxPriorityIndex].second = 0;
        operands.remove(maxPriorityIndex + 1);
        operands.remove(maxPriorityIndex - 1);
    }
    // Operands with priority 4 require one operands with priority 0 (right only).
    else if (operands[maxPriorityIndex].second == 4) {
        if (operands[maxPriorityIndex].first == "sin")
            operands[maxPriorityIndex].first =
                    doubleToString(sin(stringToDouble(operands[maxPriorityIndex + 1].first)));
        if (operands[maxPriorityIndex].first == "cos")
            operands[maxPriorityIndex].first =
                    doubleToString(cos(stringToDouble(operands[maxPriorityIndex + 1].first)));
        if (operands[maxPriorityIndex].first == "tan")
            operands[maxPriorityIndex].first =
                    doubleToString(tan(stringToDouble(operands[maxPriorityIndex + 1].first)));
        if (operands[maxPriorityIndex].first == "cotan")
            operands[maxPriorityIndex].first =
                    doubleToString(1 / tan(stringToDouble(operands[maxPriorityIndex + 1].first)));
        if (operands[maxPriorityIndex].first == "asin")
            operands[maxPriorityIndex].first =
                    doubleToString(asin(stringToDouble(operands[maxPriorityIndex + 1].first)));
        if (operands[maxPriorityIndex].first == "acos")
            operands[maxPriorityIndex].first =
                    doubleToString(acos(stringToDouble(operands[maxPriorityIndex + 1].first)));
        if (operands[maxPriorityIndex].first == "atan")
            operands[maxPriorityIndex].first =
                    doubleToString(atan(stringToDouble(operands[maxPriorityIndex + 1].first)));
        if (operands[maxPriorityIndex].first == "acotan")
            operands[maxPriorityIndex].first =
                    doubleToString(1 / atan(stringToDouble(operands[maxPriorityIndex + 1].first)));
        if (operands[maxPriorityIndex].first == "exp")
            operands[maxPriorityIndex].first =
                    doubleToString(exp(stringToDouble(operands[maxPriorityIndex + 1].first)));
        if (operands[maxPriorityIndex].first == "log")
            operands[maxPriorityIndex].first =
                    doubleToString(log(stringToDouble(operands[maxPriorityIndex + 1].first)));
        if (operands[maxPriorityIndex].first == "sqrt")
            operands[maxPriorityIndex].first =
                    doubleToString(sqrt(stringToDouble(operands[maxPriorityIndex + 1].first)));
        if (operands[maxPriorityIndex].first == "abs")
            operands[maxPriorityIndex].first =
                    doubleToString(abs(stringToDouble(operands[maxPriorityIndex + 1].first)));
        operands[maxPriorityIndex].second = 0;
        operands.remove(maxPriorityIndex + 1);
    }
    return operands;
}

double calculateFunction(Vector<pair<string, int> > operands, double argument) {
    operands = substituteArgument(operands, argument);
    while (operands.size() > 1) {
        int maxPriorityIndex = getMaxPriorityIndex(operands);
        operands = normalizeMaxPriority(operands, maxPriorityIndex);
        operands = calculateMaxPriorityOperand(operands, maxPriorityIndex);
    }
    double result = stringToDouble(operands[0].first);
    return result;
}

pair<double, double> drawCoordinateSystem(GWindow &graphicsWindow, int scale) {
    pair<double, double> result;
    result.first = graphicsWindow.getWidth() / 2;
    result.second = graphicsWindow.getHeight() / 2;
    graphicsWindow.drawLine(result.first, 0, result.first, result.second * 2);
    graphicsWindow.drawLine(0, result.second, result.first * 2, result.second);
    for (int x = -result.first / scale + 1; x <= result.first / scale - 1; x++)
        graphicsWindow.drawLine(result.first + x * scale, result.second - 5,
                                result.first + x * scale, result.second + 5);
    for (int y = -result.second / scale + 1; y <= result.second / scale - 1; y++)
        graphicsWindow.drawLine(result.first - 5, result.second + y * scale,
                                result.first + 5, result.second + y * scale);
    GLabel* xMark = new GLabel("x", result.first * 2 - 20, result.second + 15);
    graphicsWindow.add(xMark);
    GLabel* yMark = new GLabel("y", result.first + 10, 20);
    graphicsWindow.add(yMark);
    return result;
}

void drawGraph(Vector<pair<string, int> > operands, pair<double, double> scope, int scale, string formula) {
    GWindow graphicsWindow(800, 600);
    graphicsWindow.setColor("BLACK");
    pair<double, double> center = drawCoordinateSystem(graphicsWindow, scale);
    GLabel* formulaMark = new GLabel("y = " + formula, 20, 30);
    formulaMark->setFont("Courier New");
    graphicsWindow.add(formulaMark);
    graphicsWindow.setColor("RED");
    double prewX = scope.first;
    double prewY = calculateFunction(operands, prewX);
    for (double x = scope.first + 0.01; x <= scope.second; x += 0.01) {
        double y = calculateFunction(operands, x);
        graphicsWindow.drawLine(center.first + prewX * scale, center.second - prewY * scale,
                                center.first + x * scale, center.second - y * scale);
        prewX = x;
        prewY = y;
    }
}

int main() {
    while (true) {
        string formula = getFormula();
        string formulaWithoutUnaryMinuses = deleteUnaryMinuses(formula);
        Vector<pair<string, int> > operands = defineOperands(formulaWithoutUnaryMinuses);
        if (operands.isEmpty()) {
            cout << "Illegal function format. " <<
                    "Function contain wrong operands or operations or parentheses are not balanced." << endl;
            return 1;
        }
        pair<double, double> scope = getScope();
        int scale = getScale();
        drawGraph(operands, scope, scale, formula);
        if (!getYesOrNo("Do you want build ones more functions graphic? (y/n): "))
            break;
    }
	return 0;
}
