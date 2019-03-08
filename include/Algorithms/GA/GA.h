/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GA.h
 * Author: BrunoVinicius
 *
 * Created on February 27, 2019, 10:07 PM
 */

#ifndef GA_H
#define GA_H

class SimulationType;

#include <vector>
#include <memory>
#include <cassert>
#include <random>
#include <algorithm>

#include "IndividualBool.h"
#include "Individual.h"

/**
 * @brief Generic genetic algorithm class for single objective. 
 * This class contain the similar parameters, such as number of generations,
 * number of individuals, etc, to all specific genetic algorithms.
 */
class GA {
    
private:
    /**
     * @brief Struct to compare two individuals, based on their fitness.
     */
    struct IndividualCompare{
        /**
         * @brief Operator to compare two distinct individuals.
         * @param indA First individual.
         * @param indB Second individual.
         * @return True if the first individual fitness is larger than 
         * the second.
         */
        bool operator()(const std::shared_ptr<Individual>& indA,
                        const std::shared_ptr<Individual>& indB) const;
    };
    
    friend std::ostream& operator<<(std::ostream& ostream, 
    const GA* ga);

public:
    /**
     * @brief Default constructor of a GA algorithm.
     * @param simul Simulation that own this algorithm.
     */
    GA(SimulationType* simul);
    /**
     * @brief Default destructor of a GA algorithm.
     */
    virtual ~GA();

    /**
     * @brief Initialize the GA algorithm with the probability distribution.
     */
    virtual void Initialize();
    /**
     * @brief Initialize the population with random individuals, created only 
     * in the derived class. 
     */
    virtual void InitializePopulation() = 0;
    /**
     * @brief Function to create the new population based on the select 
     * population.
     */
    virtual void CreateNewPopulation() = 0;
    /**
     * @brief Keeps the selected population in the initial population container.
     */
    virtual void KeepInitialPopulation();
    /**
     * @brief Selects the best population among the total population generated
     * by crossover and mutation. The selection is made first, selecting a 
     * specified number of best individuals, and for last, choosing randomly
     * the rest of the selected population.
     */
    void SelectPopulation();
    /**
     * @brief Function that saves the best and the worst individuals in their
     * respective containers.
     */
    virtual void SaveIndividuals();
    
    /**
     * @brief Gets the number of generation of this GA algorithm.
     * @return Number of generations.
     */
    const unsigned int GetNumberGenerations() const;
    /**
     * @brief Gets the number of individuals of this GA algorithm.
     * @return Number of individuals.
     */
    const unsigned int GetNumberIndividuals() const;
    /**
     * @brief Gets the crossover probability of this GA algorithm.
     * @return Crossover probability.
     */
    const double GetProbCrossover() const;
    /**
     * @brief Gets the mutation probability of this GA algorithm.
     * @return Mutation probability.
     */
    const double GetProbMutation() const;
    /**
     * @brief Gets the maximum number of simulations per individual.
     * @return Maximum number of simulations.
     */
    const unsigned int GetMaxNumSimulation() const;
    /**
     * @brief Sets the sumFitness variable.
     * @param sumFitness Sum of fitness.
     */
    void SetSumFitness(double sumFitness);

    /**
     * @brief Return the probability generated by the probDistribution.
     * @return Random probability.
     */
    double GetProbDistribution();
    /**
     * @brief Return the size of the total population container.
     * @return Size of total population.
     */
    unsigned int GetNumTotalPopulation() const;
    /**
     * @brief Gets the simulation that owns the GA algorithm.
     * @return SimulationType pointer.
     */
    SimulationType* GetSimul() const;
    /**
     * @brief Gets the actual generation of this GA algorithm.
     * @return Actual generation.
     */
    unsigned int GetActualGeneration() const;
    /**
     * @brief Sets the actual generation of this GA algorithm.
     * @param actualGeneration Actual generation.
     */
    void SetActualGeneration(unsigned int actualGeneration);
    /**
     * @brief Calculate and sets the sum of all selected population 
     * individuals fitness.
     */
    void SetSumFitnessSelectedPop();
    
    /**
     * @brief Gets the worst individual of the actual generation.
     * @return Worst individual.
     */
    Individual* GetWorstIndividual() const;
    /**
     * @brief Gets the best individual of the actual generation.
     * @return Best individual.
     */
    Individual* GetBestIndividual() const;
    /**
     * @brief Gets an specified individual from the initial population
     * container,
     * @param index Index of the initial population container.
     * @return Specified individual.
     */
    Individual* GetIniIndividual(unsigned int index);
    
    /**
     * @brief Apply the gene of a specified individual in the network.
     * @param ind Individual pointer.
     */
    virtual void ApplyIndividualGene(Individual* ind) = 0;
    /**
     * @brief Set the individual parameters found by the simulation.
     * @param ind Specified individual.
     */
    virtual void SetIndParameters(Individual* ind) = 0;
    /**
     * @brief Pick an individual based on the roulette choosing process. 
     * @return Chosen individual.
     */
    Individual* RouletteIndividual();
    
private:
    /**
     * @brief Pointer to a SimulationType object that owns this algorithm.
     */
    SimulationType* simul;
    /**
     * @brief Total number of individuals.
     */
    const unsigned int numberIndividuals;
    /**
     * @brief Total number of generations.
     */
    const unsigned int numberGenerations;
    /**
     * @brief Crossover probability.
     */
    const double probCrossover;
    /**
     * @brief Mutation probability.
     */
    const double probMutation;
    /**
     * @brief Number of best individuals the process of selection will choose.
     */
    const unsigned int numBestIndividuals;
    /**
     * @brief Actual generation of this GA algorithm.
     */
    unsigned int actualGeneration;
    /**
     * @brief Number of maximum simulation each individual will do.
     */
    const unsigned int maxNumSimulation;
    /**
     * @brief Sum of all individuals fitness of the selected individuals 
     * container.
     */
    double sumFitness;
    
    /**
     * @brief Container of individuals, representing the initial population.
     */
    std::vector<std::shared_ptr<Individual>> initialPopulation;
    /**
     * @brief Container of individuals, representing the best individuals, one
     * for each generation.
     */
    std::vector<std::shared_ptr<Individual>> bestIndividuals;
    /**
     * @brief Container of individuals, representing the worst individuals, one
     * for each generation.
     */
    std::vector<std::shared_ptr<Individual>> worstIndividuals;
    
    /**
     * @brief Probability distribution used in this GA algorithm for crossover
     * and mutation.
     */
    std::uniform_real_distribution<double> probDistribution;
    /**
     * @brief Probability distribution used in this GA algorithm for the 
     * roulette draw.
     */
    std::uniform_real_distribution<double> fitnessDistribution;
    
public:
    /**
     * @brief Container of individuals, representing the selected population.
     */
    std::vector<std::shared_ptr<Individual>> selectedPopulation;
    /**
     * @brief Container of individuals, representing the total population.
     */
    std::vector<std::shared_ptr<Individual>> totalPopulation;
    /**
     * @brief Random generator.
     */
    static std::default_random_engine random_generator;
};

#endif /* GA_H */

