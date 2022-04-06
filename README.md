# calc

This is a C implementation of an [operator-precedence parser](https://en.wikipedia.org/wiki/Operator-precedence_parser)
to evaluate mathematical equations.

# Usage

The program executes the mathematical expression passed in via argv or stdin:

```shell
$ calc "1+2"
3

$ echo "1+2" | calc
3
```

Alternatively running the program without arguments enables REPL mode:

```shell
$ calc                                                                                                                                
= 1+2
3
= ans+5
8
```

# Supported Operators

| Operator | Description |
| -------- | ----------- |
| `+`      | Addition
| `-`      | Subtraction / Unary Negative
| `/`      | Division
| `*`      | Multiplication
| `(`      | Left Parenthesis
| `)`      | Right Parenthesis

# Supported Variables

| Name  | Description |
| ----- | ----------- |
| `ans` | Previously Calculated Answer

# Caveats

- No support for non-integer math
- No support for creating variables
- No builtin functions
- No history buffer
