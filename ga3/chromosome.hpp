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
        return genes_[index];
    }

    gene at(const uint64_t index) const
    {
        return genes_.at(index);
    }


    // set up crossover kind


    static crossover_kind_t crossover_kind_;

    static void set_crossover(crossover_kind_t kind)
    {
        crossover_kind_ = kind;
    }



//    chromosome(const chromosome &p1, const chromosome &p2); //TODO crossover mechanism?

    ///Evaluate this chromosome. Don't want to do this lazily, _or_ at construction time, necessarily. We want to be smart about it. TODO can we be _smarter_?
//    double evaluate(void);

    ///Returns the fitness of a chromosome.
//    double getFitness(void) const;

    ///Returns the size of the chromosome.
    /**
     * Returns the size of the chromosome as set by the constructor
     * or by calling chromosome::setSize()
     */
//    int getSize(void) const
//    { return genes_.size(); };


    ///Sets the minimum values for each gene.
//    static void setMinRanges(std::vector<gene> ranges);

    ///Sets the maximum values for each gene.
//    static void setMaxRanges(std::vector<gene> ranges);

    ///Sets the gene located at index.
    /**
     * \param index Index of the gene to set
     * \param value Value to set gene to
     *
     * Sets the value of a particular gene.
     * \bug Does not check against upper and lower bounds for that gene.
     */
//    void set_gene(size_t index, gene value)
//    {
//        if(index >= N) throw std::out_of_range{"Index " + std::to_string(index) + " out of range"};
//        genes_[index] = value;
//        fitness_ = OPT_NS::nullopt;
//    };
    ///Returns a vector of the chromosome's genes.
    /**
     * Returns a vector containing the chromosome's genes.
     */
    std::array<gene, N> get_genes()
    {
        return genes_;
    };


    // Slicing and concatenation operators

    ///Grabs an arbitrary slice of a chromosome.
//    chromosome operator()(int start, int end);
//    gene operator[](int idx);


    ///Concatenate two chromosomes or chromosome fragments together.
//    chromosome &operator+(chromosome &&a);

//    chromosome &operator+(const chromosome &a);



    // Comparison operators

//    bool operator<(const chromosome &a) const;

    ///Compare two chromosomes for equality.
//    bool operator==(const chromosome &a);

    ///Print the contents of a chromosome.
//    friend std::ostream &operator<<(std::ostream &out, const chromosome &chromo);

    ///Read in a chromosome.
//    friend std::istream &operator>>(std::istream &in, chromosome &chromo);

    static std::array<gene_range, N> gene_bounds;
    using evaluation_function_t = std::function<double(std::array<gene, N>)>;
    static evaluation_function_t evaluation_function;

private:
    std::array<gene, N> genes_;
//    std::array<bool, N> slice_;

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
                break;
            case crossover_kind_t::uniform:
                break;
        }
        return result;
    }
};

template<size_t N>
typename chromosome<N>::evaluation_function_t chromosome<N>::evaluation_function{
        [](std::array<gene, N> genes) -> double
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
