// Copyright (C) 2022  Nicole Alassandro

// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.

// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.

// You should have received a copy of the GNU General Public License along
// with this program.  If not, see <http://www.gnu.org/licenses/>.

struct string
{
    char * start;
    char * end;
};

enum operator
{
    SUBTRACTION,
    ADDITION,
    DIVISION,
    MULTIPLICATION,
    LPAREN,
    RPAREN,
};

enum tokentype
{
    TOKEN_NONE,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_SYMBOL,
};

enum symboltype
{
    SYMBOL_ANSWER,
    SYMBOL_UNKNOWN,
};

struct token
{
    union {
        int value;

        enum symboltype symbol;

        struct {
            enum operator operator;
            int precedence;
        };
    };

    enum tokentype type;
};

struct state
{
    int           answer;
    struct string statement;
    struct string evaluated;
};
