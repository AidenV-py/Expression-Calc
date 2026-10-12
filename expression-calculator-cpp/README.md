# Expression Calculator (C++)

A command-line calculator that parses math expressions with correct order of operations.

## Concepts used
- Stack data structure (`std::stack`)
- Shunting-yard algorithm to convert infix to postfix notation
- Operator precedence and associativity
- Exception handling for bad input (mismatched parentheses, division by zero)

## Build and run
```bash
g++ -std=c++17 -Wall -o calculator calculator.cpp
./calculator
```

## Examples
```
> 2 + 3 * 4
14
> (2 + 3) * 4
20
> 2 ^ 3 ^ 2
512
> -5 + 10 / 4
-2.5
```
