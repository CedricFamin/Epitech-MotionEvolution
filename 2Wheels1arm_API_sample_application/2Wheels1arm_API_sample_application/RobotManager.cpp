#include "RobotManager.h"

RobotManager::RobotManager()
{
	this->_exit = false;
	this->_angularSpeed = 10;
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
	int index;

	for (int i=0; i<100; i++)
	{
		index = 512;
		if (i % 2 == 0)
			index = 400;
		this->_robot->SetIndex(ApiBlli::Leg::FINGER, index, this->_angularSpeed);
		this->_robot->SetIndex(ApiBlli::Leg::ARM, index, this->_angularSpeed);
		this->_robot->SetIndex(ApiBlli::Leg::SHOULDER, index, this->_angularSpeed);
		printLegInCollision();
		printFingerTouching();
		printPlatineTouching();
		printGyroValues();
		Sleep(500);
	}
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

