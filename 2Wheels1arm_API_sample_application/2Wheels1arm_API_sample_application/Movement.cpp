#include "Movement.h"

// --------------------------------------------------------------
// Movement
// --------------------------------------------------------------
Movement::Movement()
: _part(0)
{
}

// --------------------------------------------------------------
// --------------------------------------------------------------
Movement::Movement(unsigned int part)
: _part(part)
{
}

// --------------------------------------------------------------
// --------------------------------------------------------------
Movement::~Movement(void)
{
}

// --------------------------------------------------------------
// MovementSequence
// --------------------------------------------------------------
MovementSequence::MovementSequence()
{
	this->_firstMove.armMove = Movement(ApiBlli::Leg::ARM);
	this->_firstMove.shoulderMove = Movement(ApiBlli::Leg::SHOULDER);
	this->_firstMove.fingerMove = Movement(ApiBlli::Leg::FINGER);

	this->_secondMove.armMove = Movement(ApiBlli::Leg::ARM);
	this->_secondMove.shoulderMove = Movement(ApiBlli::Leg::SHOULDER);
	this->_secondMove.fingerMove = Movement(ApiBlli::Leg::FINGER);

	this->_phaseDuration = 500;
}

// --------------------------------------------------------------
// --------------------------------------------------------------
MovementSequence::~MovementSequence()
{
}