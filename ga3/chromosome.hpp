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
//     o-O     chromosome.h
//      O      interface for the ga3::chromosome class.
//     O-o
//////////////////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include <array>
#include <functional>
#include <random>
#include <ga3/gene.h>
#include <ga3/optional.h>

namespace ga3
{

enum class crossover_kind_t
{
    one_point,
    two_point,
    uniform
};

///A class representing a chromosome i.e., a member of the population.
/**
 * The chromosome class represents a single chromosome in a population.
 * A chromosome contains some number of genes, and can be sliced up and put
 * back together. A population contains a number of chromosomes.
 */

template<size_t N>
class chromosome
{
public:
    ///Constructor that initializes the size of the chromosome. Probably more useful.
    chromosome() : fitness_{OPT_NS::nullopt}
    {
        // randomly initialize the genes
        for (uint64_t n = 0; n < N; ++n)
        {
            std::uniform_int_distribution<uint64_t> dis(gene_bounds[n].first, gene_bounds[n].second);
            genes_[n] = dis(gen_);
        }
    }


    // subscript operator
    gene &operator[](const uint64_t index)
    {
        fitness_ = OPT_NS::nullopt; // a gene may have changed. Invalidate the fitness
        return genes_[index];
    }

    gene at(const uint64_t index) const
    {
        return genes_.at(index);
    }


    static crossover_kind_t crossover_kind_;

    static void set_crossover(crossover_kind_t kind)
    {
        crossover_kind_ = kind;
    }


    static std::array<gene_range, N> gene_bounds;
    using evaluation_function_t = std::function<double(std::array<gene, N> &)>;
    static evaluation_function_t evaluation_function;

    double evaluate(void)
    {
        if (!fitness_)
        {
            fitness_ = evaluation_function(this->genes_);
        }
        return *fitness_;
    }

private:
    std::array<gene, N> genes_;

    OPT_NS::optional<double> fitness_;

protected:
    static std::random_device rd_;  //Will be used to obtain a seed for the random number engine
    static std::mt19937 gen_;

public:
    chromosome<N> operator+(chromosome<N> const &rhs)
    {
        // what we do is going to depend on the crossover function that is set.
        // TODO make kind_ protected and this function a friend!
        chromosome<N> result;

        switch (chromosome<N>::crossover_kind_)
        {
            case crossover_kind_t::one_point:
            {
                // TODO refactor this into a private function
                std::uniform_int_distribution<uint64_t> dis(0, N - 1);
                auto co_point = dis(chromosome<N>::gen_);
                uint64_t i;
                for (i = 0; i < co_point; ++i)
                {
                    result[i] = this->at(i);
                }
                for (; i < N; ++i)
                {
                    result[i] = rhs.at(i);
                }
            }
                break;
            case crossover_kind_t::two_point:
            {
                // TODO refactor this into a private function
                std::uniform_int_distribution<uint64_t> dis_1(0, N - 2);
                auto co_point_1 = dis_1(chromosome<N>::gen_);
                std::uniform_int_distribution<uint64_t> dis_2(co_point_1 + 1, N - 1);
                auto co_point_2 = dis_2(chromosome<N>::gen_);
                uint64_t i;
                for (i = 0; i < co_point_1; ++i)
                {
                    result[i] = this->at(i);
                }
                for (; i < co_point_2; ++i)
                {
                    result[i] = rhs.at(i);

                }
                for (; i < N; ++i)
                {
                    result[i] = this->at(i);
                }
            }
                break;
            case crossover_kind_t::uniform:
            {
                // TODO refactor this into a private function
                std::uniform_int_distribution<uint64_t> dis(0, 1);
                for (uint64_t i = 0; i < N; ++i)
                {
                    auto flip = dis(chromosome<N>::gen_);
                    if (flip == 0)
                    {
                        result[i] = this->at(i);
                    }
                    else
                    {
                        result[i] = rhs.at(i);
                    }
                }
            }
                break;
        }
        return result;
    }
};

template<size_t N>
typename chromosome<N>::evaluation_function_t chromosome<N>::evaluation_function{
        [](std::array<gene, N> &genes) -> double
        {
            return 0;
        }
};

template<size_t N>
std::random_device chromosome<N>::rd_{};  //Will be used to obtain a seed for the random number engine
template<size_t N>
std::mt19937 chromosome<N>::gen_{rd_()};

template<size_t N>
crossover_kind_t chromosome<N>::crossover_kind_{crossover_kind_t::one_point};


} // namespace ga3
