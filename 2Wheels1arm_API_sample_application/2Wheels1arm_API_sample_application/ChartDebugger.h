#pragma once

#include <vector>

#include <chartdir.h>

class ChartDebugger
{
public:
	ChartDebugger(void);
	~ChartDebugger(void);

	void AddGenerationData(double averageFitness, double bestFitness);
	void Render();
private:
	std::vector<double> _generationData;
	std::vector<double> _averageFitnessScore;
	std::vector<double> _bestRobotFitnessScore;
};

