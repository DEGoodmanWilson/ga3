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
//     o-O     population.cpp
//      O      implementation for the ga3::population class.
//     O-o
//////////////////////////////////////////////////////////////////////

#include "population.hpp"

namespace ga3
{
population::population(uint64_t population_size,
                       std::vector<gene_range> gene_bounds,
                       chromosome::evaluation_function_t evaluation_function,
                       uint32_t num_threads) : num_threads_{num_threads}
                       // TODO pre-set population capacity
{
    // TODO without pre-sizing the population this is gonna be _slow_. Is there a better way?
    for(auto p = 0; p < population_size; ++p)
    {
        population_.emplace_back(gene_bounds, evaluation_function);
    }
}


chromosome& population::operator[](const uint64_t index)
{
    return population_[index];
}

chromosome population::at(const uint64_t index)
{
    return population_.at(index);
}

}