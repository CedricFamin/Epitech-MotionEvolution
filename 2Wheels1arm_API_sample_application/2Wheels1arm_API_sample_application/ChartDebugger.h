#pragma once

#include <vector>

#include <chartdir.h>

class ChartDebugger
{
public:
	ChartDebugger(void);
	~ChartDebugger(void);

	void AddGenerationData(double averageFitness, double bestFitness);
	void CleanPhaseDuration();
	void AddPhaseDurationValue(unsigned int phaseDuration);
	void Render();
private:
	void RenderFitnessGraph();
	void RenderPhaseDurationRepartition();

	std::vector<double> _generationData;
	std::vector<double> _averageFitnessScore;
	std::vector<double> _bestRobotFitnessScore;

	std::vector<double> _phaseDurationRepartition;
};

