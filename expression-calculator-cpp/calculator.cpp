// Expression calculator: converts infix math to postfix (shunting-yard algorithm),
// then evaluates it with a stack.
#include <cctype>
#include <cmath>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

struct Token {
    bool isNumber;
    double value;   // used when isNumber is true
    char op;        // used otherwise; 'u' means unary minus
};

int precedence(char op) {
    switch (op) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case 'u':           return 4;
        case '^':           return 5;
    }
    return 0;
}

bool isRightAssociative(char op) { return op == '^' || op == 'u'; }

// Converts an infix expression string to a postfix token list.
vector<Token> toPostfix(const string& s) {
    vector<Token> output;
    stack<char> ops;
    bool expectOperand = true;   // true at the start and after an operator or '('

    for (size_t i = 0; i < s.size();) {
        char c = s[i];

        if (isspace(static_cast<unsigned char>(c))) { i++; continue; }

        if (isdigit(static_cast<unsigned char>(c)) || c == '.') {
            size_t used = 0;
            double v = stod(s.substr(i), &used);
            output.push_back({true, v, 0});
            i += used;
            expectOperand = false;
            continue;
        }

        if (c == '(') {
            ops.push(c);
            i++;
            expectOperand = true;
            continue;
        }

        if (c == ')') {
            while (!ops.empty() && ops.top() != '(') {
                output.push_back({false, 0, ops.top()});
                ops.pop();
            }
            if (ops.empty()) throw runtime_error("Mismatched parentheses");
            ops.pop();
            i++;
            expectOperand = false;
            continue;
        }

        if (string("+-*/^").find(c) != string::npos) {
            if (expectOperand) {
                if (c == '-') {            // unary minus, e.g. -5 or 2*-3
                    ops.push('u');
                    i++;
                    continue;
                }
                throw runtime_error(string("Unexpected operator '") + c + "'");
            }
            while (!ops.empty() && ops.top() != '(' &&
                   (precedence(ops.top()) > precedence(c) ||
                    (precedence(ops.top()) == precedence(c) && !isRightAssociative(c)))) {
                output.push_back({false, 0, ops.top()});
                ops.pop();
            }
            ops.push(c);
            i++;
            expectOperand = true;
            continue;
        }

        throw runtime_error(string("Invalid character '") + c + "'");
    }

    if (expectOperand) throw runtime_error("Expression ended unexpectedly");

    while (!ops.empty()) {
        if (ops.top() == '(') throw runtime_error("Mismatched parentheses");
        output.push_back({false, 0, ops.top()});
        ops.pop();
    }
    return output;
}

// Evaluates a postfix token list using a stack of numbers.
double evaluate(const vector<Token>& postfix) {
    stack<double> values;

    for (const Token& t : postfix) {
        if (t.isNumber) {
            values.push(t.value);
            continue;
        }
        if (t.op == 'u') {
            double a = values.top();
            values.pop();
            values.push(-a);
            continue;
        }
        double b = values.top(); values.pop();
        double a = values.top(); values.pop();
        switch (t.op) {
            case '+': values.push(a + b); break;
            case '-': values.push(a - b); break;
            case '*': values.push(a * b); break;
            case '/':
                if (b == 0) throw runtime_error("Division by zero");
                values.push(a / b);
                break;
            case '^': values.push(pow(a, b)); break;
        }
    }
    return values.top();
}

int main() {
    cout << "Expression Calculator\n";
    cout << "Supports + - * / ^ and parentheses. Type 'quit' to exit.\n";

    string line;
    while (true) {
        cout << "> ";
        if (!getline(cin, line) || line == "quit") break;
        if (line.find_first_not_of(" \t") == string::npos) continue;

        try {
            cout << evaluate(toPostfix(line)) << "\n";
        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
    return 0;
}
