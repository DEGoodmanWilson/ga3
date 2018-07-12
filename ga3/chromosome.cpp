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
//     o-O     chromosome.cpp
//      O      implementation for the ga3::chromosome class.
//     O-o
//////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include "optional.h"
#include "chromosome.hpp"

namespace ga3
{
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//template<size_t N>
//chromosome<N>::chromosome(): fitness_{OPT_NS::nullopt}
//{
//    std::random_device rd;  //Will be used to obtain a seed for the random number engine
//    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
//
//// randomly initialize the genes
//    for (uint64_t n = 0; n < N; ++n)
//    {
//        std::uniform_int_distribution<uint64_t> dis(gene_bounds[n].first(), gene_bounds[n].second());
//        genes_[n] = dis(gen);
//    }
//}

//template<size_t N>
//std::array<gene, N> chromosome<N>::get_genes(void)
//{
//    return genes_;
//};



/**
 * \param start Begining of slice
 * \param end Ending of slice
 *
 * This function emulates Python's array slicing behaviour. It returns a
 * chromosome composed of all genes from the originating chromosome, from
 * start to end. %c.grabSlice(0, c.getSize()) will copy the genes in
 * chromosome c (but the genes only! Use chromosome<N>::operator=() to make
 * an exact copy, including non-gene members.). %grabSlice(20, 25) will return
 * a chromosome with five genes, from c[20] to c[24].
 * \bug To properly emulate Python, the range should be inclusive, but is not.
 * There is much internal code that depends on this function, so fixing this
 * could be hairy. Won't be a problem in the Python port of this code =)
 */
//chromosome chromosome<N>::operator()(int start, int end)
//{
//    chromosome retval(end - start);
//    for (int i = start; i < end; ++i)
//    {
//        retval.genes_[i] = this->genes_[i];
//    }
//    return retval;
//}

/**
 * Evaluate this chromosome by calling the fitness function (assiged with
 * chromosome<N>::setEvalFunc()) If the fitness function has not been set,
 * returns 0. Useful for forcing the chromosome to re-evaluate itself.
 * \bug It is impossible to differentiate a fitness of 0 from an error caused
 * by lack of fitness function. An exception should be raised or an error code
 * set or something.
 */
//template<size_t N>
//double chromosome<N>::evaluate()
//{
////    if (_evalFunc == NULL)
////    {
////        return 0; //there has to be a better way to deal with this case.
////    }
////    else
////    {
////        _isEvaluated = true;
////        fitness_ = _evalFunc(genes_);
////        return fitness_;
////    }
//}

/**
 * Returns the fitness of the chromosome. If it has yet to be determined,
 * the fitness function set with chromosome<N>::setEvalFunc() is called.
 * If the fitness function has not been set, returns 0.
 * Multiple calls will not impose an undue speed penalty, as the
 * fitness value is cached, and the fitness function is only ever called
 * a second time if the chromosome is changed in some way. This function
 * is preferable to chromosome<N>::evaluate() for most purposes.
 * \bug It is impossible to differentiate a fitness of 0 from an error caused
 * by lack of fitness function. An exception should be raised or an error code
 * set or something.
 */
//template<size_t N>
//double chromosome<N>::getFitness(void) const
//{
//    return fitness_;
//}

/**
 * \param ranges A vector containing the upper bounds to consider, one value
 * per gene.
 *
 * Each gene can be given a range of values to cover. Default is [0,1]. This
 * function is to tell the chromosome what upper bounds to use for each gene.
 * \bug Unknown errors could result if the parameter ranges and the chromosome
 * size are not the same!
 */
//template<size_t N>
//void chromosome<N>::setMaxRanges(std::vector<gene> ranges)
//{
//    if (ranges.size() != getSize())
//    {
//        return;
//    }
//    _geneMaxRanges.clear();
//    int i;
//    for (i = 0; i < ranges.size(); ++i)
//    {
//        _geneMaxRanges.push_back(ranges[i]);
//    }
//}

/**
 * \param ranges A vector containing the lower bounds to consider, one value
 * per gene.
 *
 * Each gene can be given a range of values to cover. Default is [0,1]. This
 * function is to tell the chromosome what lower bounds to use for each gene.
 * \bug Unknown errors could result if the parameter ranges and the chromosome
 * size are not the same!
 */
//template<size_t N>
//void chromosome<N>::setMinRanges(std::vector<gene> ranges)
//{
//    if (ranges.size() != getSize())
//    {
//        return;
//    }
//    minValues_.clear();
//    int i;
//    for (i = 0; i < ranges.size(); ++i)
//    {
//        _geneMinRanges.push_back(ranges[i]);
//    }
//}

/**
 * \param genes a vector of gene to use as this chromosome's genes.
 *
 * There are two ways to initialise a chromosome's genes: one is with
 * chromosome<N>::randomInit(). The other is to set them all by hand.
 * Useful for GA related experiments and whatnot. This function can, of course
 * be called any time.
 */
//template<size_t N>
//void chromosome<N>::setGenes(std::vector<gene> genes)
//{
//    if (genes.size() != getSize())
//    {
//        return;
//    }
//    genes_.clear();
//    int i;
//    for (i = 0; i < genes.size(); ++i)
//    {
//        genes_.push_back(genes[i]);
//    }
//    _isEvaluated = false;
//}

/**
 * Concatenates two chromosomes together into one. Combined with
 * chromosome<N>::slice(), can be used to create a variety of crossover
 * operations (in addition to those provided).
 */
//template<size_t N>
//chromosome &chromosome<N>::operator+(const chromosome &a)
//{
//    this->_size += a.getSize();
//    _evalFunc = a._evalFunc;
//
//    int i;
//    for (i = 0; i < a.genes_.size(); ++i)
//    {
//        this->genes_.push_back(a.genes_[i]);
//        this->_geneMaxRanges.push_back(a._geneMaxRanges[i]);
//        this->_geneMinRanges.push_back(a._geneMinRanges[i]);
//    }
//    this->_isEvaluated = false;
//    return *this;
//};
//
//template<size_t N>
//chromosome &chromosome<N>::operator+(chromosome &&a)
//{
//    this->_size += a.getSize();
//    _evalFunc = a._evalFunc;
//
//    int i;
//    for (i = 0; i < a.genes_.size(); ++i)
//    {
//        this->genes_.push_back(a.genes_[i]);
//        this->_geneMaxRanges.push_back(a._geneMaxRanges[i]);
//        this->_geneMinRanges.push_back(a._geneMinRanges[i]);
//    }
//    return *this;
//};
//
//template<size_t N>
//bool chromosome<N>::operator<(const chromosome &a) const
//{
//    return (this->fitness_ < a.fitness_);
//}


/**
 * Exactly duplicates a chromosome in memory.
 */
//chromosome &chromosome<N>::operator=(const chromosome &a)
//{
//    if (this != &a)
//    {
//        _crossSite = a._crossSite;
//        _evalFunc = a._evalFunc;
//        fitness_ = a.fitness_;
//        _isEvaluated = a._isEvaluated;
//        _parent[0] = a._parent[0];
//        _parent[1] = a._parent[1];
//        _size = a._size;
//        int i;
//        _geneMaxRanges.clear();
//        _geneMinRanges.clear();
//        genes_.clear();
//        for (i = 0; i < _size; ++i)
//        {
//            _geneMaxRanges.push_back(a._geneMaxRanges[i]);
//            _geneMinRanges.push_back(a._geneMinRanges[i]);
//            genes_.push_back(a.genes_[i]);
//        }
//    }
//    return *this;
//}

/**
 * Checks for equivalency, looking only at the contents of the
 * genes, and not any of the other members (like fitness for example; one of
 * the chromosomes may not have been evaluated yet).
 */
//template<size_t N>
//bool chromosome<N>::operator==(const chromosome &a)
//{
//    int i;
//    for (i = 0; i < a._size; ++i)
//    {
//        if (this->genes_[i] != a.genes_[i])
//        {
//            return false;
//        }
//    }
//    return true;
//}

/**
 * Prints the useful contents of the chromosome to a stream. Can be
 * used to serialise/marshall/pickle a chromosome.
 * \bug Does not bother to evaluate a chromosome if it has not already been.
 */
//template<size_t N>
//std::ostream &operator<<(std::ostream &out, const chromosome<N> &chromo)
//{
//    int i;
//
//    //first, output the fitness
//    if (chromo._isEvaluated)
//    {
//        out << chromo.fitness_ << "\n";
//    }
//    else
//    {
//        out << "0.0\n";
//    }
//    //and finally, the chromosome itself
//    for (i = 0; i < chromo.genes_.size() - 1; ++i)
//    {
//        out << chromo.genes_[i] << " ";
//    }
//    out << chromo.genes_[i] << "\n";
//    return out;
//}

/**
 * Reads a chromosome from a stream.
 * \bug The chromosome must have already been allocated with new, and the
 * proper size set, either in the constructor or with chromosome<N>::setSize().
 * Also, a fitness value of 0.0 is interpreted to mean that the chromosome
 * has not yet been evaluated.
 */
//template<size_t N>
//std::istream &operator>>(std::istream &in, chromosome<N> &chromo)
//{
//    int i;
//    gene f;
//    //get the fitness
//    in >> chromo.fitness_;
//    //if a fitness is specifed (ie, non-zero) set the evaluated flag to true
//    if (chromo.fitness_)
//    {
//        chromo._isEvaluated = true;
//    }
//    else
//    {
//        chromo._isEvaluated = false;
//    }
//    chromo.genes_.clear();
//    for (i = 0; i < chromo._size; ++i)
//    {
//        in >> f;
//        chromo.genes_.push_back(f);
//    }
//    return in;
//};

/**
 * Prints a chromosome's genes (only) into a stream. Don't know why I created
 * this function, as it looks useless (see
 * chromosome<N>::printAsSpaceDelimetedString()).
 */
//template<size_t N>
//void chromosome<N>::printAsString(std::ostream &out)
//{
//    int i, j;
//    for (i = 0; i < _size; ++i)
//    {
//        j = genes_[i];
//        out << j;
//    }
//}

/**
 * Prints a chromosome's genes (only) into a stream with space between
 * each gene value.
 */
//template<size_t N>
//void chromosome<N>::printAsSpaceDelimitedString(std::ostream &out)
//{
//    float i, j;
//    for (i = 0; i < _size - 1; ++i)
//    {
//        j = genes_[i];
//        out << j << " ";
//    }
//    j = genes_[i];
//    out << j;
//}

} // namespace ga3