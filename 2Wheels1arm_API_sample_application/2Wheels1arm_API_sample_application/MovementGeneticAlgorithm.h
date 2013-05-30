#pragma once

#include <random>

#include "GenericGeneticAlgorithm.h"
#include "Movement.h"

class MovementChromosome : public IChromosome<MovementSequence>
{
public:
	virtual ~MovementChromosome(void)  {}

	virtual float Fitness(void) const { return this->_fitnessScore; }
	void Fitness(float value) { this->_fitnessScore = value > 0 ? value : 0; }
	virtual MovementSequence& GetValue(void) { return this->_data; } 
	virtual MovementSequence const & GetValue(void) const { return this->_data; } 
private:
	MovementSequence _data;
	float _fitnessScore;
};

class MovementGeneticAlgorithm : public AGeneticAlgorithm<MovementChromosome>
{
public:
	typedef std::pair<MovementChromosome *, double> SelectionChromosomeType;
	typedef std::list<SelectionChromosomeType> WheelType;
	
	MovementGeneticAlgorithm(void);
	virtual ~MovementGeneticAlgorithm(void);

	virtual MovementChromosome & GetBestChromosome()
	{
		unsigned int index_max = 0;
		float max_score = FLT_MIN;
		for (unsigned int i = 0; i < this->_chromosomePopulation.size(); ++i)
		{
			if (this->_chromosomePopulation[i].Fitness() > max_score)
			{
				index_max = i;
				max_score = this->_chromosomePopulation[i].Fitness();
			}
		}
		return this->_chromosomePopulation[index_max];
	}

protected:
	// override this method
	virtual void PrepareSelection();
	virtual FatherMotherType Selection();
	virtual MovementChromosome CreateInitialRandomChromosome();
	virtual PopulationType GetSurvivors();
	virtual void Mutate(PopulationType & population);
	virtual PopulationType CrossOver(MovementChromosome const & father, MovementChromosome const & mother);

private:
	unsigned int RandomAngularSpeed();
	unsigned int RandomPosition();
	unsigned int RandomDuration();
	bool RandomBool();
	double RandomProbabilistic();


	std::uniform_int_distribution<unsigned int> _randomAngularSpeed;
	std::uniform_int_distribution<unsigned int> _randomPosition;
	std::uniform_int_distribution<unsigned int> _randomDuration;
	std::uniform_int_distribution<int>			_randomBool;
	std::uniform_real_distribution<double>		_randomProbabilistic;
	std::mt19937								_randomEngine;

	WheelType _wheel;
	float _scaling;

};

