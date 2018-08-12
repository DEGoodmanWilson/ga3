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

#include "chromosome.hpp"

namespace ga3
{

std::random_device chromosome::rd_{};  //Will be used to obtain a seed for the random number engine
std::mt19937 chromosome::gen_{rd_()};

chromosome::crossover_kind_t chromosome::crossover_kind_{chromosome::crossover_kind_t::one_point};

chromosome::chromosome(const std::vector<gene_range> bounds,
                       evaluation_function_t evaluation_function) :
        gene_bounds_{bounds},
        fitness_{OPT_NS::nullopt},
        evaluation_function_{evaluation_function}
// TODO pre-set population capacity
{
    // randomly initialize the genes
    // TODO this should be done in the gene constructor?
    genes_.reserve(gene_bounds_.size());
    for (uint64_t n = 0; n < gene_bounds_.size(); ++n)
    {
        std::uniform_int_distribution<uint64_t> dis(gene_bounds_[n].first, gene_bounds_[n].second);
        genes_.emplace_back(dis(gen_));
    }
}

gene &chromosome::operator[](const uint64_t index)
{
    fitness_ = OPT_NS::nullopt; // a gene may have changed. Invalidate the fitness
    return genes_[index];
}

gene chromosome::at(const uint64_t index) const
{
    return genes_.at(index);
}

void chromosome::mutate()
{
    std::uniform_int_distribution<size_t> index_dis(0, gene_bounds_.size()-1);
    auto index{index_dis(gen_)};
    std::uniform_int_distribution<uint64_t> value_dis(gene_bounds_[index].first, gene_bounds_[index].second);

    auto new_val = genes_.at(index);
    while(new_val == genes_.at(index))
    {
        new_val = value_dis(gen_);
    }

    genes_[index] = new_val;
}

//static
void chromosome::set_crossover(crossover_kind_t kind)
{
    crossover_kind_ = kind;
}

double chromosome::evaluate(void)
{
    if (!fitness_)
    {
        fitness_ = evaluation_function_(this->genes_);
        if (fitness_ < 0.0)
        {
            throw std::out_of_range("Fitness values cannot be less than zero");
        }
    }
    return *fitness_;
}

bool chromosome::is_evaluated() const
{
    return !!fitness_;
}

double chromosome::get_fitness(void) const
{
    if (!fitness_)
    {
        throw std::runtime_error("Fitness values must be already calculated");
    }
    return *fitness_;
}

chromosome chromosome::operator+(chromosome const &rhs)
{
    // what we do is going to depend on the crossover function that is set.
    // TODO make kind_ protected and this function a friend!
    chromosome result{*this};
    const auto size = genes_.size();

    switch (chromosome::crossover_kind_)
    {
        case crossover_kind_t::one_point:
        {
            // TODO refactor this into a private function
            std::uniform_int_distribution<uint64_t> dis(0, size - 1);
            auto co_point = dis(chromosome::gen_);
            for (uint64_t i = co_point; i < size; ++i)
            {
                result[i] = rhs.at(i);
            }
        }
            break;
        case crossover_kind_t::two_point:
        {
            // TODO refactor this into a private function
            std::uniform_int_distribution<uint64_t> dis_1(0, size - 2);
            auto co_point_1 = dis_1(chromosome::gen_);
            std::uniform_int_distribution<uint64_t> dis_2(co_point_1 + 1, size - 1);
            auto co_point_2 = dis_2(chromosome::gen_);

            for (uint64_t i = co_point_1; i < co_point_2; ++i)
            {
                result[i] = rhs.at(i);

            }
        }
            break;
        case crossover_kind_t::uniform:
        {
            // TODO refactor this into a private function
            std::uniform_int_distribution<uint64_t> dis(0, 1);
            for (uint64_t i = 0; i < size; ++i)
            {
                auto flip = dis(chromosome::gen_);
                if (flip == 0)
                {
                    result[i] = rhs.at(i);
                }
            }
        }
            break;
    }
    return result;
}

bool chromosome::operator<(const ga3::chromosome &rhs) const
{
    // TODO throw something if fitness_ is false!
    if (!this->fitness_ || !rhs.fitness_)
    {
        throw std::invalid_argument("One or more chromosomes have not been evaluated!");
    }

    return *(this->fitness_) < *(rhs.fitness_);
}

bool chromosome::operator>(const ga3::chromosome &rhs) const
{
    // TODO throw something if fitness_ is false!
    if (!this->fitness_ || !rhs.fitness_)
    {
        throw std::invalid_argument("One or more chromosomes have not been evaluated!");
    }

    return *(this->fitness_) > *(rhs.fitness_);
}

}