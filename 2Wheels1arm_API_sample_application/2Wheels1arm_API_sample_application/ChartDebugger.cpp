#include <sstream>

#include "ChartDebugger.h"

// --------------------------------------------------------------
// -------------------------------------------------------------

void ChartDebugger::MouvementRepartition::Init()
{
	this->shoulderMouvementRepartition.resize(1025);
	this->armMouvementRepartition.resize(1025);
	this->fingerMouvementRepartition.resize(1025);

	this->armAngularSpeedRepatition.resize(65);
	this->shoulderAngularSpeedRepatition.resize(65);
	this->fingerAngularSpeedRepatition.resize(65);
}

void ChartDebugger::MouvementRepartition::Clean()
{
	for (double & value : this->shoulderMouvementRepartition)
		value = 0;
	for (double & value : this->armMouvementRepartition)
		value = 0;
	for (double & value : this->fingerMouvementRepartition)
		value = 0;

	for (double & value : this->armAngularSpeedRepatition)
		value = 0;
	for (double & value : this->shoulderAngularSpeedRepatition)
		value = 0;
	for (double & value : this->fingerAngularSpeedRepatition)
		value = 0;
}

// --------------------------------------------------------------
// --------------------------------------------------------------
XYChart * ChartDebugger::CreateGraph(std::string const & title, std::string const & xAxisName, std::string const & yAxisName) const
{
	// Create a XYChart object of size 600 x 300 pixels, with a light grey (cccccc)
    // background, black border, and 1 pixel 3D border effect.
    XYChart *c = new XYChart(600, 300, 0xcccccc, 0x000000, 1);

    // Set the plotarea at (55, 58) and of size 520 x 195 pixels, with white
    // background. Turn on both horizontal and vertical grid lines with light grey
    // color (cccccc)
    c->setPlotArea(55, 58, 520, 195, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
    c->addLegend(55, 32, false, "arialbd.ttf", 9)->setBackground(Chart::Transparent);
	c->addTitle(title.c_str(), "timesbi.ttf", 15, 0xffffff)->setBackground(0x000000, -1, 1);
    c->yAxis()->setTitle(xAxisName.c_str());
    c->xAxis()->setTitle(yAxisName.c_str());

    // Set the axes width to 2 pixels
    c->xAxis()->setWidth(2);
    c->yAxis()->setWidth(2);

	return c;
}


// --------------------------------------------------------------
// --------------------------------------------------------------
void ChartDebugger::RenderLine(XYChart *c, std::string const & name, std::vector<double> const & xdata, std::vector<double> const & ydata, unsigned int color) const
{
	// Add a purple (800080) spline layer to the chart with a line width of 2 pixels
	SplineLayer *splineLayer = c->addSplineLayer(DoubleArray(ydata.data(), ydata.size()), color, name.c_str());
	splineLayer->setXData(DoubleArray(xdata.data(), xdata.size()));
	splineLayer->setLineWidth(2);
}

// --------------------------------------------------------------
// --------------------------------------------------------------
void ChartDebugger::RenderPoint(XYChart *c, std::string const & name, std::vector<double> const & data, unsigned int color, Chart::SymbolType symbol) const
{
	std::vector<double> yValue;
	std::vector<double> xValue;

	for (unsigned int i = 0; i < data.size(); ++i)
	{
		double value = data[i];
		if (value != 0.0)
		{
			yValue.push_back(value);
			xValue.push_back(i);
		}
	}
	// Add an orange (0xff9933) scatter chart layer, using 13 pixel diamonds as
	// symbols
	c->addScatterLayer(DoubleArray(xValue.data(), xValue.size()), 
		DoubleArray(yValue.data(), yValue.size()),
		name.c_str(), symbol, 13, color);
}

// --------------------------------------------------------------
// --------------------------------------------------------------
ChartDebugger::ChartDebugger(void)
{
	this->_phaseDurationRepartition.resize(501);
	this->_firstMovementRepartion.Init();
	this->_secondMovementRepartion.Init();
}

// --------------------------------------------------------------
// --------------------------------------------------------------
ChartDebugger::~ChartDebugger(void)
{
}

// --------------------------------------------------------------
// --------------------------------------------------------------
void ChartDebugger::AddGenerationData(double averageFitness, double bestFitness)
{
	this->_generationData.push_back(this->_generationData.size());
	this->_averageFitnessScore.push_back(averageFitness);
	this->_bestRobotFitnessScore.push_back(bestFitness);
}

// --------------------------------------------------------------
// --------------------------------------------------------------
void ChartDebugger::Render()
{
	this->RenderFitnessGraph();
	this->RenderPhaseDurationRepartition();
	this->RenderFirstPhaseMouvement();
	this->RenderSecondPhaseMouvement();
	this->RenderFirstPhaseAngularSpeed();
	this->RenderSecondPhaseAngularSpeed();
}

// --------------------------------------------------------------
// --------------------------------------------------------------
void ChartDebugger::RenderFitnessGraph()
{
    XYChart *c = this->CreateGraph("Fitness score evolution", "Fitness score", "Generation");
	this->RenderLine(c, "Average Fitness Score", this->_generationData, this->_averageFitnessScore, 0x800080);
	this->RenderLine(c, "Best Fitness Score", this->_generationData, this->_bestRobotFitnessScore, 0xFF0000);
    c->makeChart("output/fitnessEvolution.png");
    delete c;
}


// --------------------------------------------------------------
// --------------------------------------------------------------
void ChartDebugger::CleanPhaseDuration()
{
	this->_phaseDurationRepartition.resize(500 + 1);
	for (double & value : this->_phaseDurationRepartition)
		value = 0;

	this->_firstMovementRepartion.Clean();
	this->_secondMovementRepartion.Clean();
}

// --------------------------------------------------------------
// --------------------------------------------------------------
void ChartDebugger::AddPhaseDurationValue(unsigned int phaseDuration)
{
	this->_phaseDurationRepartition[phaseDuration] += 1;
}


// --------------------------------------------------------------
// --------------------------------------------------------------
void ChartDebugger::RenderPhaseDurationRepartition()
{
	std::ostringstream oss;
    oss << "Phase duration repartition: Generation "  << this->_generationData.size();
	XYChart *c = this->CreateGraph(oss.str(), "Occurence", "Phase duration");

	this->RenderPoint(c, "Repartition", this->_phaseDurationRepartition, 0xff9933, Chart::DiamondSymbol);
    c->makeChart("output/phaseDuration.png");
    delete c;
}

// --------------------------------------------------------------
// --------------------------------------------------------------
void ChartDebugger::AddPhaseMove(MovementSequence const & move)
{
	this->_firstMovementRepartion.armMouvementRepartition[move.FirstPhase().armMove.FinalPosition()] += 1;
	this->_firstMovementRepartion.shoulderMouvementRepartition[move.FirstPhase().shoulderMove.FinalPosition()] += 1;
	this->_firstMovementRepartion.fingerMouvementRepartition[move.FirstPhase().fingerMove.FinalPosition()] += 1;

	this->_firstMovementRepartion.armAngularSpeedRepatition[move.FirstPhase().armMove.AngularSpeed()] += 1;
	this->_firstMovementRepartion.shoulderAngularSpeedRepatition[move.FirstPhase().shoulderMove.AngularSpeed()] += 1;
	this->_firstMovementRepartion.fingerAngularSpeedRepatition[move.FirstPhase().fingerMove.AngularSpeed()] += 1;

	this->_secondMovementRepartion.armMouvementRepartition[move.SecondPhase().armMove.FinalPosition()] += 1;
	this->_secondMovementRepartion.shoulderMouvementRepartition[move.SecondPhase().shoulderMove.FinalPosition()] += 1;
	this->_secondMovementRepartion.fingerMouvementRepartition[move.SecondPhase().fingerMove.FinalPosition()] += 1;

	this->_secondMovementRepartion.armAngularSpeedRepatition[move.SecondPhase().armMove.AngularSpeed()] += 1;
	this->_secondMovementRepartion.shoulderAngularSpeedRepatition[move.SecondPhase().shoulderMove.AngularSpeed()] += 1;
	this->_secondMovementRepartion.fingerAngularSpeedRepatition[move.SecondPhase().fingerMove.AngularSpeed()] += 1;
}

// --------------------------------------------------------------
// --------------------------------------------------------------
void ChartDebugger::RenderFirstPhaseMouvement()
{
	std::ostringstream oss;
    oss << "First move position: Generation "  << this->_generationData.size();
	XYChart *c = this->CreateGraph(oss.str(), "Occurence", "Position");

	this->RenderPoint(c, "Arm", this->_firstMovementRepartion.armMouvementRepartition, 0xff9933, Chart::DiamondSymbol);
	this->RenderPoint(c, "Shoulder", this->_firstMovementRepartion.shoulderMouvementRepartition, 0x99ff33, Chart::TriangleSymbol);
	this->RenderPoint(c, "Finger", this->_firstMovementRepartion.fingerMouvementRepartition, 0x9933ff, Chart::Cross2Symbol);

    // Output the chart
    c->makeChart("output/firstMovePosition.png");

    //free up resources
    delete c;
}

// --------------------------------------------------------------
// --------------------------------------------------------------
void ChartDebugger::RenderSecondPhaseMouvement()
{
	std::ostringstream oss;
    oss << "Second move position: Generation "  << this->_generationData.size();
	XYChart *c = this->CreateGraph(oss.str(), "Occurence", "Position");

	this->RenderPoint(c, "Arm", this->_secondMovementRepartion.armMouvementRepartition, 0xff9933, Chart::DiamondSymbol);
	this->RenderPoint(c, "Shoulder", this->_secondMovementRepartion.shoulderMouvementRepartition, 0x99ff33, Chart::TriangleSymbol);
	this->RenderPoint(c, "Finger", this->_secondMovementRepartion.fingerMouvementRepartition, 0x9933ff, Chart::Cross2Symbol);

    // Output the chart
    c->makeChart("output/SecondMovePosition.png");

    //free up resources
    delete c;
}

// --------------------------------------------------------------
// --------------------------------------------------------------
void ChartDebugger::RenderFirstPhaseAngularSpeed()
{
	std::ostringstream oss;
    oss << "First move angular speed: Generation "  << this->_generationData.size();
	XYChart *c = this->CreateGraph(oss.str(), "Occurence", "Speed");

	this->RenderPoint(c, "Arm", this->_firstMovementRepartion.armAngularSpeedRepatition, 0xff9933, Chart::DiamondSymbol);
	this->RenderPoint(c, "Shoulder", this->_firstMovementRepartion.shoulderAngularSpeedRepatition, 0x99ff33, Chart::TriangleSymbol);
	this->RenderPoint(c, "Finger", this->_firstMovementRepartion.fingerAngularSpeedRepatition, 0x9933ff, Chart::Cross2Symbol);

    // Output the chart
    c->makeChart("output/FirstMoveAngularSpeed.png");

    //free up resources
    delete c;
}

// --------------------------------------------------------------
// --------------------------------------------------------------
void ChartDebugger::RenderSecondPhaseAngularSpeed()
{
	std::ostringstream oss;
    oss << "Second move angular speed: Generation "  << this->_generationData.size();
	XYChart *c = this->CreateGraph(oss.str(), "Occurence", "Speed");

	this->RenderPoint(c, "Arm", this->_secondMovementRepartion.armAngularSpeedRepatition, 0xff9933, Chart::DiamondSymbol);
	this->RenderPoint(c, "Shoulder", this->_secondMovementRepartion.shoulderAngularSpeedRepatition, 0x99ff33, Chart::TriangleSymbol);
	this->RenderPoint(c, "Finger", this->_secondMovementRepartion.fingerAngularSpeedRepatition, 0x9933ff, Chart::Cross2Symbol);

    // Output the chart
    c->makeChart("output/SecondMoveAngularSpeed.png");

    //free up resources
    delete c;
}
