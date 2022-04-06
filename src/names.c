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

bool symbol_matches(
    struct string * const symbol,
    const  char   * const name)
{
    const size_t size = (size_t)(symbol->end - symbol->start);

    if (size != strlen(name))
        return 0;

    return strncmp(symbol->start, name, size) == 0;
}

enum symboltype resolve_symbol(
    struct string * const symbol)
{
    if (symbol_matches(symbol, "ans"))
        return SYMBOL_ANSWER;

    error(UNDEFINED_SYMBOL);
    return SYMBOL_UNKNOWN;
}
