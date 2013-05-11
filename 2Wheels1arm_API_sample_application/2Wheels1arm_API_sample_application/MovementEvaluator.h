#pragma once
#include "ApiBlli.h"

class MovementEvaluator
{
public:
	MovementEvaluator(void);
	~MovementEvaluator(void);

	void Init(ApiBlli::Position const & position);
	void SetSecure(bool secure);
	void SetFinalPosition(ApiBlli::Position const & position);
	void SetMovementDuration(unsigned int duration);

	double GetScore() const;

protected:
private:
	bool				_secure;
	unsigned int		_duration;
	ApiBlli::Position	_beginPos;
	ApiBlli::Position	_endPos;
};

