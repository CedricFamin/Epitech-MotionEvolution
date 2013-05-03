#pragma once

#include <map>

class Blli
{
public:
	Blli(const std::map<int, float**> _luts);
	~Blli(void);

	int		GetIndex(int _table, int _servo, int _cycle);
	float	GetSpeed(int _table, int _servo, int _cycle);

	float	GetScore();
	void	SetScore(float _score);
	int		GetRank();
	void	SetRank(int _rank);
	float   getEfficiancyScore();
	float   getTrajectoryScore();
	float   getSecurityScore();
	void	setEfficiancyScore(float score);
	void	setSecurityScore(float score);
	void	setTrajectoryScore(float score);

	const std::map<int, float**>&	GetLuts();

	enum
	{
		INDEX = 0,
		SPEED
	};

private:
	std::map<int, float**>	_Luts;
	float					_Score;
	int						_Rank;
	float					_efficiancyScore;
	float					_securityScore;
	float					_trajectoryScore;
};
