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

#include <ga3/types.hpp>
#include <iostream>
#include <vector>
#include <functional>
#include <ga3/chromosome.hpp>
#include <ThreadPool/ThreadPool.hpp>

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
               chromosome::evaluation_function_t evaluation_function) :
            num_threads_{single_threaded ? 0 :std::thread::hardware_concurrency() - 1},
            population_size_{population_size},
            most_fit_member_{0},
            task_size_{(num_threads_ > 0) ? population_size_ / (num_threads_+1) : 0},
            thread_pool_{num_threads_},
            selection_kind_{selection_kind_t::roulette},
            replacement_kind_{replacement_kind_t::generational},
            mutation_rate_{0.0},
            crossover_rate_{1.0},
            crossover_kind_{chromosome::crossover_kind_t::one_point},
            replacement_rate_{1.0}
    {
        initialize_(population_size, std::move(gene_bounds), std::move(evaluation_function));
    }

    template<typename ...Os>
    population(uint64_t population_size,
               std::vector<gene_range> gene_bounds,
               chromosome::evaluation_function_t evaluation_function,
               Os &&...os) :
            num_threads_{single_threaded ? 0 : std::thread::hardware_concurrency() - 1},
            population_size_{population_size},
            most_fit_member_{0},
            task_size_{(num_threads_ > 0) ? population_size_ / (num_threads_+1) : 0},
            thread_pool_{num_threads_},
            selection_kind_{selection_kind_t::roulette},
            replacement_kind_{replacement_kind_t::generational},
            mutation_rate_{0.0},
            crossover_rate_{1.0},
            crossover_kind_{chromosome::crossover_kind_t::one_point},
            replacement_rate_{1.0}
    {
        initialize_(population_size, std::move(gene_bounds), std::move(evaluation_function));
        set_options_(GA3_FWD(os)...);
    }


    enum class selection_kind_t
    {
        roulette,
        ranked
    };
    enum class replacement_kind_t
    {
        steady_state,
        generational
    };

    // helpful operators

    chromosome &operator[](const uint64_t index);

    chromosome at(const uint64_t index);


    // The meat

    chromosome evaluate();

    void evolve(uint64_t generations, std::function<void(void)> = nullptr);


    // configuration types
    MAKE_NUMERIC_LIKE(double, mutation_rate);
    MAKE_NUMERIC_LIKE(double, crossover_rate);
    MAKE_NUMERIC_LIKE(double, replacement_rate);

    static bool single_threaded;

private:
    size_t population_size_;
    std::vector<chromosome> population_;
    uint32_t num_threads_;
    size_t most_fit_member_;
    uint64_t task_size_;
    ThreadPool::ThreadPool thread_pool_;
    selection_kind_t selection_kind_;
    replacement_kind_t replacement_kind_;
    double mutation_rate_;
    double crossover_rate_;
    chromosome::crossover_kind_t crossover_kind_;
    double replacement_rate_;



    size_t select_(void);


    void initialize_(uint64_t population_size,
                     std::vector<gene_range> &&gene_bounds,
                     chromosome::evaluation_function_t &&evaluation_function);

    template<typename T>
    void set_options_(T &&t)
    {
        set_option_(GA3_FWD(t));
    }

    template<typename T, typename... Ts>
    void set_options_(T &&t, Ts &&... ts)
    {
        set_options_(GA3_FWD(t));
        set_options_(GA3_FWD(ts)...);
    }

    void set_option_(selection_kind_t value);

    void set_option_(replacement_kind_t value);

    void set_option_(mutation_rate value);
    void set_option_(crossover_rate value);
    void set_option_(chromosome::crossover_kind_t value);
    void set_option_(replacement_rate value);

};

} //namespace ga3
