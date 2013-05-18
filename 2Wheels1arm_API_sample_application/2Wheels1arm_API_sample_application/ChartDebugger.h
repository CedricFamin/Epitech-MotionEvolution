#pragma once

#include <vector>

#include <chartdir.h>
#include "Movement.h"

class ChartDebugger
{
private:
	// Graph lib
	XYChart * CreateGraph(std::string const & title, std::string const & xAxisName, std::string const & yAxisName) const;
	void RenderLine(XYChart *c, std::string const & name, std::vector<double> const & xdata, std::vector<double> const & ydata, unsigned int color) const;
	void RenderPoint(XYChart *c, std::string const & name, std::vector<double> const & data, unsigned int color, Chart::SymbolType symbol) const;
public:
	ChartDebugger(void);
	~ChartDebugger(void);

	void AddGenerationData(double averageFitness, double bestFitness);
	void CleanPhaseDuration();
	void AddPhaseDurationValue(unsigned int phaseDuration);
	void AddPhaseMove(MovementSequence const & move);

	void Render();
private:
	void RenderFitnessGraph();
	void RenderPhaseDurationRepartition(); 
	void RenderFirstPhaseMouvement();
	void RenderSecondPhaseMouvement();
	void RenderFirstPhaseAngularSpeed();
	void RenderSecondPhaseAngularSpeed();

	

	std::vector<double> _generationData;
	std::vector<double> _averageFitnessScore;
	std::vector<double> _bestRobotFitnessScore;

	std::vector<double> _phaseDurationRepartition;

	struct MouvementRepartition
	{
		std::vector<double> armMouvementRepartition;
		std::vector<double> shoulderMouvementRepartition;
		std::vector<double> fingerMouvementRepartition;

		std::vector<double> armAngularSpeedRepatition;
		std::vector<double> shoulderAngularSpeedRepatition;
		std::vector<double> fingerAngularSpeedRepatition;

		void Init();
		void Clean();
	};

	MouvementRepartition _firstMovementRepartion;
	MouvementRepartition _secondMovementRepartion;
};

