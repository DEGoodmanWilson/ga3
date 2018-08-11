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
//     o-O     population.hpp
//      O      interface for the ga3::population class.
//     O-o
//////////////////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <ga3/chromosome.hpp>
#include <ThreadPool/ThreadPool.h>

namespace ga3
{

/// A class representing a population of chromosomes
/**
 * The population class represents an entire population of a single
 * generation of chromosomes. This population is replaced with each iteration
 * of the algorithm. Functions are provided for storing generations for later
 * analysis or retrieval, or for reloading the population from some point.
 * All of the high level functionality is in this
 * class: generally speaking, you will almost never call a function from any
 * of the other classes.
 */

class population
{
public:
    ///The constructor.
    population(uint64_t population_size,
               std::vector<gene_range> gene_bounds,
               chromosome::evaluation_function_t evaluation_function = [](std::vector<gene> &genes) -> double
               {
                   return 0;
               },
               uint32_t num_threads = std::thread::hardware_concurrency() - 1);

    chromosome &operator[](const uint64_t index);
    chromosome at(const uint64_t index);

private:
    std::vector<chromosome> population_;
    uint32_t num_threads_;
};

} //namespace ga3
