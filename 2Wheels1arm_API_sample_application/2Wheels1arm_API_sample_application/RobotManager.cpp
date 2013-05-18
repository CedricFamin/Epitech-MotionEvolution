#include "RobotManager.h"

#include "ChartDebugger.h"
#include "MovementGeneticAlgorithm.h"
#include "MovementEvaluator.h"

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

void RobotManager::RunSequence(MovementChromosome & chromosome)
{
	static unsigned int TIME_MULTIPLIER = 10;
	MovementEvaluator evaluator;

	this->_debugger.AddPhaseDurationValue(chromosome.GetValue().PhaseDuration());
	this->_debugger.AddPhaseMove(chromosome.GetValue());

	this->_robot->Init();
	Sleep(500);
	evaluator.Init(*this->_robot->GetPosition());
	evaluator.SetMovementDuration(chromosome.GetValue().PhaseDuration() * 2);
	chromosome.Fitness(0.0f);
	MovementSequence const & movementSequence = chromosome.GetValue();
			
	for (unsigned int i = 0; i < 10; ++i)
	{
		MovementSequence::ArmMove const & move = (i%2) ? movementSequence.SecondPhase() : movementSequence.FirstPhase();

		this->_robot->SetIndex(ApiBlli::Leg::FINGER, move.fingerMove.FinalPosition(), move.fingerMove.AngularSpeed());
		this->_robot->SetIndex(ApiBlli::Leg::ARM, move.armMove.FinalPosition(), move.armMove.AngularSpeed());
		this->_robot->SetIndex(ApiBlli::Leg::SHOULDER, move.shoulderMove.FinalPosition(), move.shoulderMove.AngularSpeed());
		Sleep(movementSequence.PhaseDuration() / TIME_MULTIPLIER);
		if (this->_robot->IsInColision() && !this->_robot->IsTuchingGround())
			evaluator.SetSecure(false);
	}
	evaluator.SetFinalPosition(*this->_robot->GetPosition());
	chromosome.Fitness(evaluator.GetScore());
	std::cout << "Fitness " << chromosome.Fitness() << std::endl;
	
}

void RobotManager::runSequence()
{
	MovementGeneticAlgorithm geneticAlgorithm;
	geneticAlgorithm.CreateInitialPopulation();

	while (1)
	{
		MovementGeneticAlgorithm::PopulationType & population = geneticAlgorithm.ChromosomePopulation();
		double averageFitness = 0;
		this->_debugger.CleanPhaseDuration();

		for (MovementChromosome & chromosome : population)
		{
			this->RunSequence(chromosome);
			averageFitness += chromosome.Fitness();
		}
		averageFitness/= population.size();

		MovementChromosome & chromosome = geneticAlgorithm.GetBestChromosome();
		this->_debugger.AddGenerationData(averageFitness, chromosome.Fitness());
		this->_debugger.Render();

		std::cout << "Generation " << geneticAlgorithm.GenerationNumber() << std::endl 
			<< "Best robot: score " << chromosome.Fitness() << std::endl
			<< "Average Fitness " << averageFitness << std::endl;

		this->RunSequence(chromosome);
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

