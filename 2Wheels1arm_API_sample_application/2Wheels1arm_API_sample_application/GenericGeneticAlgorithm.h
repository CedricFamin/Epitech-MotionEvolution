#pragma once

#include <vector>
#include <random>

template<typename T>
class IChromosome
{
public:
	virtual ~IChromosome(void)  = 0;

	virtual float Fitnress(void) const = 0;
	virtual T& GetValue(void) = 0;
};

template<typename T>
class AGeneticAlgorithm
{
public:
	typedef T value_type;
	typedef IChromosome<T> Chromosome;
	typedef std::vector<Chromosome> PopulationType;

	AGeneticAlgorithm(unsigned int populationSize, unsigned int generationSurviveCount)
	: _chromosomePopulationSize(populationSize)
	, _generationSurvivorCount(generationSurviveCount)
	, _generationNumber(1)
	{
		this->CreateInitialPopulation();
	}

	virtual ~AGeneticAlgorithm(void) = 0;

	PopulationType const & ChromosomePopulation() const { return this->_chromosomePopulation; }

	void MakeEvolution()
	{
		PopulationType newPopulation = this->GetSurvivors();

		while (newPopulation.size() < this->_chromosomePopulationSize)
		{
			// remarque, on peu faire un BB tout seul ...
			unsigned int fatherIndex = rand() % newPopulation.size();
			unsigned int motherIndex = rand() % newPopulation.size();

			PopulationType newChromosome = this->CrossOver(newPopulation[fatherIndex], newPopulation[motherIndex]);
			this->Mutate(newChromosome);
			newPopulation.insert(newPopulation.end(), newChromosome.begin(); newChromosome.end());
		}

		++_generationNumber;
		_chromosomePopulation = newPopulation;
	}

	virtual Chromosome const & GetBestChromosome() const = 0;

protected:
	// override this method
	virtual Chromosome CreateInitialRandomChromosome() = 0;
	virtual PopulationType GetSurvivors() = 0;
	virtual void Mutate(PopulationType const & population) = 0;
	virtual PopulationType CrossOver(Chromosome const & father, Chromosome const & mother) = 0;

	void CreateInitialPopulation()
	{
		for (int i = 0; i < this->_chromosomePopulationSize; i++)
			_chromosomePopulation.push_back(CreateInitialRandomChromosome());
	}

	unsigned int _chromosomePopulationSize;
	unsigned int _generationSurvivorCount;
	unsigned int _generationNumber;

	PopulationType _chromosomePopulation;
};

