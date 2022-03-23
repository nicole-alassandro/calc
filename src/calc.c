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

#include <ctype.h>
#include <limits.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "types.c"
#include "error.c"
#include "input.c"
#include "parse.c"

int main(
    int     argc,
    char ** argv)
{
    volatile struct string statement = {NULL, NULL};
    volatile struct string evaluated = {NULL, NULL};

    int err = setjmp(exc_env);
    if (!err)
    {
        bool remaining_input = true;
        while (remaining_input)
        {
            remaining_input = get_input(argc, argv, &statement);

            evaluated = statement;
            printf("%d\n", eval(&evaluated));

            free(statement.start);

            statement = (struct string){NULL, NULL};
            evaluated = (struct string){NULL, NULL};
        };
    }
    else if (err == INVALID_USAGE)
    {
        puts("usage: calc <expr>");
    }
    else
    {
        const char * message = NULL;

        switch(err)
        {
            case INVALID_TOKEN:
                message = "Invalid token";
                break;

            case INVALID_EXPRESSION:
                message = "Invalid expression";
                break;

            case DIVIDE_BY_ZERO:
                message = "Cannot divide by zero";
                break;

            case MISSING_PARENTHESES:
                message = "Missing matching parentheses";
                break;

            default:
                message = "Encountered unknown error";
                break;
        }

        printf(
            " %.*s\n"
            "%*s^\n"
            "ERROR: %s\n",
            (int)(statement.end - statement.start),
            statement.start,
            (int)(evaluated.start - statement.start),
            "",
            message
        );
    }

    free(statement.start);

    return err;
}
