#include "ChartDebugger.h"


ChartDebugger::ChartDebugger(void)
{
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
	this->_generationData.push_back(this->_generationData.size() + 1);
	this->_averageFitnessScore.push_back(averageFitness);
	this->_bestRobotFitnessScore.push_back(bestFitness);
}

// --------------------------------------------------------------
// --------------------------------------------------------------
void ChartDebugger::Render()
{
    // Create a XYChart object of size 600 x 300 pixels, with a light grey (cccccc)
    // background, black border, and 1 pixel 3D border effect.
    XYChart *c = new XYChart(600, 300, 0xcccccc, 0x000000, 1);

    // Set the plotarea at (55, 58) and of size 520 x 195 pixels, with white
    // background. Turn on both horizontal and vertical grid lines with light grey
    // color (cccccc)
    c->setPlotArea(55, 58, 520, 195, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
    c->addLegend(55, 32, false, "arialbd.ttf", 9)->setBackground(Chart::Transparent);
    c->addTitle("Fitness score evolution", "timesbi.ttf", 15, 0xffffff)->setBackground(0x000000, -1, 1);
    c->yAxis()->setTitle("Fitness score");
    c->xAxis()->setTitle("Generation");

    // Set the axes width to 2 pixels
    c->xAxis()->setWidth(2);
    c->yAxis()->setWidth(2);

	{
		// Add a purple (800080) spline layer to the chart with a line width of 2 pixels
		SplineLayer *splineLayer = c->addSplineLayer(DoubleArray(this->_averageFitnessScore.data(), this->_averageFitnessScore.size()), 0x800080, "Average Fitness Score");
		splineLayer->setXData(DoubleArray(this->_generationData.data(), this->_generationData.size()));
		splineLayer->setLineWidth(2);
	}
	{
		// Add a purple (800080) spline layer to the chart with a line width of 2 pixels
		SplineLayer *splineLayer = c->addSplineLayer(DoubleArray(this->_bestRobotFitnessScore.data(), this->_bestRobotFitnessScore.size()), 0xFF0000, "Best Fitness Score");
		splineLayer->setXData(DoubleArray(this->_generationData.data(), this->_generationData.size()));
		splineLayer->setLineWidth(2);
	}

    // Output the chart
    c->makeChart("output/fitnessEvolution.png");

    //free up resources
    delete c;
}