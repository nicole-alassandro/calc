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

bool get_input(
    int     argc,
    char ** argv,
    volatile struct string * const statement)
{
    // Passing statement as argument
    if (argc > 1)
    {
        if (argc <= 1)
            error(INVALID_USAGE);

        size_t bufsize = 0;

        for (int i = 1; i < argc; ++i)
            bufsize += strlen(argv[i]);

        if (!bufsize)
            error(INVALID_USAGE);

        // Add one whitespace between each argv, plus null terminator
        bufsize += (size_t)(argc - 2) + 1;

        struct string result;
        result.start = calloc(bufsize, sizeof(char));

        if (!result.start)
            error(ALLOCATION_ERROR);

        result.end = result.start + bufsize - 1;

        char * dest = result.start;

        for (int i = 1; i < argc; ++i)
        {
            char * word = argv[i];

            while (*word) *dest++ = *word++;
            *dest++ = ' ';
        }

        *statement = result;

        return false;
    }
    // Passing statement via stdin (either in REPL or piped in)
    else
    {
        const bool is_repl = isatty(STDIN_FILENO);

        if (is_repl)
            fputs("= ", stdout);

        const size_t bufsize = 128;

        char * result = calloc(bufsize, sizeof(char));

        if (!result)
            error(ALLOCATION_ERROR);

        size_t total = 0;

        while (true)
        {
            const int c = getchar();

            if (c != EOF)
            {
                if (c != '\n')
                {
                    *(result + (total++)) = (char)c;
                }
                else
                {
                    *(result + (total++)) = '\0';
                    break;
                }

                if (total >= bufsize)
                    if (!realloc(result, bufsize + total))
                        goto repl_allocation_error;
            }
            else if (ferror(stdin))
            {
                goto repl_input_error;
            }
        }

        statement->start = result;
        statement->end   = result + total - 1;

        return is_repl;

    repl_input_error:
        free(result);
        error(INPUT_ERROR);

    repl_allocation_error:
        free(result);
        error(ALLOCATION_ERROR);
    }

    return false;
}
