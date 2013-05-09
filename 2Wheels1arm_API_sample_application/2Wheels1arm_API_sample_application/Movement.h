#pragma once

#include "Leg.h"

class Movement
{
public:
	Movement();
	// part should be ApiBlli::Leg::ARM ...
	Movement(unsigned int part);
	~Movement(void);

	unsigned int Part() const { return this->_part; }

	void AngularSpeed(unsigned int value) { this->_angularSpeed = value; }
	unsigned int AngularSpeed() const { return this->_angularSpeed; }

	void FinalPosition(unsigned int value) { this->_finalPosition = value; }
	unsigned int FinalPosition() const { return this->_finalPosition ;}

protected:
private:
	unsigned int _angularSpeed;
	unsigned int _finalPosition;
	unsigned int _part;
};

class MovementSequence
{
public:
	
	struct ArmMove
	{
		Movement armMove;
		Movement shoulderMove;
		Movement fingerMove;
	};

	MovementSequence();
	~MovementSequence();

	unsigned int PhaseDuration() const { return this->_phaseDuration; }
	void PhaseDuration(unsigned int value) { this->_phaseDuration = value; }

	ArmMove const & FirstPhase() const { return this->_firstMove; }
	ArmMove const & SecondPhase() const { return this->_secondMove; }
	ArmMove & FirstPhase() { return this->_firstMove; }
	ArmMove & SecondPhase() { return this->_secondMove; }
protected:
private:

	ArmMove _firstMove;
	ArmMove _secondMove;

	unsigned int _phaseDuration;

};

