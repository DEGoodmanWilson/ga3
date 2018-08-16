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
//     o-O     chromosome.hpp
//      O      interface for the ga3::chromosome class.
//     O-o
//////////////////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include <functional>
#include <random>
#include <vector>
#include <ga3/gene.hpp>
#include <ga3/optional.hpp>

namespace ga3
{

///A class representing a chromosome i.e., a member of the population.
/**
 * The chromosome class represents a single chromosome in a population.
 * A chromosome contains some number of genes, and can be sliced up and put
 * back together. A population contains a number of chromosomes.
 */


class chromosome
{
public:
    using evaluation_function_t = std::function<double(std::vector<gene> &)>;

    enum class crossover_kind_t
    {
        one_point,
        two_point,
        uniform
    };

    ///Constructor that initializes the size of the chromosome. Probably more useful.
    chromosome(const std::vector<gene_range> bounds,
               crossover_kind_t crossover_kind,
               evaluation_function_t evaluation_function);

    // subscript operator
    gene &operator[](const size_t index);

    gene at(const size_t index) const;

    void mutate(void);

    chromosome operator+(chromosome const &rhs);

    bool operator<(const chromosome &rhs) const;
    bool operator>(const chromosome &rhs) const;

    double evaluate(void);
    double get_fitness(void) const;
    std::vector<gene> get_genes(void) const;

private:
    std::vector<gene> genes_;
    std::vector<gene_range> gene_bounds_;
    evaluation_function_t evaluation_function_;
    crossover_kind_t crossover_kind_;

    OPT_NS::optional<double> fitness_;

    static std::random_device rd_;  //Will be used to obtain a seed for the random number engine
    static std::mt19937 gen_;
};


} // namespace ga3
