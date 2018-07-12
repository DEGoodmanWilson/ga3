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
//     o-O     population.h
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
    int _size;

    std::function<double(const std::vector<ga2Gene> &)> _evalFunc;

    bool _mutateFunc(ga2Chromosome &a);

    int _selectRoulette(void); //returns integer index into vector
    bool _crossoverFunc(ga2Chromosome &a, ga2Chromosome &b);

    bool _replaceFunc(void);

    int _selectRanked(void);

    int _selectFunc(void);

    bool _crossoverOnePoint(ga2Chromosome &a, ga2Chromosome &b); //inserts two new critters in the nextGen
    bool _crossoverUniform(ga2Chromosome &a, ga2Chromosome &b);

    bool _replaceSteadyState(void);

    bool _replaceSteadyStateNoDuplicates(void);

    bool _replaceGenerational(void);

    int _chromoSize;
    std::vector<ga2Gene> _chromoMaxRanges;
    std::vector<ga2Gene> _chromoMinRanges;
    std::vector<ga2Chromosome> _chromosomes;
    std::vector<ga2Chromosome> _nextGen;

    double _mutationRate;
    double _crossoverRate;
    double _replacementSize;
    bool _integer;
    bool _isSorted;
    int _crossoverType;
    int _selectionType;
    int _replacementType;

    double _sumFitness;
    double _avgFitness;
    double _minFitness;
    double _maxFitness;

    int _crossCount;
    int _mutationCount;

    ThreadPool::ThreadPool thread_pool_;
    uint32_t num_threads_;

public:
    ///The constructor.
    population(int initialSize, int chromoSize, uint32_t num_threads = std::thread::hardware_concurrency() - 1);

    ///The destructor.
    virtual ~population();
    ///Set the selection function to use.
    /**
     * \param type valid values are GA2_SELECT_ROULETTE or GA2_SELECT_RANKED
     *
     * Sets selection type to roulette or ranked
     */
    void setSelectType(int type)
    { _selectionType = type; };
    ///Set the crossover function to use.
    /**
     * \param type valid values are GA2_CROSSOVER_ONEPOINT or
     * GA2_CROSSOVER_UNIFORM
     *
     * Sets crossover to one-point or uniform. More types to come
     */
    void setCrossoverType(int type)
    { _crossoverType = type; };
    ///Set the replacement function to use.
    /**
     * \param type valid values are GA2_REPLACE_GENERATIONAL,
     * GA2_REPLACE_STEADYSTATE, or
     * GA2_REPLACE_STEADYSTATENODUPLICATES
     *
     * Sets replacement to generational (total replacement) or
     * steady-state (only the worst offenders are booted). If
     * I recall, steady-state requires a sorted population to
     * properly remove the least fit.
     */
    void setReplaceType(int type)
    { _replacementType = type; };
    ///Set the evaluation function to use.
    /**
     * \param func the function to call. Must be of form
     * double my_func(std::vector<ga2Gene> chromo_to_evaluate)
     *
     * Tells the Population which function to use to evaluate its
     * members.
     */
    void setEvalFunc(std::function<double(const std::vector<ga2Gene> &)> func)
    { _evalFunc = func; };

    ///Initialise the population.
    bool init(void);

    ///Select from the current generation for the next.
    bool select(void);

    ///Evaluate the next generation.
    bool evaluate(void);

    ///Crossover the current generation.
    bool crossover(void);

    ///Mutate the current generation.
    bool mutate(void);

    ///Replace the current generation with the next generation.
    bool replace(void);

    ///Set the minimum values for each gene.
    void setMinRanges(std::vector<ga2Gene> ranges);

    ///Set the maximum values for each gene.
    void setMaxRanges(std::vector<ga2Gene> ranges);

    ///Get the maximum values for each gene.
    float getMaxRanges(int index)
    { return _chromoMaxRanges[index]; };

    ///Get the minimum values for each gene.
    float getMinRanges(int index)
    { return _chromoMinRanges[index]; };

    ///Get the size of the population.
    int getSize(void)
    { return _size; };

    ///Set the probability of a gene mutating.
    void setMutationRate(float mRate)
    { _mutationRate = mRate; };

    ///Set the probability of a chromosome crossing-over.
    void setCrossoverRate(float cRate)
    { _crossoverRate = cRate; };

    ///Set the number of chromosomes chosen from the next generation to replace the current generation.
    void setReplacementSize(int rSize)
    { _replacementSize = rSize; };

    ///Print the population as a pretty string.
    void printAsString(std::ostream &out, int index)
    { _chromosomes[index].printAsString(out); };

    ///Print the population as a more machine friendly string.
    void printAsSpaceDelimitedString(std::ostream &out, int index)
    { _chromosomes[index].printAsSpaceDelimitedString(out); };

    ///Are we using integer genes or floating point genes?
    void setInteger(bool val)
    { _integer = val; };

    //note: sorting really messes up canonical generational replacement.
    //instead of replacing the entire generation, only the best fit
    //of the current and next are kept, weeding out all the weaker ones.
    ///Should the population be sorted by fitness?
    void setSort(bool val)
    { _isSorted = val; }; //expensive!! use only if you must!
    ///Return the highest fitness value reported.
    /**
     * Returns the highest fitness value in the population, but
     * not the chromosome it belongs to.
     */
    double getMaxFitness(void)
    { return _maxFitness; };
    ///Return the smallest fitness value reported.
    /**
     * Returns the smallest fitness value in the population, but
     * not the chromosome it belongs to.
     */
    double getMinFitness(void)
    { return _minFitness; };
    ///Return the average fitness of the population.
    /**
     * Does what it says. Returns the average fitness over the entire
     * population
     */
    double getAvgFitness(void)
    { return _avgFitness; };
    ///Return the sum of all chromosomes' fitness values.
    /**
     * Does what it says. Returns the sum of all fitness values
     * in the population.
     */
    double getSumFitness(void)
    { return _sumFitness; };

    ///Return the fitness of a single chromosome.
    double getFitness(int index)
    { return _chromosomes[index].getFitness(); };

    ///Return the most fit chromosome.
    std::vector<gene> getBestFitChromosome(void);

    ///Return one of the parents of a single chromosome.
    int getParent(int c, int p)
    { return _chromosomes[c].getParent(p); };

    ///Get the crossover site of a single chromosome.
    int getCrossSite(int index)
    { return _chromosomes[index].getCrossSite(); };

    ///Return the number of mutations performed.
    int getMutationCount(void)
    { return _mutationCount; };

    ///Return the number of crossovers performed.
    int getCrossCount(void)
    { return _crossCount; };

    ///Output the entire population.
    friend std::ostream &operator<<(std::ostream &o, population &pop);

    ///Read in a stored generation.
    friend std::istream &operator>>(std::istream &in, population &pop);
};

} //namespace ga3
