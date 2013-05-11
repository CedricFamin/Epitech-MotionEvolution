#pragma once
#include "ApiBlli.h"
#include <fstream>
#include <iostream>

#include "ChartDebugger.h"
#include "MovementGeneticAlgorithm.h"

class RobotManager
{
public:
	RobotManager();
	~RobotManager(void);
	void initRobot();
	void pauseAndExit();
	void runSequence();
	void printLegInCollision();
	void printGyroValues();
	void printFingerTouching();
	void printPlatineTouching();
	float EvalRobot();

private:
	void RunSequence(MovementChromosome & chromosome);

	ApiBlli::Robot* _robot;
	int							 _timer;
	float						 _angularSpeed;
	ModaCPP::RobotPHX			*_pPHX;
	std::string					 _name;
	bool						 _exit;
	ModaCPP::DeviceAccelGyro	*_gyro;
	ModaCPP::DeviceContact		*_contact_platine;
	ChartDebugger				_debugger;
};

