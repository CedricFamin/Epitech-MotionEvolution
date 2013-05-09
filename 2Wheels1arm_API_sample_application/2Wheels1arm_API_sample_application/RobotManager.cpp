#include "RobotManager.h"

#include "ChartDebugger.h"
#include "MovementGeneticAlgorithm.h"

RobotManager::RobotManager()
{
	this->_exit = false;
	this->_angularSpeed = 1;
	this->initRobot();
	this->runSequence();
	this->pauseAndExit();
}


RobotManager::~RobotManager(void)
{
	if (this->_robot != NULL)
		delete this->_robot;
}

void RobotManager::initRobot()
{
	ApiBlli::ApiBlli::GetInstance()->Init("localhost");
	std::list<ApiBlli::Robot*> robotList = ApiBlli::ApiBlli::GetInstance()->GetRobots();
	if (robotList.size() != 0 )
	{
		this->_robot = robotList.front();
	}
	else
	{
		printf("No robot found, ");
		this->pauseAndExit();
	}
}

void RobotManager::pauseAndExit()
{
	std::string line;
	printf("press any key to exit\n");
		while (this->_exit == false)
		{
			if (std::getline(std::cin, line)) 
			{
				this->_exit = true;
			}
		}
		exit(0);
}

void RobotManager::runSequence()
{
	static unsigned int TIME_MULTIPLIER = 10;
	MovementGeneticAlgorithm geneticAlgorithm;
	ChartDebugger debugger;

	geneticAlgorithm.CreateInitialPopulation();
	while (1)
	{
		MovementGeneticAlgorithm::PopulationType & population = geneticAlgorithm.ChromosomePopulation();
		double averageFitness = 0;

		for (MovementChromosome & chromosome : population)
		{
			this->_robot->Init();
			Sleep(500);
			ApiBlli::Position beginPos = *this->_robot->GetPosition();
			chromosome.Fitness(0.0f);
			MovementSequence const & movementSequence = chromosome.GetValue();

			for (unsigned int i = 0; i < 10; ++i)
			{
				MovementSequence::ArmMove const & move = (i%2) ? movementSequence.SecondPhase() : movementSequence.FirstPhase();

				this->_robot->SetIndex(ApiBlli::Leg::FINGER, move.fingerMove.FinalPosition(), move.fingerMove.AngularSpeed());
				this->_robot->SetIndex(ApiBlli::Leg::ARM, move.armMove.FinalPosition(), move.armMove.AngularSpeed());
				this->_robot->SetIndex(ApiBlli::Leg::SHOULDER, move.shoulderMove.FinalPosition(), move.shoulderMove.AngularSpeed());
				//chromosome.Fitness(chromosome.Fitness() + EvalRobot());
				Sleep(movementSequence.PhaseDuration() / TIME_MULTIPLIER);
			}
			ApiBlli::Position lastPos = *this->_robot->GetPosition();
			chromosome.Fitness(
				fabs(beginPos.x - lastPos.x) -
				fabs(beginPos.y - lastPos.y) -
				fabs(beginPos.z - lastPos.z)
				);
			std::cout << "Fitness " << chromosome.Fitness() << std::endl;
			averageFitness += chromosome.Fitness();
		}

		averageFitness/= population.size();
		MovementChromosome const & chromosome = geneticAlgorithm.GetBestChromosome();
		debugger.AddGenerationData(averageFitness, chromosome.Fitness());
		debugger.Render();

		std::cout << "Generation " << geneticAlgorithm.GenerationNumber() << std::endl 
			<< "Best robot: score " << chromosome.Fitness() << std::endl
			<< "Average Fitness " << averageFitness << std::endl;

		{
			this->_robot->Init();
			Sleep(500);
			MovementSequence const & movementSequence = chromosome.GetValue();
			for (unsigned int i = 0; i < 10; ++i)
			{
				MovementSequence::ArmMove const & move = (i%2) ? movementSequence.SecondPhase() : movementSequence.FirstPhase();
				this->_robot->SetIndex(ApiBlli::Leg::FINGER, move.fingerMove.FinalPosition(), move.fingerMove.AngularSpeed() * TIME_MULTIPLIER);
				this->_robot->SetIndex(ApiBlli::Leg::ARM, move.armMove.FinalPosition(), move.armMove.AngularSpeed() * TIME_MULTIPLIER);
				this->_robot->SetIndex(ApiBlli::Leg::SHOULDER, move.shoulderMove.FinalPosition(), move.shoulderMove.AngularSpeed() * TIME_MULTIPLIER);
				Sleep(movementSequence.PhaseDuration() / TIME_MULTIPLIER);
			}
		}
		std::cout << "New generation" << std::endl;
		geneticAlgorithm.MakeEvolution();
		
	}
}

float RobotManager::EvalRobot()
{
	float result = 0.0f;
	ApiBlli::GyroValues * values =this->_robot->GetGyroValues();

	result += values->LinearAccelerations[0] * values->LinearAccelerations[0];
	result += values->LinearAccelerations[1] * values->LinearAccelerations[1];
	result += values->LinearAccelerations[2] * values->LinearAccelerations[2];
	return result;
}
void RobotManager::printLegInCollision()
{
	if (this->_robot->IsInColision())
		std::cout<< "Leg Collision : YES" << std::endl;
	else
		std::cout<< "Leg Collision : NO"<< std::endl;
}

void RobotManager::printFingerTouching()
{
	if (this->_robot->IsLegTuchingGround())
		printf("Finger touching ground : YES\n");
	else
		printf("Finger touching ground : NO\n");
}

void RobotManager::printPlatineTouching()
{
	if (this->_robot->IsTuchingGround())
		printf("Tail touching ground : YES\n");
	else
		printf("Tail touching ground : NO\n");
}

void RobotManager::printGyroValues()
{
	ApiBlli::GyroValues * values =this->_robot->GetGyroValues();
	std::string resultStr = "";

	//	//Display axis XYZ accelerations
		printf("Gyro Values : Linear acceleration (%f %f %f) Angle (%f %f %f)\n",
				values->LinearAccelerations[0],values->LinearAccelerations[1],values->LinearAccelerations[2],
				xkode::lib::Math::ToDeg(values->Angles[0]),xkode::lib::Math::ToDeg(values->Angles[1]),xkode::lib::Math::ToDeg(values->Angles[2]) );
}

