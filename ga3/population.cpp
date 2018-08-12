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
population::selection_kind_t population::selection_kind_{population::selection_kind_t::roulette};
population::replacement_kind_t population::replacement_kind_{population::replacement_kind_t::generational};

namespace private_
{
static std::random_device rd_{};  //Will be used to obtain a seed for the random number engine
static std::mt19937 gen_{rd_()};
}

population::population(uint64_t population_size,
                       std::vector<gene_range> gene_bounds,
                       chromosome::evaluation_function_t evaluation_function,
                       uint32_t num_threads)
        :
        num_threads_{num_threads},
        most_fit_member_{0},
        task_size_{(num_threads_ > 0) ? population_size / num_threads_ : 0},
        thread_pool_{num_threads_}

// TODO pre-set population capacity
{
    population_.reserve(population_size);
    for (auto p = 0; p < population_size; ++p)
    {
        population_.emplace_back(gene_bounds, evaluation_function);
    }
}


chromosome &population::operator[](const uint64_t index)
{
    most_fit_member_ = index; // we may be overwriting a chromo. Invalidate the marker that fitnesses have been measured.
    return population_[index];
}

chromosome population::at(const uint64_t index)
{
    return population_.at(index);
}

// We need to understand how the next generation interacts with the current generation. This function is going to change for certain.
// move all of this into the initialization. The current generation must always be evaluated and sorted.


chromosome population::evaluate()
{
    // First, check if we've already done everything below
    if (!population_.at(most_fit_member_).is_evaluated())
    {
        // Else, 1) evaluate each chromosome, 2) sort the population 3) return the highest fit chromo

        std::vector<ThreadPool::ThreadPool::TaskFuture<void>> futures;
//        ThreadPool::ThreadSafeQueue<chromosome> chromos;
        auto start{0};

        for (auto t = 0; t < num_threads_; ++t)
        {
            futures.emplace_back(thread_pool_.submit([&, start = start]() -> auto
                                                     {
                                                         for (auto i = start; i < start + task_size_; ++i)
                                                         {
                                                             population_[i].evaluate();
                                                         }
                                                     }));

            start += task_size_;
        }

        //do the remainder of the work in this thread;
        for (auto i = start; i < population_.size(); ++i)
        {
            population_[i].evaluate();
        }

        // now we need to wait for all the tasks to complete; this is not super efficient, but what are you gonna do?
        for (auto &future : futures)
        {
            future.get();
        }

        // put all the chromos processed into _chromosomes
        // NOTICE THIS IS REALLY ONLY NEEDED FOR RANKED SELECTION
        // TODO make this more sophisitcated to not have to sort if we aren't using ranked selection
        std::sort(population_.begin(), population_.end(), std::greater<chromosome>());

    }
    most_fit_member_ = 0; //because it is sorted. TODO change this
    return population_.at(most_fit_member_);
}

void population::set_selection(ga3::population::selection_kind_t kind)
{
    population::selection_kind_ = kind;
}

void population::set_replacement(ga3::population::replacement_kind_t kind)
{
    population::replacement_kind_ = kind;
}

size_t population::select_()
{
    //TODO break these out into individual functions
    // The difference is that with ranked, we randomly pick a chromosome,
    switch (population::selection_kind_)
    {
        case selection_kind_t::roulette:
        {
            // calculate the total fitness of the population
            double sum_fitness = std::accumulate(population_.begin(),
                                                 population_.end(),
                                                 0.0,
                                                 [](double a, const chromosome &b)
                                                 {
                                                     return a + b.get_fitness();
                                                 });


            //spin that wheel!
            // The values on the wheel range from 0 to sum_fitness
            std::uniform_real_distribution<double> dis(0, sum_fitness);
            auto wheel_position = dis(private_::gen_);

            // now we need to determine _which_ chromosome this wheel position corresponds to!

            size_t i{std::numeric_limits<size_t>::max()};
            double partial_sum{0.0};
            do
            {
                ++i;
                auto f = population_[i].get_fitness();
                partial_sum += f;
            } while ((partial_sum < wheel_position) && (i != population_.size() - 1));


            return i;
        }
        case selection_kind_t::ranked:
            break;
    }
}

void population::evolve(uint64_t generations)
{
    evaluate();

    for (uint64_t i = 0; i < generations; ++i)
    {
        decltype(population_) next_generation;
        for (size_t i = 0; i < population_.size(); ++i)
        {
            auto a = select_();
            auto b = select_();
            auto chromo_a = population_.at(a);
            auto chromo_b = population_.at(b);
            next_generation.emplace_back(chromo_a + chromo_b);
        }

        //TODO break these out into individual functions
        switch (population::replacement_kind_)
        {
            case replacement_kind_t::generational:
                // and just full-on replace it. Do it.
                population_ = next_generation;
                evaluate();
                break;
            case replacement_kind_t::steady_state:
                auto size = population_.size();
                population_.insert(
                        population_.end(),
                        std::make_move_iterator(next_generation.begin()),
                        std::make_move_iterator(next_generation.end())
                );
                most_fit_member_ = population_.size() - 1; //invalidate evaluation. TODO need a better way!
                evaluate();
                // lop off the least-fit elements at the end.
                population_.erase(population_.begin() + size, population_.end());

                break;
        }

    }
}

}