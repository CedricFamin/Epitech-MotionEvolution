#include "Blli.h"

Blli::Blli(const std::map<int, float**> _luts)
{
	std::map<int, float**>::const_iterator itb = _luts.begin();
	std::map<int, float**>::const_iterator ite = _luts.end();
	for (; itb != ite; ++itb)
	{
		float** lut = new float*[100];
		for (int i = 0; i < 100; ++i)
		{
			lut[i] = new float[2];
			lut[i][0] = itb->second[i][0];
			lut[i][1] = itb->second[i][1];
		}
		this->_Luts[itb->first] = lut;
	}
}

Blli::~Blli(void)
{
	std::map<int, float**>::iterator itb = this->_Luts.begin();
	std::map<int, float**>::iterator ite = this->_Luts.end();

	for (; itb != ite; ++itb)
	{
		for (int i = 0; i < 100; ++i)
		{
			delete [] itb->second[i];
		}
		delete [] itb->second;
	}
	this->_Luts.clear();
}

float	Blli::GetScore()
{
	return this->_Score;
}

void	Blli::SetScore(float _score)
{
	this->_Score = _score;
}

int		Blli::GetRank()
{
	return this->_Rank;
}

void	Blli::SetRank(int _rank)
{
	this->_Rank = _rank;
}

int		Blli::GetIndex(int _table, int _servo, int _cycle)
{
	return (int)this->_Luts[_table * 3 + _servo][_cycle][INDEX];
}

float	Blli::GetSpeed(int _table, int _servo, int _cycle)
{
	return this->_Luts[_table * 3 + _servo][_cycle][SPEED];
}

const std::map<int, float**>&	Blli::GetLuts()
{
	return this->_Luts;
}

float   Blli::getEfficiancyScore()
{
	return this->_efficiancyScore;
}

float   Blli::getTrajectoryScore()
{
	return this->_trajectoryScore;
}

float   Blli::getSecurityScore()
{
	return this->_securityScore;
}

void	Blli::setEfficiancyScore(float score)
{
		this->_efficiancyScore = score;
}

void	Blli::setSecurityScore(float score)
{
		this->_securityScore = score;
}

void	Blli::setTrajectoryScore(float score)
{
		this->_trajectoryScore = score;
}