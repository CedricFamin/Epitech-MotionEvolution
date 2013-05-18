#include "MovementEvaluator.h"


MovementEvaluator::MovementEvaluator(void)
{
}


MovementEvaluator::~MovementEvaluator(void)
{
}

void MovementEvaluator::Init(ApiBlli::Position const & position)
{
	this->_beginPos = position;
	this->_secure = true;
}

void MovementEvaluator::SetSecure(bool secure)
{
	this->_secure = secure;
}

void MovementEvaluator::SetFinalPosition(ApiBlli::Position const & position)
{
	this->_endPos = position;
}

void MovementEvaluator::SetMovementDuration(unsigned int duration)
{
	this->_duration = duration;
}

#undef min
double MovementEvaluator::GetScore() const
{
	int resultAsInt = 0;
	float dist = fabs(this->_beginPos.x - this->_endPos.x) +
				fabs(this->_beginPos.y - this->_endPos.y) +
				fabs(this->_beginPos.z - this->_endPos.z);
	float vitesse = dist * 1000.0f /  ((float)this->_duration);
	bool xdeplacement = (this->_beginPos.x - this->_endPos.x) > 0;
	bool ydeplacement = fabs(this->_beginPos.y - this->_endPos.y) > .01f;
	bool zdeplacement = fabs(this->_beginPos.z - this->_endPos.z) < .1f;

	std::cout << "Evaluator {" << dist << ", " << vitesse << ", " << xdeplacement << ", " << ydeplacement << ", " << zdeplacement << "}" << std::endl;
	
	// Bonus distance parcourue
	resultAsInt += (this->_beginPos.x - this->_endPos.x) * 1000;
	// Malus pas tout droit
	// resultAsInt -= (this->_beginPos.y - this->_endPos.y) * 500;
	// Bonus si on ne se met pas en danger
	resultAsInt -= this->_secure * 200;

	return static_cast<double>(resultAsInt);
}