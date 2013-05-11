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
	bool ydeplacement = (this->_beginPos.y - this->_endPos.y) == 0;
	bool zdeplacement = (this->_beginPos.z - this->_endPos.z) == 0;
	std::cout << "Evaluator {" << dist << ", " << vitesse << ", " << xdeplacement << ", " << ydeplacement << ", " << zdeplacement << "}" << std::endl;
	
	resultAsInt += std::min(256, (int)(dist * 128)) << 0;
	/*resultAsInt += this->_secure << 4;
	{
		// vecteur deplacement
		resultAsInt += zdeplacement << 1;
		resultAsInt += ydeplacement << 2;
		resultAsInt += xdeplacement << 3;
	}
	resultAsInt += std::min(8, (int)vitesse) << 5;
	*/



	return static_cast<double>(resultAsInt);
}