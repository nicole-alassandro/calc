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

enum errortype
{
    INVALID_USAGE = 1,
    INVALID_TOKEN,
    INVALID_EXPRESSION,
    DIVIDE_BY_ZERO,
    ALLOCATION_ERROR,
    INPUT_ERROR,
    MISSING_PARENTHESES,
};

jmp_buf exc_env;

void error(
    const enum errortype code)
{
    longjmp(exc_env, (int)code);
}

int check_zero(
    const int n)
{
    if (!n)
        error(DIVIDE_BY_ZERO);

    return n;
}
