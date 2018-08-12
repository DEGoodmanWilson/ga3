//   O-----O
//   o-----O   ga3 - C++ genetic algorithm library
//    o---O    Copyright (C) 2001-2018 Don Goodman-Wilson
//     o-O
//      O      This program is free software; you can redistribute it and/or modify
//     O-o     it under the terms of the GNU General Public License as published by
//    O---o    the Free Software Foundation; either version 2 of the License, or
//   O-----o   (at your option) any later version.
//   O-----O   This program is distributed in the hope that it will be useful,
//   o-----O   but WITHOUT ANY WARRANTY; without even the implied warranty of
//    o---O    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     o-O     GNU General Public License for more details.
//      O
//     O-o     ga3 - C++ genetic algorithm library
//    O---o    You should have received a copy of the GNU General Public License
//   O-----o   along with this program; if not, write to the Free Software
//   O-----O   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//   o-----O
//    o---O
//     o-O     types.hpp
//      O      helpful typing stuffs
//     O-o
//////////////////////////////////////////////////////////////////////

#pragma once

#define GA3_FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

#define MAKE_NUMERIC_LIKE(T, x) class x\
{ \
public: \
    x() = default; \
    x(const x &rhs) = default; \
    x(x &&rhs) = default; \
    x &operator=(const x &rhs) = default; \
    x &operator=(x &&rhs) = default; \
    x(T new_val) : value{new_val} {} \
    operator T() {return value;} \
private: \
    T value; \
}