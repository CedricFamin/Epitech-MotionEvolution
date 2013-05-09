#include "MovementGeneticAlgorithm.h"

#include <queue>
#include <time.h>

namespace
{
	// --------------------------------------------------------------
	// Tools
	// --------------------------------------------------------------
	typedef std::pair<MovementChromosome*, MovementChromosome*> PairChromosome;
	template<typename T>
	std::pair<T*, T*> GetRandomPtr(T & c1, T & c2)
	{
		bool invert = rand() & 1;
		std::pair<T*, T*> result;
		if (invert)
		{
			result.first = &c2;
			result.second = &c1;
		}
		else
		{
			result.first = &c1;
			result.second = &c2;
		}
		return result;
	}
	// --------------------------------------------------------------
	void CrossOverMove(Movement & s1, Movement & s2, Movement const & father, Movement const & mother)
	{
		std::pair<Movement*, Movement*> randomPair = GetRandomPtr(s1, s2);
		randomPair.first->AngularSpeed(father.AngularSpeed());
		randomPair.second->AngularSpeed(mother.AngularSpeed());

		randomPair = GetRandomPtr(s1, s2);
		randomPair.first->FinalPosition(father.FinalPosition());
		randomPair.second->FinalPosition(mother.FinalPosition());
	}
	// --------------------------------------------------------------
	void CrossOverMovePhase(MovementSequence::ArmMove & s1, MovementSequence::ArmMove & s2, MovementSequence::ArmMove const & father, MovementSequence::ArmMove const & mother)
	{
		CrossOverMove(s1.armMove, s2.armMove, father.armMove, mother.armMove);
		CrossOverMove(s1.shoulderMove, s2.shoulderMove, father.shoulderMove, mother.shoulderMove);
		CrossOverMove(s1.fingerMove, s2.fingerMove, father.fingerMove, mother.fingerMove);
	}
	// --------------------------------------------------------------
	unsigned int GetRandomValue(unsigned int max)
	{
		return rand() % max;
	}
	// --------------------------------------------------------------
	static unsigned int POSITION_MAX = 1024;
	static unsigned int ANGULAR_SPEED_MAX = 64;
	static unsigned int PHASE_DURATION_MAX = 500;
	static double MUTATION_PROBABILTY = 0.01;
}

// --------------------------------------------------------------
// MovementGeneticAlgorithm
// --------------------------------------------------------------
MovementGeneticAlgorithm::MovementGeneticAlgorithm(void)
	: AGeneticAlgorithm<MovementChromosome>(50, 25)
	, _randomAngularSpeed(0, ANGULAR_SPEED_MAX)
	, _randomPosition(0, POSITION_MAX)
	, _randomBool(0, 1)
	, _randomDuration(0, PHASE_DURATION_MAX)
	, _randomProbabilistic(0.0f, 1.0f)
	, _randomEngine(time(NULL))
{

}

// --------------------------------------------------------------
// --------------------------------------------------------------
MovementGeneticAlgorithm::~MovementGeneticAlgorithm(void)
{
}

// --------------------------------------------------------------
// --------------------------------------------------------------
unsigned int MovementGeneticAlgorithm::RandomAngularSpeed()
{
	return this->_randomAngularSpeed(this->_randomEngine);
}

// --------------------------------------------------------------
// --------------------------------------------------------------
unsigned int MovementGeneticAlgorithm::RandomPosition()
{
	return this->_randomPosition(this->_randomEngine);
}

// --------------------------------------------------------------
// --------------------------------------------------------------
unsigned int MovementGeneticAlgorithm::RandomDuration()
{
	return this->_randomDuration(this->_randomEngine);
}

// --------------------------------------------------------------
// --------------------------------------------------------------
bool MovementGeneticAlgorithm::RandomBool()
{
	return static_cast<bool>(this->_randomBool(this->_randomEngine));
}

// --------------------------------------------------------------
// --------------------------------------------------------------
double MovementGeneticAlgorithm::RandomProbabilistic()
{
	return this->_randomProbabilistic(this->_randomEngine);
}

// --------------------------------------------------------------
// --------------------------------------------------------------
MovementChromosome MovementGeneticAlgorithm::CreateInitialRandomChromosome()
{
	MovementChromosome chromosome;


	chromosome.GetValue().FirstPhase().armMove.AngularSpeed(RandomAngularSpeed());
	chromosome.GetValue().FirstPhase().armMove.FinalPosition(RandomPosition());
	chromosome.GetValue().FirstPhase().shoulderMove.AngularSpeed(RandomAngularSpeed());
	chromosome.GetValue().FirstPhase().shoulderMove.FinalPosition(RandomPosition());
	chromosome.GetValue().FirstPhase().fingerMove.AngularSpeed(RandomAngularSpeed());
	chromosome.GetValue().FirstPhase().fingerMove.FinalPosition(RandomPosition());

	chromosome.GetValue().SecondPhase().armMove.AngularSpeed(RandomAngularSpeed());
	chromosome.GetValue().SecondPhase().armMove.FinalPosition(RandomPosition());
	chromosome.GetValue().SecondPhase().shoulderMove.AngularSpeed(RandomAngularSpeed());
	chromosome.GetValue().SecondPhase().shoulderMove.FinalPosition(RandomPosition());
	chromosome.GetValue().SecondPhase().fingerMove.AngularSpeed(RandomAngularSpeed());
	chromosome.GetValue().SecondPhase().fingerMove.FinalPosition(RandomPosition());

	chromosome.GetValue().PhaseDuration(RandomDuration());

	return chromosome;
}

// --------------------------------------------------------------
// --------------------------------------------------------------
struct ChromosomeComparator
{
	bool operator()(MovementChromosome const & c1, MovementChromosome const & c2)
	{
		return c2.Fitness() > c1.Fitness();
	}
};

MovementGeneticAlgorithm::PopulationType MovementGeneticAlgorithm::GetSurvivors()
{
	PopulationType population;
	std::priority_queue<MovementChromosome, std::vector<MovementChromosome>, ChromosomeComparator> chromosomeQueue;
	for (MovementChromosome & chromosome : this->_chromosomePopulation)
	{
		chromosomeQueue.push(chromosome);
	}

	for (unsigned int i = 0; i < this->_generationSurvivorCount; ++i)
	{
		population.push_back(chromosomeQueue.top());
		chromosomeQueue.pop();
	}
	return population;
}

// --------------------------------------------------------------
// --------------------------------------------------------------
MovementGeneticAlgorithm::PopulationType MovementGeneticAlgorithm::CrossOver(MovementChromosome const & father, MovementChromosome const & mother)
{
	PopulationType newPop;

	MovementChromosome s1;
	MovementChromosome s2;

	{
		PairChromosome randomPair = GetRandomPtr(s1, s2);
		randomPair.first->GetValue().PhaseDuration(father.GetValue().PhaseDuration());
		randomPair.second->GetValue().PhaseDuration(mother.GetValue().PhaseDuration());
	}
	CrossOverMovePhase(s1.GetValue().FirstPhase(), s2.GetValue().FirstPhase(), father.GetValue().FirstPhase(), mother.GetValue().FirstPhase());
	CrossOverMovePhase(s1.GetValue().SecondPhase(), s2.GetValue().SecondPhase(), father.GetValue().SecondPhase(), mother.GetValue().SecondPhase());

	newPop.push_back(s1);
	newPop.push_back(s2);
	return newPop;
}

// --------------------------------------------------------------
// --------------------------------------------------------------
void MovementGeneticAlgorithm::Mutate(PopulationType & population)
{
	for (MovementChromosome & chromosome : population)
	{
		if (RandomProbabilistic() <= MUTATION_PROBABILTY)
			chromosome.GetValue().FirstPhase().armMove.AngularSpeed(RandomAngularSpeed());
		if (RandomProbabilistic() <= MUTATION_PROBABILTY)
			chromosome.GetValue().FirstPhase().armMove.FinalPosition(RandomPosition());
		if (RandomProbabilistic() <= MUTATION_PROBABILTY)
			chromosome.GetValue().FirstPhase().shoulderMove.AngularSpeed(RandomAngularSpeed());
		if (RandomProbabilistic() <= MUTATION_PROBABILTY)
			chromosome.GetValue().FirstPhase().shoulderMove.FinalPosition(RandomPosition());
		if (RandomProbabilistic() <= MUTATION_PROBABILTY)
			chromosome.GetValue().FirstPhase().fingerMove.AngularSpeed(RandomAngularSpeed());
		if (RandomProbabilistic() <= MUTATION_PROBABILTY)
			chromosome.GetValue().FirstPhase().fingerMove.FinalPosition(RandomPosition());

		if (RandomProbabilistic() <= MUTATION_PROBABILTY)
			chromosome.GetValue().SecondPhase().armMove.AngularSpeed(RandomAngularSpeed());
		if (RandomProbabilistic() <= MUTATION_PROBABILTY)
			chromosome.GetValue().SecondPhase().armMove.FinalPosition(RandomPosition());
		if (RandomProbabilistic() <= MUTATION_PROBABILTY)
			chromosome.GetValue().SecondPhase().shoulderMove.AngularSpeed(RandomAngularSpeed());
		if (RandomProbabilistic() <= MUTATION_PROBABILTY)
			chromosome.GetValue().SecondPhase().shoulderMove.FinalPosition(RandomPosition());
		if (RandomProbabilistic() <= MUTATION_PROBABILTY)
			chromosome.GetValue().SecondPhase().fingerMove.AngularSpeed(RandomAngularSpeed());
		if (RandomProbabilistic() <= MUTATION_PROBABILTY)
			chromosome.GetValue().SecondPhase().fingerMove.FinalPosition(RandomPosition());

		if (RandomProbabilistic() <= MUTATION_PROBABILTY)
			chromosome.GetValue().PhaseDuration(RandomDuration());
	}
}