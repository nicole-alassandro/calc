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

int parse_primary   (volatile struct state *);
int parse_expression(volatile struct state *, int, int);

struct token advance(
    volatile struct string * const string)
{
    struct token result = {.type = TOKEN_NONE};

    while (isspace(*string->start))
        if (string->start++ >= string->end)
            return result;

    if (string->start >= string->end)
        return result;

    switch (*string->start)
    {
        case '-':
            result.operator   = SUBTRACTION;
            result.precedence = 0;
            break;

        case '+':
            result.operator   = ADDITION;
            result.precedence = 0;
            break;

        case '/':
            result.operator   = DIVISION;
            result.precedence = 1;
            break;

        case '*':
            result.operator   = MULTIPLICATION;
            result.precedence = 1;
            break;

        case ')':
            result.operator   = RPAREN;
            result.precedence = -1;
            break;

        case '(':
            result.operator   = LPAREN;
            result.precedence = INT_MAX;
            break;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
            result.type  = TOKEN_NUMBER;
            result.value = 0;

            char c = *string->start;
            while (isdigit(c) && string->start < string->end)
            {
                result.value = result.value * 10 + (c - '0');
                c = *(++string->start);
            }

            return result;
        }

        default:
        {
            char c = *string->start;

            if (!isalnum(c))
                error(INVALID_TOKEN);

            struct string symbol = *string;
            while (isalnum(c) && string->start < string->end)
                c = *(++string->start);

            symbol.end = string->start;

            result.type   = TOKEN_SYMBOL;
            result.symbol = resolve_symbol(&symbol);
            return result;
        }
    }

    result.type = TOKEN_OPERATOR;
    string->start++;

    return result;
}

struct token peek(
    volatile struct string * const string)
{
    struct string copy = *string;
    return advance(&copy);
}

int parse_expression(
    volatile struct state * const state,
    const int lhs,
    const int min_precedence)
{
    int result = lhs;

    volatile struct string * const expression = &state->evaluated;

    struct token next = peek(expression);
    while (next.type == TOKEN_OPERATOR && next.precedence >= min_precedence)
    {
        const enum operator operator   = next.operator;
        const int           precedence = next.precedence;

        advance(expression);
        int rhs = parse_primary(state);

        next = peek(expression);
        while (next.type == TOKEN_OPERATOR && next.precedence > precedence)
        {
            rhs  = parse_expression(state, rhs, precedence + 1);
            next = peek(expression);
        }

        switch (operator)
        {
            case SUBTRACTION:    result -= rhs; break;
            case ADDITION:       result += rhs; break;
            case MULTIPLICATION: result *= rhs; break;
            case DIVISION:       result /= check_zero(rhs); break;

            default: break;
        }
    }

    return result;
}

int parse_primary(
    volatile struct state * const state)
{
    int result = 0;

    volatile struct string * const expression = &state->evaluated;

    struct token next = advance(expression);

    if (next.type == TOKEN_NONE)
        error(INVALID_EXPRESSION);

    if (next.type == TOKEN_OPERATOR)
    {
        switch (next.operator)
        {
            case SUBTRACTION:
            {
                result = -parse_primary(state);
                break;
            }

            case LPAREN:
            {
                result = parse_expression(state, parse_primary(state), 0);

                next = advance(expression);
                if (next.type != TOKEN_OPERATOR || next.operator != RPAREN)
                    error(MISSING_PARENTHESES);

                break;
            }

            default:
                error(INVALID_EXPRESSION);
        }
    }
    else if (next.type == TOKEN_NUMBER)
    {
        if (next.type == TOKEN_NUMBER)
            result = next.value;

        next = peek(expression);
        if (next.type == TOKEN_OPERATOR && next.operator == LPAREN)
            error(INVALID_EXPRESSION);
        else if (next.type != TOKEN_OPERATOR && next.type != TOKEN_NONE)
            error(INVALID_EXPRESSION);
    }
    else if (next.type == TOKEN_SYMBOL)
    {
        if (next.symbol == SYMBOL_ANSWER)
            result = state->answer;

        next = peek(expression);
        if (next.type == TOKEN_OPERATOR && next.operator == LPAREN)
            // TODO: Remove once built-in functions are supported
            error(INVALID_EXPRESSION);
        else if (next.type != TOKEN_OPERATOR && next.type != TOKEN_NONE)
            error(INVALID_EXPRESSION);
    }

    return result;
}

void eval(
    volatile struct state * const state)
{
    state->answer = parse_expression(state, parse_primary(state), 0);
}
